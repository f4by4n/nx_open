// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtCore/QObject>

#include <common/common_globals.h>
#include <recording/time_period_list.h>

class QnTimePeriodList;

namespace nx::vms::client::core {

class TimePeriodsStore: public QObject
{
    Q_OBJECT
    using base_type = QObject;

public:
    static void registerQmlType();

    TimePeriodsStore(QObject* parent = nullptr);

    bool hasPeriods(Qn::TimePeriodContent type) const;
    void setPeriods(Qn::TimePeriodContent type, const QnTimePeriodList& periods);
    QnTimePeriodList periods(Qn::TimePeriodContent type) const;

signals:
    void periodsUpdated(Qn::TimePeriodContent type);

private:
    using Periods = QHash<Qn::TimePeriodContent, QnTimePeriodList>;
    Periods m_periods;
};

} // namespace nx::vms::client::core
