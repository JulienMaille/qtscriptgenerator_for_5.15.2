#include "qtscriptshell_QIntValidator.h"

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

QtScriptShell_QIntValidator::QtScriptShell_QIntValidator(QObject*  parent)
    : QIntValidator(parent) {}

QtScriptShell_QIntValidator::QtScriptShell_QIntValidator(int  bottom, int  top, QObject*  parent)
    : QIntValidator(bottom, top, parent) {}

QtScriptShell_QIntValidator::~QtScriptShell_QIntValidator() {}

void QtScriptShell_QIntValidator::childEvent(QChildEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("childEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("childEvent") & QScriptValue::QObjectMember)) {
        QIntValidator::childEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

void QtScriptShell_QIntValidator::customEvent(QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("customEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("customEvent") & QScriptValue::QObjectMember)) {
        QIntValidator::customEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

bool  QtScriptShell_QIntValidator::event(QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("event");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("event") & QScriptValue::QObjectMember)) {
        return QIntValidator::event(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<bool >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event)));
    }
}

bool  QtScriptShell_QIntValidator::eventFilter(QObject*  watched, QEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("eventFilter");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("eventFilter") & QScriptValue::QObjectMember)) {
        return QIntValidator::eventFilter(watched, event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<bool >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, watched)
            << qScriptValueFromValue(_q_engine, event)));
    }
}

void QtScriptShell_QIntValidator::fixup(QString&  input) const
{
    QScriptValue _q_function = __qtscript_self.property("fixup");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("fixup") & QScriptValue::QObjectMember)) {
        QIntValidator::fixup(input);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        QScriptValue _q_result =
            _q_function.call(__qtscript_self,
                             QScriptValueList() << qScriptValueFromValue(_q_engine, input));
        if( !_q_engine->hasUncaughtException() && _q_result.isString() ) input = _q_result.toString();
    }
}

void*  QtScriptShell_QIntValidator::qt_metacast(const char*  arg__1)
{
    QScriptValue _q_function = __qtscript_self.property("qt_metacast");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("qt_metacast") & QScriptValue::QObjectMember)) {
        return QIntValidator::qt_metacast(arg__1);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        return qscriptvalue_cast<void* >(_q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, const_cast<const char *>(arg__1))));
    }
}

void QtScriptShell_QIntValidator::timerEvent(QTimerEvent*  event)
{
    QScriptValue _q_function = __qtscript_self.property("timerEvent");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("timerEvent") & QScriptValue::QObjectMember)) {
        QIntValidator::timerEvent(event);
    } else {
        QScriptEngine *_q_engine = __qtscript_self.engine();
        _q_function.call(__qtscript_self,
            QScriptValueList()
            << qScriptValueFromValue(_q_engine, event));
    }
}

QValidator::State  QtScriptShell_QIntValidator::validate(QString&  arg__1, int&  arg__2) const
{
    QScriptValue _q_function = __qtscript_self.property("validate");
    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)
        || (__qtscript_self.propertyFlags("validate") & QScriptValue::QObjectMember)) {
        return QIntValidator::validate(arg__1, arg__2);
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

