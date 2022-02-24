// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <chrono>

#include <QtCore/QObject>

#include <common/common_module_aware.h>
#include <core/resource/resource_fwd.h>

namespace nx::vms::client::desktop {

class TimeSynchronizationWidgetStore;

class TimeSynchronizationServerTimeWatcher:
    public QObject,
    public QnCommonModuleAware
{
    Q_OBJECT
    using base_type = QObject;

public:
    explicit TimeSynchronizationServerTimeWatcher(TimeSynchronizationWidgetStore* store,
        QObject* parent = nullptr);
    virtual ~TimeSynchronizationServerTimeWatcher() override;

    void start();
    void stop();

    void forceUpdate();

    std::chrono::milliseconds elapsedTime() const;

private:
    class Private;
    const QScopedPointer<Private> d;
};

} // namespace nx::vms::client::desktop
