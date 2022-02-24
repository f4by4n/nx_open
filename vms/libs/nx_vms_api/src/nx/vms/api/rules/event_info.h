// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtCore/QJsonValue>
#include <QtCore/QMap>

#include <nx/utils/uuid.h>
#include <nx/vms/api/data/data_macros.h>

namespace nx::vms::api::rules {

struct NX_VMS_API EventInfo
{
    NX_REFLECTION_ENUM_CLASS_IN_CLASS(State,
        none,
        started,
        ended,
        instant
    )

    QnUuid id;
    QString eventType;
    State state = State::none;
    QMap<QString, QString> props;
};

#define nx_vms_api_rules_EventInfo_Fields \
    (id)(eventType)(state)(props)
NX_VMS_API_DECLARE_STRUCT(EventInfo)

QN_FUSION_DECLARE_FUNCTIONS(EventInfo, (json)(ubjson), NX_VMS_API)

} // namespace nx::vms::api::rules
