// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

////////////////////////////////////////////////////////////
// 20 feb 2012    Andrey Kolesnikov
////////////////////////////////////////////////////////////

#include "h264_mp4_to_annexb.h"

#include <array>

#include <utils/media/nalUnits.h>
#include <utils/media/hevc_decoder_configuration_record.h>
#include <nx/utils/log/log.h>
#include <nx/network/socket_common.h>
#include <nx/streaming/video_data_packet.h>

namespace {

static const std::array<uint8_t, 4> kStartCode = { 0, 0, 0, 1 };
static const std::array<uint8_t, 3> kStartCode3B = { 0, 0, 1 };

int getNalSize(const uint8_t* ptr)
{
    return (ptr[0] << 8) | ptr[1];
}

template <size_t arraySize>
bool startsWith(const void* data, size_t size, const std::array<uint8_t, arraySize> value)
{
    return size >= value.size() && memcmp(data, value.data(), value.size()) == 0;
}

bool isStartCode(const void* data, size_t size)
{
    return startsWith(data, size, kStartCode) || startsWith(data, size, kStartCode3B);
}

void appendNalUnit(std::vector<uint8_t>& result, const uint8_t* data, int size)
{
    result.insert(result.end(), kStartCode.begin(), kStartCode.end());
    result.insert(result.end(), data, data + size);
}

// TODO: Code duplication with "h264_utils.cpp".
std::vector<uint8_t> readH264SeqHeaderFromExtraData(const uint8_t* extraData, int extraDataSize)
{
    std::vector<uint8_t> result;
    if (!extraData || extraDataSize < 8 || extraData[0] != 1)
        return result;

    const uint8_t* p = extraData;
    //sps & pps is in the extradata, parsing it...
    //following code has been taken from libavcodec/h264.c

    int lengthSizeMinusOne = (extraData[4] & 0x03);
    if (lengthSizeMinusOne != 3)
    {
        NX_WARNING(NX_SCOPE_TAG, "Unsupported NAL lenght size: %1, TODO impl this",
            lengthSizeMinusOne + 1);
        return result;
    }

    // Decode sps from avcC
    int cnt = *(p + 5) & 0x1f; // Number of sps
    p += 6;

    for (int i = 0; i < cnt; i++)
    {
        const int nalsize = getNalSize(p);
        p += 2; //skipping nalusize
        if (nalsize > extraDataSize - (p - extraData))
            break;
        appendNalUnit(result, p, nalsize);
        p += nalsize;
    }

    // Decode pps from avcC
    cnt = *(p++); // Number of pps
    for (int i = 0; i < cnt; ++i)
    {
        const int nalsize = getNalSize(p);
        p += 2;
        if (nalsize > extraDataSize - (p - extraData))
            break;

        appendNalUnit(result, p, nalsize);
        p += nalsize;
    }
    return result;
}

void convertToStartCodes(uint8_t* const data, const int size)
{
    uint8_t* offset = data;
    while (offset + 4 < data + size)
    {
        size_t naluSize = ntohl(*(uint32_t*)offset);
        memcpy(offset, kStartCode.data(), kStartCode.size());
        offset += kStartCode.size() + naluSize;
    }
}

} // namespace

std::vector<uint8_t> readH265SeqHeaderFromExtraData(const uint8_t* extraData, int extraDataSize)
{
    std::vector<uint8_t> result;
    if (!extraData)
        return result;

    nx::media::hevc::HEVCDecoderConfigurationRecord hvcc;
    if (!hvcc.read(extraData, extraDataSize))
    {
        NX_WARNING(NX_SCOPE_TAG, "Failed to parse hvcc");
        return result;
    }
    if (hvcc.lengthSizeMinusOne != 3)
    {
        NX_WARNING(NX_SCOPE_TAG, "Unsupported NAL lenght size: %1, TODO impl this",
            hvcc.lengthSizeMinusOne + 1);
        return result;
    }
    for (auto& vps: hvcc.vps)
        appendNalUnit(result, vps.data(), vps.size());
    for (auto& sps: hvcc.sps)
        appendNalUnit(result, sps.data(), sps.size());
    for (auto& pps: hvcc.pps)
        appendNalUnit(result, pps.data(), pps.size());
    return result;
}

QnConstAbstractDataPacketPtr H2645Mp4ToAnnexB::processData(const QnConstAbstractDataPacketPtr& data)
{
    const QnCompressedVideoData* videoData =
        dynamic_cast<const QnCompressedVideoData*>(data.get());
    if (!videoData)
        return data;
    auto codecId = videoData->compressionType;
    if (codecId != AV_CODEC_ID_H264 && codecId != AV_CODEC_ID_H265)
        return data;

    if (!videoData->context)
    {
        NX_DEBUG(this, "Invalid video stream, failed to convert to AnnexB format, no extra data");
        return data;
    }

    std::vector<uint8_t> header;
    const uint8_t* extraData = videoData->context->getExtradata();
    int extraDataSize = videoData->context->getExtradataSize();


    if (extraData && extraDataSize)
    {
        if (isStartCode(extraData, extraDataSize))
            return data;
    }
    else
    {
        if (isStartCode(videoData->data(), videoData->dataSize()))
            return data;
    }

    if (videoData->flags.testFlag(QnAbstractMediaData::MediaFlags_AVKey))
    {
        // Reading sequence header from extradata.
        if (codecId == AV_CODEC_ID_H264)
            header = readH264SeqHeaderFromExtraData(extraData, extraDataSize);
        else if (codecId == AV_CODEC_ID_H265)
            header = readH265SeqHeaderFromExtraData(extraData, extraDataSize);
        if (header.empty())
        {
            NX_DEBUG(this,
                "Invalid video stream, failed to convert to AnnexB format, invalid extra data");
            return data;
        }

        auto context = std::make_shared<CodecParameters>(
            videoData->context->getAvCodecParameters());
        context->setExtradata(header.data(), header.size());

        m_newContext = context;
    }

    QnWritableCompressedVideoDataPtr result(new QnWritableCompressedVideoData());
    result->QnCompressedVideoData::assign(videoData);

    const int resultDataSize = header.size() + videoData->dataSize();
    result->m_data.resize(resultDataSize);
    uint8_t* const resultData = (uint8_t*)result->m_data.data();
    memcpy(resultData, header.data(), header.size());
    memcpy(resultData + header.size(), videoData->data(), videoData->dataSize());

    // Replacing NALU size with {0 0 0 1}.
    convertToStartCodes(resultData + header.size(), videoData->dataSize());
    result->context = m_newContext;
    return result;
}
