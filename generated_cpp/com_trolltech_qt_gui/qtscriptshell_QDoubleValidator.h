#ifndef QTSCRIPTSHELL_QDOUBLEVALIDATOR_H
#define QTSCRIPTSHELL_QDOUBLEVALIDATOR_H

#include <qvalidator.h>

#include <QtScript/qscriptvalue.h>
#include <__package_shared.h>

class QtScriptShell_QDoubleValidator : public QDoubleValidator
{
public:
    QtScriptShell_QDoubleValidator(QObject*  parent = nullptr);
    QtScriptShell_QDoubleValidator(double  bottom, double  top, int  decimals, QObject*  parent = nullptr);
    ~QtScriptShell_QDoubleValidator();

    void childEvent(QChildEvent*  event);
    void customEvent(QEvent*  event);
    bool  event(QEvent*  event);
    bool  eventFilter(QObject*  watched, QEvent*  event);
    void fixup(QString&  input) const;
    void*  qt_metacast(const char*  arg__1);
    void timerEvent(QTimerEvent*  event);
    QValidator::State  validate(QString&  arg__1, int&  arg__2) const;

    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QDOUBLEVALIDATOR_H
