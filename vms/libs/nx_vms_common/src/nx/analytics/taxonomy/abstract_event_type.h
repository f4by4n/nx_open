// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <vector>

#include <QtCore/QString>

#include <nx/analytics/taxonomy/abstract_attribute.h>
#include <nx/analytics/taxonomy/abstract_scope.h>

namespace nx::analytics::taxonomy {

class NX_VMS_COMMON_API AbstractEventType: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString iconSource READ icon CONSTANT)
    Q_PROPERTY(nx::analytics::taxonomy::AbstractEventType* baseType READ base CONSTANT)
    Q_PROPERTY(std::vector<nx::analytics::taxonomy::AbstractEventType*> derivedTypes
        READ derivedTypes CONSTANT)
    Q_PROPERTY(std::vector<nx::analytics::taxonomy::AbstractAttribute*> attributes
        READ attributes CONSTANT)
    Q_PROPERTY(std::vector<nx::analytics::taxonomy::AbstractAttribute*> supportedAttributes
        READ supportedAttributes CONSTANT)
    Q_PROPERTY(std::vector<nx::analytics::taxonomy::AbstractAttribute*> ownAttributes
        READ ownAttributes CONSTANT)
    Q_PROPERTY(std::vector<nx::analytics::taxonomy::AbstractAttribute*> supportedOwnAttributes
        READ supportedOwnAttributes CONSTANT)
    Q_PROPERTY(bool hasEverBeenSupported READ hasEverBeenSupported CONSTANT)
    Q_PROPERTY(bool isPrivate READ isPrivate CONSTANT)
    Q_PROPERTY(std::vector<AbstractScope*> scopes READ scopes CONSTANT)

public:
    AbstractEventType(QObject* parent = nullptr):
        QObject(parent)
    {
    }

    virtual ~AbstractEventType() {}

    virtual QString id() const = 0;

    virtual QString name() const = 0;

    virtual QString icon() const = 0;

    virtual AbstractEventType* base() const = 0;

    virtual std::vector<AbstractEventType*> derivedTypes() const = 0;

    virtual std::vector<AbstractAttribute*> baseAttributes() const = 0;

    virtual std::vector<AbstractAttribute*> ownAttributes() const = 0;

    virtual std::vector<AbstractAttribute*> attributes() const = 0;

    virtual std::vector<AbstractAttribute*> supportedAttributes() const = 0;

    virtual std::vector<AbstractAttribute*> supportedOwnAttributes() const = 0;

    virtual bool hasEverBeenSupported() const = 0;

    virtual bool isPrivate() const = 0;

    virtual std::vector<AbstractScope*> scopes() const = 0;

    virtual nx::vms::api::analytics::EventTypeDescriptor serialize() const = 0;
};

} // namespace nx::analytics::taxonomy
