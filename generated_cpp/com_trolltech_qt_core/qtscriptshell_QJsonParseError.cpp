#include "qtscriptshell_QJsonParseError.h"

#include <QtScript/QScriptEngine>
#include <QVariant>

#define QTSCRIPT_IS_GENERATED_FUNCTION(fun) ((fun.data().toUInt32() & 0xFFFF0000) == 0xBABE0000)


QtScriptShell_QJsonParseError::QtScriptShell_QJsonParseError()
    : QJsonParseError() {}

QtScriptShell_QJsonParseError::~QtScriptShell_QJsonParseError() {}

