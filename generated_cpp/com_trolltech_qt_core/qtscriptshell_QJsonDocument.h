#ifndef QTSCRIPTSHELL_QJSONDOCUMENT_H
#define QTSCRIPTSHELL_QJSONDOCUMENT_H

#include <qjsondocument.h>

#include <QtScript/qscriptvalue.h>
#include <__package_shared.h>

class QtScriptShell_QJsonDocument : public QJsonDocument
{
public:
    QtScriptShell_QJsonDocument();
    QtScriptShell_QJsonDocument(const QJsonArray&  array);
    QtScriptShell_QJsonDocument(const QJsonDocument&  other);
    QtScriptShell_QJsonDocument(const QJsonObject&  object);
    ~QtScriptShell_QJsonDocument();


    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QJSONDOCUMENT_H
