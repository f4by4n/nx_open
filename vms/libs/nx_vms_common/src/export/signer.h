// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <nx/streaming/media_data_packet.h>
#include <nx/utils/cryptographic_hash.h>

class QnLicensePool;

/*
 * Class helper that build new version of signature that process audio frames separatelly, due to
 * some media containers(#: ismv) can change audio/video packets order.
 */

class NX_VMS_COMMON_API MediaSigner
{
public:
    MediaSigner();
    void processMedia(
        AVCodecParameters* context, const uint8_t* data, int size, QnAbstractMediaData::DataType type);

    QByteArray buildSignature(QnLicensePool* licensePool);
    QByteArray buildSignature(const QByteArray& signPattern);

private:
    nx::utils::QnCryptographicHash m_audioHash;
    nx::utils::QnCryptographicHash m_signatureHash;
};
