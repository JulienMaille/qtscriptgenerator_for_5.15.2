#ifndef QTSCRIPTSHELL_QVALIDATOR_H
#define QTSCRIPTSHELL_QVALIDATOR_H

#include <qvalidator.h>

#include <QtScript/qscriptvalue.h>
#include <__package_shared.h>

class QtScriptShell_QValidator : public QValidator
{
public:
    QtScriptShell_QValidator(QObject*  parent = nullptr);
    ~QtScriptShell_QValidator();

    void childEvent(QChildEvent*  event);
    void customEvent(QEvent*  event);
    bool  event(QEvent*  event);
    bool  eventFilter(QObject*  watched, QEvent*  event);
    void fixup(QString&  arg__1) const;
    void*  qt_metacast(const char*  arg__1);
    void timerEvent(QTimerEvent*  event);
    QValidator::State  validate(QString&  arg__1, int&  arg__2) const;

    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QVALIDATOR_H
