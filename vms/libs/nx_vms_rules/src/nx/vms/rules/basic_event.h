// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <chrono>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <nx/vms/api/rules/event_info.h>

#include "manifest.h"

namespace nx::vms::rules {

/**
 * Base class for storing data of input events produced by event connectors.
 * Derived classes should provide Q_PROPERTY's for all significant variables
 * (event fields) of their event type.
 */
class NX_VMS_RULES_API BasicEvent: public QObject
{
    Q_OBJECT
    using base_type = QObject;

    Q_PROPERTY(quint64 timestamp MEMBER m_timestamp)
    Q_PROPERTY(QString type READ type)

public:
    using State = nx::vms::api::rules::EventInfo::State;

    BasicEvent(nx::vms::api::rules::EventInfo& info);

    QString type() const;

    /** List of event field names. */
    QStringList fields() const;

protected:
    BasicEvent();

private:
    QString m_type;
    State m_state;

    quint64 m_timestamp;
    QStringList m_source;
};

using EventPtr = QSharedPointer<BasicEvent>;
using EventData = QHash<QString, QVariant>; // TODO: #spanasenko Move to separate class?

using FilterManifest = QMap<QString, QString>; //< Field name to field type mapping.

} // namespace nx::vms::rules

Q_DECLARE_METATYPE(nx::vms::rules::EventPtr)
