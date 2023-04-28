// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtCore/QSet>

#include <nx/fusion/fusion/fusion_fwd.h>
#include <nx/fusion/serialization/json_fwd.h>
#include <nx/utils/uuid.h>

namespace nx::vms::rules {

// TODO: #amalov Select a better place for field types. Consider API library.

/** Common type for UUID selection field. Stores the result of camera/user selection dialog.*/
struct UuidSelection
{
    /** Manually selected ids.*/
    QSet<QnUuid> ids;

    /** Accept/target all flag. */
    bool all = false;

    bool operator==(const UuidSelection&) const = default;
};

#define UuidSelection_Fields (ids)(all)
QN_FUSION_DECLARE_FUNCTIONS(UuidSelection, (json), NX_VMS_RULES_API);

NX_REFLECTION_ENUM_CLASS(LookupCheckType,
    /* Checks whether the entity exists in the source. */
    in,
    /* Checks whether the entity does not exist in the source. */
    out);

NX_REFLECTION_ENUM_CLASS(LookupSource,
    /* Space separated string. Transforms to a lookup list with 'keyword' attribute. */
    keywords,
    /* Lookup list from the system context manager. */
    lookupList);

} // namespace nx::vms::rules
