// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include "../event_fields/source_camera_field.h"
#include "../utils/type.h"
#include "motion_event.h"

#include <nx/utils/metatypes.h>

#include "../utils/event_details.h"
#include "../utils/string_helper.h"

namespace nx::vms::rules {

MotionEvent::MotionEvent(std::chrono::microseconds timestamp, State state, QnUuid deviceId):
    base_type(timestamp, state, deviceId)
{
}

QVariantMap MotionEvent::details(common::SystemContext* context) const
{
    auto result = BasicEvent::details(context);

    utils::insertIfNotEmpty(result, utils::kExtendedCaptionDetailName, extendedCaption(context));
    result.insert(utils::kEmailTemplatePathDetailName, manifest().emailTemplatePath);

    return result;
}

QString MotionEvent::extendedCaption(common::SystemContext* context) const
{
    if (totalEventCount() == 1)
    {
        const auto resourceName = utils::StringHelper(context).resource(source(), Qn::RI_WithUrl);
        return tr("Motion on %1").arg(resourceName);
    }

    return BasicEvent::extendedCaption();
}

const ItemDescriptor& MotionEvent::manifest()
{
    static const auto kDescriptor = ItemDescriptor{
        .id = "nx.events.motion",
        .displayName = tr("Motion Event"),
        .description = "",
        .flags = ItemFlag::prolonged,
        .fields = {
            makeFieldDescriptor<SourceCameraField>("source", tr("Camera")),
        },
        .emailTemplatePath = ":/email_templates/camera_motion.mustache"
    };
    return kDescriptor;
}

} // namespace nx::vms::rules
