#ifndef QTSCRIPTSHELL_QJSONPARSEERROR_H
#define QTSCRIPTSHELL_QJSONPARSEERROR_H

#include <qjsondocument.h>

#include <QtScript/qscriptvalue.h>
#include <__package_shared.h>

class QtScriptShell_QJsonParseError : public QJsonParseError
{
public:
    QtScriptShell_QJsonParseError();
    ~QtScriptShell_QJsonParseError();


    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QJSONPARSEERROR_H
