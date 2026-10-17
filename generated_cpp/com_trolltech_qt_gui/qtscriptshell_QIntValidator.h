#ifndef QTSCRIPTSHELL_QINTVALIDATOR_H
#define QTSCRIPTSHELL_QINTVALIDATOR_H

#include <qvalidator.h>

#include <QtScript/qscriptvalue.h>
#include <__package_shared.h>

class QtScriptShell_QIntValidator : public QIntValidator
{
public:
    QtScriptShell_QIntValidator(QObject*  parent = nullptr);
    QtScriptShell_QIntValidator(int  bottom, int  top, QObject*  parent = nullptr);
    ~QtScriptShell_QIntValidator();

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

#endif // QTSCRIPTSHELL_QINTVALIDATOR_H
