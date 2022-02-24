// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <memory>

#include <QtCore/QObject>

#include <nx/core/access/access_types.h>
#include <nx/fusion/serialization_format.h>
#include <nx/utils/impl_ptr.h>
#include <nx/utils/singleton.h>
#include <nx/vms/client/core/network/server_certificate_validation_level.h>

class QQmlEngine;

class QnCommonModule;
class QnPtzControllerPool;
class QnLayoutTourStateManager;
class QnDataProviderFactory;
class QnCloudStatusWatcher;

namespace nx::vms::api { enum class PeerType; }

namespace nx::vms::client::core { class NetworkModule; }

class QnClientCoreModule: public QObject, public Singleton<QnClientCoreModule>
{
    Q_OBJECT
    using base_type = QObject;

public:
    enum class Mode
    {
        desktopClient,
        mobileClient,
        unitTests
    };

    QnClientCoreModule(
        Mode mode,
        nx::core::access::Mode resourceAccessMode = nx::core::access::Mode::cached);
    virtual ~QnClientCoreModule() override;

    using CertificateValidationLevel =
        nx::vms::client::core::network::server_certificate::ValidationLevel;
    void initializeNetworking(
        nx::vms::api::PeerType peerType,
        Qn::SerializationFormat serializationFormat,
        CertificateValidationLevel certificateValidationLevel);

    nx::vms::client::core::NetworkModule* networkModule() const;

    QnCommonModule* commonModule() const;
    QnPtzControllerPool* ptzControllerPool() const;
    QnLayoutTourStateManager* layoutTourStateManager() const;
    QnDataProviderFactory* dataProviderFactory() const;

    QQmlEngine* mainQmlEngine();

    QnCloudStatusWatcher* cloudStatusWatcher() const;

private:
    struct Private;
    nx::utils::ImplPtr<Private> d;
};

#define qnClientCoreModule QnClientCoreModule::instance()
