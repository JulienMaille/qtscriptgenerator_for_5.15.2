#include "qtscriptshell_QDoubleValidator.h"

#include <QtScript/QScriptEngine>
#include <QVariant>
#include <qanystringview.h>
#include <qbytearray.h>
#include <qcoreevent.h>
#include <qlocale.h>
#include <qobject.h>

#define QTSCRIPT_IS_GENERATED_FUNCTION(fun) ((fun.data().toUInt32() & 0xFFFF0000) == 0xBABE0000)

Q_DECLARE_METATYPE(QChildEvent*)
Q_DECLARE_METATYPE(QEvent*)
Q_DECLARE_METATYPE(const char*)
Q_DECLARE_METATYPE(QTimerEvent*)
Q_DECLARE_METATYPE(QValidator::State)

QtScriptShell_QDoubleValidator::QtScriptShell_QDoubleValidator(QObject*  parent)
    : QDoubleValidator(parent) {}

QtScriptShell_QDoubleValidator::QtScriptShell_QDoubleValidator(double  bottom, double  top, int  decimals, QObject*  parent)
    : QDoubleValidator(bottom, top, decimals, parent) {}

QtScriptShell_QDoubleValidator::~QtScriptShell_QDoubleValidator() {}

void QtScriptShell_QDoubleValidator::childEvent(QChildEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("childEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("childEvent") & QScriptValue::QObjectMember)) {
        QDoubleValidator::childEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

void QtScriptShell_QDoubleValidator::customEvent(QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("customEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("customEvent") & QScriptValue::QObjectMember)) {
        QDoubleValidator::customEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

bool  QtScriptShell_QDoubleValidator::event(QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("event");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("event") & QScriptValue::QObjectMember)) {
        return QDoubleValidator::event(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<bool >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event)));
    }
}

bool  QtScriptShell_QDoubleValidator::eventFilter(QObject*  watched, QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("eventFilter");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("eventFilter") & QScriptValue::QObjectMember)) {
        return QDoubleValidator::eventFilter(watched, event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<bool >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, watched)
            << qScriptValueFromValue(_q_engine, event)));
    }
}

void QtScriptShell_QDoubleValidator::fixup(QString&  input) const
{
    QScriptValue _q_function = __qtscript_self.property("fixup");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("fixup") & QScriptValue::QObjectMember)) {
        QDoubleValidator::fixup(input);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        QScriptValue _q_result =
            _q_function.call(__qtscript_self,
                             QScriptValueList() << qScriptValueFromValue(_q_engine, input));
        if( !_q_engine->hasUncaughtException() && _q_result.isString() ) input = _q_result.toString();
    }
}

void*  QtScriptShell_QDoubleValidator::qt_metacast(const char*  arg__1)
{
    QScriptValue _q_function = __qtscript_self.property("qt_metacast");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("qt_metacast") & QScriptValue::QObjectMember)) {
        return QDoubleValidator::qt_metacast(arg__1);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<void* >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, const_cast<const char *>(arg__1))));
    }
}

void QtScriptShell_QDoubleValidator::timerEvent(QTimerEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("timerEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("timerEvent") & QScriptValue::QObjectMember)) {
        QDoubleValidator::timerEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

QValidator::State  QtScriptShell_QDoubleValidator::validate(QString&  arg__1, int&  arg__2) const
{
    QScriptValue _q_function = __qtscript_self.property("validate");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("validate") & QScriptValue::QObjectMember)) {
        return QDoubleValidator::validate(arg__1, arg__2);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        QScriptValue _q_result =
            _q_function.call(__qtscript_self,
                             QScriptValueList() << qScriptValueFromValue(_q_engine, arg__1)
                                                << qScriptValueFromValue(_q_engine, arg__2));
        if( _q_engine->hasUncaughtException() ) return QValidator::Invalid;
        if( _q_result.isObject() )
        {
            QScriptValue _q_state = _q_result.property("state");
            if( _q_engine->hasUncaughtException() ) return QValidator::Invalid;
            if( _q_state.isValid() )
            {
                QValidator::State _q_value = qscriptvalue_cast<QValidator::State>(_q_state);
                if( _q_engine->hasUncaughtException() ) return QValidator::Invalid;
                QScriptValue _q_arg__1 = _q_result.property("arg__1");
                if( _q_engine->hasUncaughtException() ) return QValidator::Invalid;
                QScriptValue _q_arg__2 = _q_result.property("arg__2");
                if( _q_engine->hasUncaughtException() ) return QValidator::Invalid;
                if( _q_arg__1.isString() ) arg__1 = _q_arg__1.toString();
                if( _q_arg__2.isNumber() ) arg__2 = _q_arg__2.toInt32();
                return _q_value;
            }
        }
        return qscriptvalue_cast<QValidator::State>(_q_result);
    }
}

