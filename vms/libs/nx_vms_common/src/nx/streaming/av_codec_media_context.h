// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

extern "C" {
#include <libavformat/avformat.h>
} // extern "C"

#include <nx/streaming/media_context_serializable_data.h>

/**
 * Class that wraps (and owns) AVCodecParameters
 */
class NX_VMS_COMMON_API CodecParameters
{
public:
    virtual ~CodecParameters();

    // Deserialize from ubjson format.
    bool deserialize(const QByteArray& data);

    // Serialize to ubjson format.
    QByteArray serialize() const;

    CodecParameters();

    /**
     * Copy the specified AVCodecContext into the internal one.
     */
    CodecParameters(const AVCodecContext* codecContext);

    /**
     * Copy the specified AVCodecParameters into the internal ones.
     */
    CodecParameters(const AVCodecParameters* avCodecParams);

    /**
     * @return Pointer to the internal AVCodecContext which can be altered.
     */
    AVCodecParameters* getAvCodecParameters() const { return m_codecParams; }

    void toAvCodecContext(AVCodecContext* context) const;

    /**
     * Replace existing extradata with the copy of the provided one.
     * @param extradata Can be null, in which case extradata_size is ignored.
     * @param extradata_size Can be 0.
     */
    void setExtradata(const uint8_t* data, int size);

    // Check equality
    bool isEqual(const CodecParameters& other) const;

    QString toString() const;

    // Can be empty, but never null.
    QString getCodecName() const;

    // Can be empty, but never null.
    QString getAudioCodecDescription() const;

    //--------------------------------------------------------------------------

    AVCodecID getCodecId() const;
    AVMediaType getCodecType() const;
    const quint8* getExtradata() const;
    int getExtradataSize() const;

    int getChannels() const;
    int getSampleRate() const;
    AVSampleFormat getSampleFmt() const;
    int getBitsPerCodedSample() const;
    int getWidth() const;
    int getHeight() const;
    int getBitRate() const;
    quint64 getChannelLayout() const;
    int getBlockAlign() const;
    int getFrameSize() const;

private:
    CodecParameters(const CodecParameters&);
    CodecParameters& operator=(const CodecParameters&);

    AVCodecParameters* m_codecParams;
};

using CodecParametersPtr = std::shared_ptr<CodecParameters>;
using CodecParametersConstPtr = std::shared_ptr<const CodecParameters>;
