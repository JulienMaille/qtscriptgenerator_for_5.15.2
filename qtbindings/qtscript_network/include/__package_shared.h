
#pragma once

#include <qobject.h>

#ifdef QT_NO_SSL
class QSslError : public QObject {
	Q_OBJECT
public:
	QSslError(): QObject() {}
	QSslError(const QSslError &) : QObject() {}
};
class QSslConfiguration : public QObject {
	Q_OBJECT
public:
	QSslConfiguration() : QObject() {}
	QSslConfiguration(const QSslConfiguration &) : QObject() {}
};
#endif


// Qt 6: qVariantFromValue() was removed; the generated bindings use it,
// so every module provides the shim here.
#include <QtCore/QVariant>
template <typename T>
inline QVariant qVariantFromValue(const T &value)
{
    return QVariant::fromValue(value);
}
