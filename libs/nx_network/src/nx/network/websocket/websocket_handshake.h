// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <nx/network/http/http_async_client.h>
#include <nx/network/http/http_types.h>
#include <nx/utils/buffer.h>

#include "websocket_common_types.h"

namespace nx::network::websocket {

static constexpr char kWebsocketProtocolName[] = "websocket";

namespace detail {

NX_NETWORK_API nx::Buffer makeAcceptKey(const nx::ConstBufferRefType& requestKey);

} // namespace detail

NX_NETWORK_API Error validateRequest(
    const nx::network::http::Request& request,
    nx::network::http::Response* response,
    bool disableCompression = false);

NX_NETWORK_API Error validateRequest(
    const nx::network::http::Request& request,
    nx::network::http::HttpHeaders* responseHeaders,
    bool disableCompression = false);

NX_NETWORK_API void addClientHeaders(
    nx::network::http::HttpHeaders* headers,
    const nx::Buffer& protocolName,
    CompressionType compressionType);

NX_NETWORK_API void addClientHeaders(
    nx::network::http::AsyncClient* request,
    const nx::Buffer& protocolName,
    CompressionType compressionType);

NX_NETWORK_API void copyClientHeaders(
    nx::network::http::HttpHeaders* dstHeaders,
    const nx::network::http::HttpHeaders* srcHeaders);

NX_NETWORK_API Error validateResponse(
    const nx::network::http::Request& request,
    const nx::network::http::Response& response);

NX_NETWORK_API CompressionType compressionType(const nx::network::http::HttpHeaders& headers);

} // namespace nx::network::websocket
