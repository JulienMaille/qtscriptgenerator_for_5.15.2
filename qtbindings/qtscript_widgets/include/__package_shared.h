#pragma once

#include <QtGui/QPainter>
#include <QtGui/QPaintEngine>

// Qt 6: qVariantFromValue() was removed; the generated bindings use it,
// so every module provides the shim here.
#include <QtCore/QVariant>
template <typename T>
inline QVariant qVariantFromValue(const T &value)
{
    return QVariant::fromValue(value);
}
