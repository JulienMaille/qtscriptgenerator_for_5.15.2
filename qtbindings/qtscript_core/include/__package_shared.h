
#pragma once

#include <qcoreevent.h>

#include <QtScript/qscriptvalue.h>
#include <QtScript/QScriptEngine>


#include <QFile>


Q_DECLARE_METATYPE(QEvent)

// Qt 6: qVariantFromValue() was removed; the generated bindings use it, so
// provide the shim here (same as qtscriptgenerator-qt6's shared header).
template <typename T>
inline QVariant qVariantFromValue(const T &value)
{
    return QVariant::fromValue(value);
}

// Qt 6: QEvent is neither copyable nor movable, so there is deliberately NO
// qscriptvalue_cast<QEvent> by-value overload (its `return t` would reference
// the deleted move ctor). QEvent travels by pointer in the Qt6 bindings;
// pointer casts use the default template and need no specialization here.


