// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtCore/QObject>
#include <QtGui/QRegion>

#include <camera/data/camera_data_fwd.h>
#include <camera/loaders/abstract_camera_data_loader.h>
#include <core/resource/camera_bookmark_fwd.h>
#include <core/resource/resource_fwd.h>
#include <nx/vms/client/core/network/remote_connection_aware.h>
#include <recording/time_period.h>
#include <recording/time_period_list.h>

/**
 * Per-camera data loader that caches loaded data.
 * Uses flat structure. Data is loaded with the most detailed level.
 * Source data period is solid, no spaces are allowed.
 */
class QnFlatCameraDataLoader:
    public QnAbstractCameraDataLoader,
    public nx::vms::client::core::RemoteConnectionAware
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * \param resource                  Network resource representing the camera to work with.
     * \param parent                    Parent object.
     */
    QnFlatCameraDataLoader(const QnVirtualCameraResourcePtr& camera,
        Qn::TimePeriodContent dataType,
        QObject* parent = nullptr);
    virtual ~QnFlatCameraDataLoader();

    virtual int load(const QString &filter = QString(), const qint64 resolutionMs = 1) override;

    virtual void discardCachedData(const qint64 resolutionMs = 0) override;

private:
    void at_timePeriodsReceived(bool success,
        int requestHandle,
        const MultiServerPeriodDataList &timePeriods);

private:
    int sendRequest(qint64 startTimeMs, qint64 resolutionMs);
    void handleDataLoaded(bool success, int requestHandle, const QnAbstractCameraDataPtr &data);

private:
    struct LoadingInfo
    {
        /** Real loading handle, provided by the server connection object. */
        int handle;

        /** Starting time of the request. */
        qint64 startTimeMs;

        /** List of local (fake) handles for requests to this time period loader
         * that are waiting for the same time period to be loaded. */
        QList<int> waitingHandles;

        LoadingInfo();

        void clear();
    };

    QString m_filter;

    LoadingInfo m_loading;

    /** Loaded data. */
    QnAbstractCameraDataPtr m_loadedData;
};
