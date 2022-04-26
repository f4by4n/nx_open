// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <type_traits>

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

namespace nx::utils {

/** Describes access to the properties of the QObject based classes. */
enum class PropertyAccess
{
    /** Property must exists but not necessary must be readable or writable. */
    anyAccess = 0x0,

    /** Property must exists and must be readable at least. */
    readable = 0x1,

    /** Property must exists and must be writable at least. */
    writable = 0x2,

    /** Property must exists and must be readable and writable. */
    fullAccess = readable | writable
};
Q_DECLARE_FLAGS(PropertyAccessFlags, PropertyAccess)

/**
 * Returns list of the object property and dynamic property names or empty list if the object is
 * nullptr. By default QObject properties are excluded, set includeBaseProperties to true to include.
 */
template<class Base = QObject>
inline QStringList propertyNames(
    QObject* object,
    const PropertyAccessFlags& propertyAccessFlags = PropertyAccess::anyAccess,
    bool includeBaseProperties = false)
{
    static_assert(std::is_base_of<QObject, Base>());

    if (!object )
        return {};

    QStringList result;

    const int offset = includeBaseProperties
        ? Base::staticMetaObject.propertyOffset()
        : Base::staticMetaObject.propertyOffset() + Base::staticMetaObject.propertyCount();

    auto metaObject = object->metaObject();
    for (int i = offset; i < metaObject->propertyCount(); ++i)
    {
        auto property = metaObject->property(i);

        if (propertyAccessFlags.testFlag(PropertyAccess::readable) && !property.isReadable())
            continue;

        if (propertyAccessFlags.testFlag(PropertyAccess::writable) && !property.isWritable())
            continue;

        result << metaObject->property(i).name();
    }

    for (const auto& propertyName: object->dynamicPropertyNames())
        result << propertyName;

    return result;
}

} // namespace nx::utils
