// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <nx/cloud/db/api/result_code.h>
#include <nx/network/http/http_types.h>
#include <nx/network/http/server/api_request_result.h>
#include <nx/reflect/enum_instrument.h>

namespace nx::cloud::db::api {

nx::network::http::StatusCode::Value resultCodeToHttpStatusCode(ResultCode resultCode);
ResultCode httpStatusCodeToResultCode(nx::network::http::StatusCode::Value statusCode);

nx::network::http::ApiRequestResult apiResultToFusionRequestResult(Result result);
ResultCode fusionRequestResultToResultCode(nx::network::http::ApiRequestResult result);

NX_REFLECTION_INSTRUMENT_ENUM(ResultCode,
    ok,
    partialContent,
    notAuthorized,
    forbidden,
    accountNotActivated,
    accountBlocked,
    notFound,
    alreadyExists,
    dbError,
    networkError,
    notImplemented,
    unknownRealm,
    badUsername,
    badRequest,
    invalidNonce,
    serviceUnavailable,
    notAllowedInCurrentState,
    mergedSystemIsOffline,
    vmsRequestFailure,
    credentialsRemovedPermanently,
    invalidFormat,
    retryLater,
    unknownError
)

} // namespace nx::cloud::db::api
