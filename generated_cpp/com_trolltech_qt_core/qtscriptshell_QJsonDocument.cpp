#include "qtscriptshell_QJsonDocument.h"

#include <QtScript/QScriptEngine>
#include <QVariant>
#include <qbytearray.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qstringview.h>

#define QTSCRIPT_IS_GENERATED_FUNCTION(fun) ((fun.data().toUInt32() & 0xFFFF0000) == 0xBABE0000)


QtScriptShell_QJsonDocument::QtScriptShell_QJsonDocument()
    : QJsonDocument() {}

QtScriptShell_QJsonDocument::QtScriptShell_QJsonDocument(const QJsonArray&  array)
    : QJsonDocument(array) {}

QtScriptShell_QJsonDocument::QtScriptShell_QJsonDocument(const QJsonDocument&  other)
    : QJsonDocument(other) {}

QtScriptShell_QJsonDocument::QtScriptShell_QJsonDocument(const QJsonObject&  object)
    : QJsonDocument(object) {}

QtScriptShell_QJsonDocument::~QtScriptShell_QJsonDocument() {}

