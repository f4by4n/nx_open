// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include "system_helpers.h"

#include <QtCore/QCoreApplication>

#include <api/global_settings.h>
#include <common/common_module.h>
#include <core/resource/media_server_resource.h>
#include <network/cloud_system_data.h>
#include <nx/utils/uuid.h>
#include <nx/vms/api/data/module_information.h>
#include <utils/common/id.h>

namespace {

struct SystemHelpers
{
    Q_DECLARE_TR_FUNCTIONS(SystemHelpers)
};

QnUuid getFactorySystemIdImpl(const QnUuid& serverId)
{
    return serverId;
}

QString generateTargetId(const QString& cloudSystemId,
    const QnUuid& localSystemId)
{
    return (cloudSystemId.isEmpty() ? localSystemId.toString() : cloudSystemId);
}

QnUuid getLocalSystemIdImpl(const QnUuid& localSystemId, const QnUuid& serverId)
{
    if (localSystemId.isNull())
        return getFactorySystemIdImpl(serverId);  //< New System id

    return localSystemId;
}

QString getTargetSystemIdImpl(
    const QString& cloudSystemId,
    const QnUuid& localSystemId,
    const QnUuid& serverId)
{
    const auto fixedLocalId = getLocalSystemIdImpl(
        localSystemId, serverId);
    return generateTargetId(cloudSystemId, fixedLocalId);
}

} // namespace

namespace helpers {

QString getTargetSystemId(const nx::vms::api::ModuleInformation& info)
{
    return ::getTargetSystemIdImpl(info.cloudSystemId, info.localSystemId, info.id);
}

QString getTargetSystemId(const QnCloudSystem& cloudSystem)
{
    return generateTargetId(cloudSystem.cloudId, cloudSystem.localId);
}

QnUuid getLocalSystemId(const nx::vms::api::ModuleInformation& info)
{
    return getLocalSystemIdImpl(info.localSystemId, info.id);
}

bool isNewSystem(const nx::vms::api::ModuleInformation& info)
{
    return info.localSystemId.isNull();
}

bool isNewSystem(const QnCloudSystem& info)
{
    return info.localId.isNull();
}

bool isCloudSystem(const nx::vms::api::ModuleInformation& info)
{
    return !info.cloudSystemId.isEmpty();
}

QString getSystemName(const nx::vms::api::ModuleInformation& info)
{
    return helpers::isNewSystem(info)
        ? SystemHelpers::tr("New System")
        : info.systemName;
}

QnUuid currentSystemLocalId(const QnCommonModule* commonModule)
{
    if (!commonModule)
        return QnUuid();

    const auto& settings = commonModule->globalSettings();
    const auto localId = settings->localSystemId();
    return (localId.isNull() ? commonModule->remoteGUID() : localId);
}

bool serverBelongsToCurrentSystem(
    const nx::vms::api::ModuleInformation& info, const QnCommonModule* commonModule)
{
    return !isNewSystem(info)
        && (getLocalSystemId(info) == currentSystemLocalId(commonModule));
}

bool serverBelongsToCurrentSystem(const QnMediaServerResourcePtr& server)
{
    return serverBelongsToCurrentSystem(server->getModuleInformation(), server->commonModule());
}

bool currentSystemIsNew(const QnCommonModule* commonModule)
{
    const auto& settings = commonModule->globalSettings();
    return settings->localSystemId().isNull();
}

} // namespace helpers
