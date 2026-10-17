#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qtextdocument.h>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextFormat>
#include <QVariant>
#include <QtScript/QRegExp>
#include <qabstracttextdocumentlayout.h>
#include <qanystringview.h>
#include <qbytearray.h>
#include <qcoreevent.h>
#include <qfont.h>
#include <qobject.h>
#include <qpainter.h>
#include <qrect.h>
#include <qsize.h>
#include <qtextcursor.h>
#include <qtextdocument.h>
#include <qtextformat.h>
#include <qtextobject.h>
#include <qurl.h>

#include "qtscriptshell_QTextDocument.h"

static const char * const qtscript_QTextDocument_function_names[] = {
    "QTextDocument"
    // static
    // prototype
    , "addResource"
    , "adjustSize"
    , "allFormats"
    , "availableRedoSteps"
    , "availableUndoSteps"
    , "baseUrl"
    , "baseUrlChanged"
    , "baselineOffset"
    , "begin"
    , "blockCount"
    , "blockCountChanged"
    , "characterAt"
    , "characterCount"
    , "clear"
    , "clearUndoRedoStacks"
    , "clone"
    , "contentsChange"
    , "contentsChanged"
    , "cursorPositionChanged"
    , "defaultCursorMoveStyle"
    , "defaultFont"
    , "defaultStyleSheet"
    , "documentLayout"
    , "documentLayoutChanged"
    , "documentMargin"
    , "drawContents"
    , "end"
    , "find"
    , "findBlock"
    , "findBlockByLineNumber"
    , "findBlockByNumber"
    , "firstBlock"
    , "frameAt"
    , "idealWidth"
    , "indentWidth"
    , "isEmpty"
    , "isLayoutEnabled"
    , "isModified"
    , "isRedoAvailable"
    , "isUndoAvailable"
    , "isUndoRedoEnabled"
    , "lastBlock"
    , "lineCount"
    , "markContentsDirty"
    , "maximumBlockCount"
    , "metaInformation"
    , "modificationChanged"
    , "object"
    , "objectForFormat"
    , "pageCount"
    , "pageSize"
    , "redo"
    , "redoAvailable"
    , "resource"
    , "revision"
    , "rootFrame"
    , "setBaseUrl"
    , "setBaselineOffset"
    , "setDefaultCursorMoveStyle"
    , "setDefaultFont"
    , "setDefaultStyleSheet"
    , "setDocumentLayout"
    , "setDocumentMargin"
    , "setHtml"
    , "setIndentWidth"
    , "setLayoutEnabled"
    , "setMaximumBlockCount"
    , "setMetaInformation"
    , "setModified"
    , "setPageSize"
    , "setPlainText"
    , "setSubScriptBaseline"
    , "setSuperScriptBaseline"
    , "setTextWidth"
    , "setUndoRedoEnabled"
    , "setUseDesignMetrics"
    , "size"
    , "subScriptBaseline"
    , "superScriptBaseline"
    , "textWidth"
    , "toHtml"
    , "toPlainText"
    , "toRawText"
    , "undo"
    , "undoAvailable"
    , "undoCommandAdded"
    , "useDesignMetrics"
    , "toString"
};

static const char * const qtscript_QTextDocument_function_signatures[] = {
    "QObject parent\nString text, QObject parent"
    // static
    // prototype
    , "int type, QUrl name, Object resource"
    , ""
    , ""
    , ""
    , ""
    , ""
    , "QUrl url"
    , ""
    , ""
    , ""
    , "int newBlockCount"
    , "int pos"
    , ""
    , ""
    , "Stacks historyToClear"
    , "QObject parent"
    , "int from, int charsRemoved, int charsAdded"
    , ""
    , "QTextCursor cursor"
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "QPainter painter, QRectF rect"
    , ""
    , "QRegularExpression expr, QTextCursor cursor, FindFlags options\nQRegularExpression expr, int from, FindFlags options\nString subString, QTextCursor cursor, FindFlags options\nString subString, int from, FindFlags options"
    , "int pos"
    , "int blockNumber"
    , "int blockNumber"
    , ""
    , "int pos"
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "int from, int length"
    , ""
    , "MetaInformation info"
    , "bool m"
    , "int objectIndex"
    , "QTextFormat arg__1"
    , ""
    , ""
    , "\nQTextCursor cursor"
    , "bool arg__1"
    , "int type, QUrl name"
    , ""
    , ""
    , "QUrl url"
    , "qreal baseline"
    , "CursorMoveStyle style"
    , "QFont font"
    , "String sheet"
    , "QAbstractTextDocumentLayout layout"
    , "qreal margin"
    , "String html"
    , "qreal width"
    , "bool b"
    , "int maximum"
    , "MetaInformation info, String arg__2"
    , "bool m"
    , "QSizeF size"
    , "String text"
    , "qreal baseline"
    , "qreal baseline"
    , "qreal width"
    , "bool enable"
    , "bool b"
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "\nQTextCursor cursor"
    , "bool arg__1"
    , ""
    , ""
""
};

static const int qtscript_QTextDocument_function_lengths[] = {
    2
    // static
    // prototype
    , 3
    , 0
    , 0
    , 0
    , 0
    , 0
    , 1
    , 0
    , 0
    , 0
    , 1
    , 1
    , 0
    , 0
    , 1
    , 1
    , 3
    , 0
    , 1
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 2
    , 0
    , 3
    , 1
    , 1
    , 1
    , 0
    , 1
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 2
    , 0
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
    , 1
    , 1
    , 2
    , 0
    , 0
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 2
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
    , 0
    , 0
    , 0
};

static QScriptValue qtscript_QTextDocument_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QTextDocument::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QTextDocument*)
Q_DECLARE_METATYPE(QtScriptShell_QTextDocument*)
Q_DECLARE_METATYPE(QTextDocument::Stacks)
Q_DECLARE_METATYPE(QTextDocument::FindFlag)
Q_DECLARE_METATYPE(QFlags<QTextDocument::FindFlag>)
Q_DECLARE_METATYPE(QTextDocument::ResourceType)
Q_DECLARE_METATYPE(QTextDocument::MetaInformation)
Q_DECLARE_METATYPE(QTextFormat)
Q_DECLARE_METATYPE(QList<QTextFormat >)
Q_DECLARE_METATYPE(QTextBlock)
Q_DECLARE_METATYPE(QTextCursor)
Q_DECLARE_METATYPE(Qt::CursorMoveStyle)
Q_DECLARE_METATYPE(QFont)
Q_DECLARE_METATYPE(QAbstractTextDocumentLayout*)
Q_DECLARE_METATYPE(QPainter*)
Q_DECLARE_METATYPE(QTextFrame*)
Q_DECLARE_METATYPE(QTextObject*)
Q_DECLARE_METATYPE(QTextCursor*)

static QScriptValue qtscript_create_enum_class_helper(
    QScriptEngine *engine,
    QScriptEngine::FunctionSignature construct,
    QScriptEngine::FunctionSignature valueOf,
    QScriptEngine::FunctionSignature toString)
{
    QScriptValue proto = engine->newObject();
    proto.setProperty(QString::fromLatin1("valueOf"),
        engine->newFunction(valueOf), QScriptValue::SkipInEnumeration);
    proto.setProperty(QString::fromLatin1("toString"),
        engine->newFunction(toString), QScriptValue::SkipInEnumeration);
    return engine->newFunction(construct, proto, 1);
}

static QScriptValue qtscript_create_flags_class_helper(
    QScriptEngine *engine,
    QScriptEngine::FunctionSignature construct,
    QScriptEngine::FunctionSignature valueOf,
    QScriptEngine::FunctionSignature toString,
    QScriptEngine::FunctionSignature equals)
{
    QScriptValue proto = engine->newObject();
    proto.setProperty(QString::fromLatin1("valueOf"),
        engine->newFunction(valueOf), QScriptValue::SkipInEnumeration);
    proto.setProperty(QString::fromLatin1("toString"),
        engine->newFunction(toString), QScriptValue::SkipInEnumeration);
    proto.setProperty(QString::fromLatin1("equals"),
        engine->newFunction(equals), QScriptValue::SkipInEnumeration);
    return engine->newFunction(construct, proto);
}

//
// QTextDocument::Stacks
//

static const QTextDocument::Stacks qtscript_QTextDocument_Stacks_values[] = {
    QTextDocument::UndoStack
    , QTextDocument::RedoStack
    , QTextDocument::UndoAndRedoStacks
};

static const char * const qtscript_QTextDocument_Stacks_keys[] = {
    "UndoStack"
    , "RedoStack"
    , "UndoAndRedoStacks"
};

static QString qtscript_QTextDocument_Stacks_toStringHelper(QTextDocument::Stacks value)
{
    if ((value >= QTextDocument::UndoStack) && (value <= QTextDocument::UndoAndRedoStacks))
        return qtscript_QTextDocument_Stacks_keys[static_cast<int>(value)-static_cast<int>(QTextDocument::UndoStack)];
    return QString();
}

static QScriptValue qtscript_QTextDocument_Stacks_toScriptValue(QScriptEngine *engine, const QTextDocument::Stacks &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextDocument"));
    return clazz.property(qtscript_QTextDocument_Stacks_toStringHelper(value));
}

static void qtscript_QTextDocument_Stacks_fromScriptValue(const QScriptValue &value, QTextDocument::Stacks &out)
{
    out = qvariant_cast<QTextDocument::Stacks>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextDocument_Stacks(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextDocument::Stacks>())
        arg = static_cast<int>(qvariant_cast<QTextDocument::Stacks>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("Stacks(): invalid enum value"));
    if ((arg >= QTextDocument::UndoStack) && (arg <= QTextDocument::UndoAndRedoStacks))
        return qScriptValueFromValue(engine,  static_cast<QTextDocument::Stacks>(arg));
    return context->throwError(QString::fromLatin1("Stacks(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextDocument_Stacks_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::Stacks value = qscriptvalue_cast<QTextDocument::Stacks>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextDocument_Stacks_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::Stacks value = qscriptvalue_cast<QTextDocument::Stacks>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextDocument_Stacks_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextDocument_Stacks_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextDocument_Stacks,
        qtscript_QTextDocument_Stacks_valueOf, qtscript_QTextDocument_Stacks_toString);
    qScriptRegisterMetaType<QTextDocument::Stacks>(engine, qtscript_QTextDocument_Stacks_toScriptValue,
        qtscript_QTextDocument_Stacks_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextDocument_Stacks_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextDocument_Stacks_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextDocument_Stacks_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextDocument::FindFlag
//

static const QTextDocument::FindFlag qtscript_QTextDocument_FindFlag_values[] = {
    QTextDocument::FindBackward
    , QTextDocument::FindCaseSensitively
    , QTextDocument::FindWholeWords
};

static const char * const qtscript_QTextDocument_FindFlag_keys[] = {
    "FindBackward"
    , "FindCaseSensitively"
    , "FindWholeWords"
};

static QString qtscript_QTextDocument_FindFlag_toStringHelper(QTextDocument::FindFlag value)
{
    for (int i = 0; i < 3; ++i) {
        if (qtscript_QTextDocument_FindFlag_values[i] == value)
            return QString::fromLatin1(qtscript_QTextDocument_FindFlag_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_QTextDocument_FindFlag_toScriptValue(QScriptEngine *engine, const QTextDocument::FindFlag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextDocument"));
    return clazz.property(qtscript_QTextDocument_FindFlag_toStringHelper(value));
}

static void qtscript_QTextDocument_FindFlag_fromScriptValue(const QScriptValue &value, QTextDocument::FindFlag &out)
{
    out = qvariant_cast<QTextDocument::FindFlag>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextDocument_FindFlag(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextDocument::FindFlag>())
        arg = static_cast<int>(qvariant_cast<QTextDocument::FindFlag>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("FindFlag(): invalid enum value"));
    for (int i = 0; i < 3; ++i) {
        if (qtscript_QTextDocument_FindFlag_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<QTextDocument::FindFlag>(arg));
    }
    return context->throwError(QString::fromLatin1("FindFlag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextDocument_FindFlag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::FindFlag value = qscriptvalue_cast<QTextDocument::FindFlag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextDocument_FindFlag_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::FindFlag value = qscriptvalue_cast<QTextDocument::FindFlag>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextDocument_FindFlag_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextDocument_FindFlag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextDocument_FindFlag,
        qtscript_QTextDocument_FindFlag_valueOf, qtscript_QTextDocument_FindFlag_toString);
    qScriptRegisterMetaType<QTextDocument::FindFlag>(engine, qtscript_QTextDocument_FindFlag_toScriptValue,
        qtscript_QTextDocument_FindFlag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextDocument_FindFlag_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextDocument_FindFlag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextDocument_FindFlag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextDocument::FindFlags
//

static QScriptValue qtscript_QTextDocument_FindFlags_toScriptValue(QScriptEngine *engine, const QTextDocument::FindFlags &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_QTextDocument_FindFlags_fromScriptValue(const QScriptValue &value, QTextDocument::FindFlags &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<QTextDocument::FindFlags>())
        out = qvariant_cast<QTextDocument::FindFlags>(var);
    else if (var.userType() == qMetaTypeId<QTextDocument::FindFlag>())
        out = qvariant_cast<QTextDocument::FindFlag>(var);
    else if (value.isNumber())
        out = static_cast<QTextDocument::FindFlags>(value.toInt32());
    else
        out = {};
}

static QScriptValue qtscript_construct_QTextDocument_FindFlags(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::FindFlags result{};
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<QTextDocument::FindFlags>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<QTextDocument::FindFlag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("FindFlags(): argument %0 is not of type FindFlag").arg(i));
            }
            result |= qvariant_cast<QTextDocument::FindFlag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_QTextDocument_FindFlags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::FindFlags value = qscriptvalue_cast<QTextDocument::FindFlags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextDocument_FindFlags_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::FindFlags value = qscriptvalue_cast<QTextDocument::FindFlags>(context->thisObject());
    QString result;
    for (int i = 0; i < 3; ++i) {
        if ((value & qtscript_QTextDocument_FindFlag_values[i]) == qtscript_QTextDocument_FindFlag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_QTextDocument_FindFlag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_QTextDocument_FindFlags_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<QTextDocument::FindFlags>() == otherObj.value<QTextDocument::FindFlags>())));
}

static QScriptValue qtscript_create_QTextDocument_FindFlags_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_QTextDocument_FindFlags, qtscript_QTextDocument_FindFlags_valueOf,
        qtscript_QTextDocument_FindFlags_toString, qtscript_QTextDocument_FindFlags_equals);
    qScriptRegisterMetaType<QTextDocument::FindFlags>(engine, qtscript_QTextDocument_FindFlags_toScriptValue,
        qtscript_QTextDocument_FindFlags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}

//
// QTextDocument::ResourceType
//

static const QTextDocument::ResourceType qtscript_QTextDocument_ResourceType_values[] = {
    QTextDocument::UnknownResource
    , QTextDocument::HtmlResource
    , QTextDocument::ImageResource
    , QTextDocument::StyleSheetResource
    , QTextDocument::MarkdownResource
    , QTextDocument::UserResource
};

static const char * const qtscript_QTextDocument_ResourceType_keys[] = {
    "UnknownResource"
    , "HtmlResource"
    , "ImageResource"
    , "StyleSheetResource"
    , "MarkdownResource"
    , "UserResource"
};

static QString qtscript_QTextDocument_ResourceType_toStringHelper(QTextDocument::ResourceType value)
{
    for (int i = 0; i < 6; ++i) {
        if (qtscript_QTextDocument_ResourceType_values[i] == value)
            return QString::fromLatin1(qtscript_QTextDocument_ResourceType_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_QTextDocument_ResourceType_toScriptValue(QScriptEngine *engine, const QTextDocument::ResourceType &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextDocument"));
    return clazz.property(qtscript_QTextDocument_ResourceType_toStringHelper(value));
}

static void qtscript_QTextDocument_ResourceType_fromScriptValue(const QScriptValue &value, QTextDocument::ResourceType &out)
{
    out = qvariant_cast<QTextDocument::ResourceType>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextDocument_ResourceType(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextDocument::ResourceType>())
        arg = static_cast<int>(qvariant_cast<QTextDocument::ResourceType>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("ResourceType(): invalid enum value"));
    for (int i = 0; i < 6; ++i) {
        if (qtscript_QTextDocument_ResourceType_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<QTextDocument::ResourceType>(arg));
    }
    return context->throwError(QString::fromLatin1("ResourceType(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextDocument_ResourceType_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::ResourceType value = qscriptvalue_cast<QTextDocument::ResourceType>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextDocument_ResourceType_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::ResourceType value = qscriptvalue_cast<QTextDocument::ResourceType>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextDocument_ResourceType_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextDocument_ResourceType_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextDocument_ResourceType,
        qtscript_QTextDocument_ResourceType_valueOf, qtscript_QTextDocument_ResourceType_toString);
    qScriptRegisterMetaType<QTextDocument::ResourceType>(engine, qtscript_QTextDocument_ResourceType_toScriptValue,
        qtscript_QTextDocument_ResourceType_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 6; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextDocument_ResourceType_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextDocument_ResourceType_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextDocument_ResourceType_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextDocument::MetaInformation
//

static const QTextDocument::MetaInformation qtscript_QTextDocument_MetaInformation_values[] = {
    QTextDocument::DocumentTitle
    , QTextDocument::DocumentUrl
    , QTextDocument::CssMedia
    , QTextDocument::FrontMatter
};

static const char * const qtscript_QTextDocument_MetaInformation_keys[] = {
    "DocumentTitle"
    , "DocumentUrl"
    , "CssMedia"
    , "FrontMatter"
};

static QString qtscript_QTextDocument_MetaInformation_toStringHelper(QTextDocument::MetaInformation value)
{
    if ((value >= QTextDocument::DocumentTitle) && (value <= QTextDocument::FrontMatter))
        return qtscript_QTextDocument_MetaInformation_keys[static_cast<int>(value)-static_cast<int>(QTextDocument::DocumentTitle)];
    return QString();
}

static QScriptValue qtscript_QTextDocument_MetaInformation_toScriptValue(QScriptEngine *engine, const QTextDocument::MetaInformation &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextDocument"));
    return clazz.property(qtscript_QTextDocument_MetaInformation_toStringHelper(value));
}

static void qtscript_QTextDocument_MetaInformation_fromScriptValue(const QScriptValue &value, QTextDocument::MetaInformation &out)
{
    out = qvariant_cast<QTextDocument::MetaInformation>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextDocument_MetaInformation(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextDocument::MetaInformation>())
        arg = static_cast<int>(qvariant_cast<QTextDocument::MetaInformation>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("MetaInformation(): invalid enum value"));
    if ((arg >= QTextDocument::DocumentTitle) && (arg <= QTextDocument::FrontMatter))
        return qScriptValueFromValue(engine,  static_cast<QTextDocument::MetaInformation>(arg));
    return context->throwError(QString::fromLatin1("MetaInformation(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextDocument_MetaInformation_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::MetaInformation value = qscriptvalue_cast<QTextDocument::MetaInformation>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextDocument_MetaInformation_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextDocument::MetaInformation value = qscriptvalue_cast<QTextDocument::MetaInformation>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextDocument_MetaInformation_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextDocument_MetaInformation_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextDocument_MetaInformation,
        qtscript_QTextDocument_MetaInformation_valueOf, qtscript_QTextDocument_MetaInformation_toString);
    qScriptRegisterMetaType<QTextDocument::MetaInformation>(engine, qtscript_QTextDocument_MetaInformation_toScriptValue,
        qtscript_QTextDocument_MetaInformation_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextDocument_MetaInformation_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextDocument_MetaInformation_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextDocument_MetaInformation_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextDocument
//

static QScriptValue qtscript_QTextDocument_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 87;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QTextDocument* _q_self = qscriptvalue_cast<QTextDocument*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QTextDocument.%0(): this object is not a QTextDocument")
            .arg(qtscript_QTextDocument_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 3) {
        int _q_arg0 = context->argument(0).toInt32();
        QUrl _q_arg1 = qscriptvalue_cast<QUrl>(context->argument(1));
        QVariant _q_arg2 = context->argument(2).toVariant();
        _q_self->addResource(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        _q_self->adjustSize();
        return context->engine()->undefinedValue();
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        QList<QTextFormat > _q_result = _q_self->allFormats();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->availableRedoSteps();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->availableUndoSteps();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        QUrl _q_result = _q_self->baseUrl();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 1) {
        QUrl _q_arg0 = qscriptvalue_cast<QUrl>(context->argument(0));
        _q_self->baseUrlChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->baselineOffset();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 8:
    if (context->argumentCount() == 0) {
        QTextBlock _q_result = _q_self->begin();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->blockCount();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->blockCountChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 11:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QChar _q_result = _q_self->characterAt(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->characterCount();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 13:
    if (context->argumentCount() == 0) {
        _q_self->clear();
        return context->engine()->undefinedValue();
    }
    break;

    case 14:
    if (context->argumentCount() == 0) {
        _q_self->clearUndoRedoStacks();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        QTextDocument::Stacks _q_arg0 = qscriptvalue_cast<QTextDocument::Stacks>(context->argument(0));
        _q_self->clearUndoRedoStacks(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 15:
    if (context->argumentCount() == 0) {
        QTextDocument* _q_result = _q_self->clone();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QObject* _q_arg0 = context->argument(0).toQObject();
        QTextDocument* _q_result = _q_self->clone(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 16:
    if (context->argumentCount() == 3) {
        int _q_arg0 = context->argument(0).toInt32();
        int _q_arg1 = context->argument(1).toInt32();
        int _q_arg2 = context->argument(2).toInt32();
        _q_self->contentsChange(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    break;

    case 17:
    if (context->argumentCount() == 0) {
        _q_self->contentsChanged();
        return context->engine()->undefinedValue();
    }
    break;

    case 18:
    if (context->argumentCount() == 1) {
        QTextCursor _q_arg0 = qscriptvalue_cast<QTextCursor>(context->argument(0));
        _q_self->cursorPositionChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 19:
    if (context->argumentCount() == 0) {
        Qt::CursorMoveStyle _q_result = _q_self->defaultCursorMoveStyle();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 20:
    if (context->argumentCount() == 0) {
        QFont _q_result = _q_self->defaultFont();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 21:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->defaultStyleSheet();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 22:
    if (context->argumentCount() == 0) {
        QAbstractTextDocumentLayout* _q_result = _q_self->documentLayout();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 23:
    if (context->argumentCount() == 0) {
        _q_self->documentLayoutChanged();
        return context->engine()->undefinedValue();
    }
    break;

    case 24:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->documentMargin();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 25:
    if (context->argumentCount() == 1) {
        QPainter* _q_arg0 = qscriptvalue_cast<QPainter*>(context->argument(0));
        _q_self->drawContents(_q_arg0);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 2) {
        QPainter* _q_arg0 = qscriptvalue_cast<QPainter*>(context->argument(0));
        QRectF _q_arg1 = qscriptvalue_cast<QRectF>(context->argument(1));
        _q_self->drawContents(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 26:
    if (context->argumentCount() == 0) {
        QTextBlock _q_result = _q_self->end();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 27:
    if (context->argumentCount() == 1) {
        if (context->argument(0).isRegExp()) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            QTextCursor _q_result = _q_self->find(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QTextCursor _q_result = _q_self->find(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    if (context->argumentCount() == 2) {
        if (context->argument(0).isRegExp()
            && (qMetaTypeId<QTextCursor>() == context->argument(1).toVariant().userType())) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            QTextCursor _q_arg1 = qscriptvalue_cast<QTextCursor>(context->argument(1));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isRegExp()
            && context->argument(1).isNumber()) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            int _q_arg1 = context->argument(1).toInt32();
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()
            && (qMetaTypeId<QTextCursor>() == context->argument(1).toVariant().userType())) {
            QString _q_arg0 = context->argument(0).toString();
            QTextCursor _q_arg1 = qscriptvalue_cast<QTextCursor>(context->argument(1));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()
            && context->argument(1).isNumber()) {
            QString _q_arg0 = context->argument(0).toString();
            int _q_arg1 = context->argument(1).toInt32();
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    if (context->argumentCount() == 3) {
        if (context->argument(0).isRegExp()
            && (qMetaTypeId<QTextCursor>() == context->argument(1).toVariant().userType())
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(2).toVariant().userType()))) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            QTextCursor _q_arg1 = qscriptvalue_cast<QTextCursor>(context->argument(1));
            QFlags<QTextDocument::FindFlag> _q_arg2 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(2));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1, _q_arg2);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isRegExp()
            && context->argument(1).isNumber()
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(2).toVariant().userType()))) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            int _q_arg1 = context->argument(1).toInt32();
            QFlags<QTextDocument::FindFlag> _q_arg2 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(2));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1, _q_arg2);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()
            && (qMetaTypeId<QTextCursor>() == context->argument(1).toVariant().userType())
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(2).toVariant().userType()))) {
            QString _q_arg0 = context->argument(0).toString();
            QTextCursor _q_arg1 = qscriptvalue_cast<QTextCursor>(context->argument(1));
            QFlags<QTextDocument::FindFlag> _q_arg2 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(2));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1, _q_arg2);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()
            && context->argument(1).isNumber()
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(2).toVariant().userType()))) {
            QString _q_arg0 = context->argument(0).toString();
            int _q_arg1 = context->argument(1).toInt32();
            QFlags<QTextDocument::FindFlag> _q_arg2 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(2));
            QTextCursor _q_result = _q_self->find(_q_arg0, _q_arg1, _q_arg2);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 28:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QTextBlock _q_result = _q_self->findBlock(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 29:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QTextBlock _q_result = _q_self->findBlockByLineNumber(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 30:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QTextBlock _q_result = _q_self->findBlockByNumber(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 31:
    if (context->argumentCount() == 0) {
        QTextBlock _q_result = _q_self->firstBlock();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 32:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QTextFrame* _q_result = _q_self->frameAt(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 33:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->idealWidth();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 34:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->indentWidth();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 35:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isEmpty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 36:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isLayoutEnabled();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 37:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isModified();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 38:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isRedoAvailable();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 39:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isUndoAvailable();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 40:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isUndoRedoEnabled();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 41:
    if (context->argumentCount() == 0) {
        QTextBlock _q_result = _q_self->lastBlock();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 42:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->lineCount();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 43:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        int _q_arg1 = context->argument(1).toInt32();
        _q_self->markContentsDirty(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 44:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->maximumBlockCount();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 45:
    if (context->argumentCount() == 1) {
        QTextDocument::MetaInformation _q_arg0 = qscriptvalue_cast<QTextDocument::MetaInformation>(context->argument(0));
        QString _q_result = _q_self->metaInformation(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 46:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->modificationChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 47:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QTextObject* _q_result = _q_self->object(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 48:
    if (context->argumentCount() == 1) {
        QTextFormat _q_arg0 = qscriptvalue_cast<QTextFormat>(context->argument(0));
        QTextObject* _q_result = _q_self->objectForFormat(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 49:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->pageCount();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 50:
    if (context->argumentCount() == 0) {
        QSizeF _q_result = _q_self->pageSize();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 51:
    if (context->argumentCount() == 0) {
        _q_self->redo();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        QTextCursor* _q_arg0 = qscriptvalue_cast<QTextCursor*>(context->argument(0));
        _q_self->redo(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 52:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->redoAvailable(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 53:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        QUrl _q_arg1 = qscriptvalue_cast<QUrl>(context->argument(1));
        QVariant _q_result = _q_self->resource(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 54:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->revision();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 55:
    if (context->argumentCount() == 0) {
        QTextFrame* _q_result = _q_self->rootFrame();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 56:
    if (context->argumentCount() == 1) {
        QUrl _q_arg0 = qscriptvalue_cast<QUrl>(context->argument(0));
        _q_self->setBaseUrl(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 57:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setBaselineOffset(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 58:
    if (context->argumentCount() == 1) {
        Qt::CursorMoveStyle _q_arg0 = qscriptvalue_cast<Qt::CursorMoveStyle>(context->argument(0));
        _q_self->setDefaultCursorMoveStyle(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 59:
    if (context->argumentCount() == 1) {
        QFont _q_arg0 = qscriptvalue_cast<QFont>(context->argument(0));
        _q_self->setDefaultFont(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 60:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setDefaultStyleSheet(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 61:
    if (context->argumentCount() == 1) {
        QAbstractTextDocumentLayout* _q_arg0 = qscriptvalue_cast<QAbstractTextDocumentLayout*>(context->argument(0));
        _q_self->setDocumentLayout(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 62:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setDocumentMargin(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 63:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setHtml(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 64:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setIndentWidth(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 65:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setLayoutEnabled(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 66:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->setMaximumBlockCount(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 67:
    if (context->argumentCount() == 2) {
        QTextDocument::MetaInformation _q_arg0 = qscriptvalue_cast<QTextDocument::MetaInformation>(context->argument(0));
        QString _q_arg1 = context->argument(1).toString();
        _q_self->setMetaInformation(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 68:
    if (context->argumentCount() == 0) {
        _q_self->setModified();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setModified(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 69:
    if (context->argumentCount() == 1) {
        QSizeF _q_arg0 = qscriptvalue_cast<QSizeF>(context->argument(0));
        _q_self->setPageSize(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 70:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setPlainText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 71:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setSubScriptBaseline(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 72:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setSuperScriptBaseline(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 73:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setTextWidth(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 74:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setUndoRedoEnabled(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 75:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setUseDesignMetrics(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 76:
    if (context->argumentCount() == 0) {
        QSizeF _q_result = _q_self->size();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 77:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->subScriptBaseline();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 78:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->superScriptBaseline();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 79:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->textWidth();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 80:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toHtml();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 81:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toPlainText();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 82:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toRawText();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 83:
    if (context->argumentCount() == 0) {
        _q_self->undo();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        QTextCursor* _q_arg0 = qscriptvalue_cast<QTextCursor*>(context->argument(0));
        _q_self->undo(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 84:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->undoAvailable(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 85:
    if (context->argumentCount() == 0) {
        _q_self->undoCommandAdded();
        return context->engine()->undefinedValue();
    }
    break;

    case 86:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->useDesignMetrics();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 87: {
    QString result = QString::fromLatin1("QTextDocument");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QTextDocument_throw_ambiguity_error_helper(context,
        qtscript_QTextDocument_function_names[_id+1],
        qtscript_QTextDocument_function_signatures[_id+1]);
}

static QScriptValue qtscript_QTextDocument_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QTextDocument(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QTextDocument* _q_cpp_result = new QtScriptShell_QTextDocument();
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextDocument*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if (context->argument(0).isQObject()) {
            QObject* _q_arg0 = context->argument(0).toQObject();
            QtScriptShell_QTextDocument* _q_cpp_result = new QtScriptShell_QTextDocument(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextDocument*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QtScriptShell_QTextDocument* _q_cpp_result = new QtScriptShell_QTextDocument(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextDocument*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        }
    } else if (context->argumentCount() == 2) {
        QString _q_arg0 = context->argument(0).toString();
        QObject* _q_arg1 = context->argument(1).toQObject();
        QtScriptShell_QTextDocument* _q_cpp_result = new QtScriptShell_QTextDocument(_q_arg0, _q_arg1);
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextDocument*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QTextDocument_throw_ambiguity_error_helper(context,
        qtscript_QTextDocument_function_names[_id],
        qtscript_QTextDocument_function_signatures[_id]);
}

static QScriptValue qtscript_QTextDocument_toScriptValue(QScriptEngine *engine, QTextDocument* const &in)
{
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

static void qtscript_QTextDocument_fromScriptValue(const QScriptValue &value, QTextDocument* &out)
{
    out = qobject_cast<QTextDocument*>(value.toQObject());
}

QScriptValue qtscript_create_QTextDocument_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QTextDocument*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QTextDocument*)0));
    proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QObject*>()));
    for (int i = 0; i < 88; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QTextDocument_prototype_call, qtscript_QTextDocument_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QTextDocument_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    qScriptRegisterMetaType<QTextDocument*>(engine, qtscript_QTextDocument_toScriptValue, 
        qtscript_QTextDocument_fromScriptValue, proto);

    QScriptValue ctor = engine->newFunction(qtscript_QTextDocument_static_call, proto, qtscript_QTextDocument_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("Stacks"),
        qtscript_create_QTextDocument_Stacks_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("FindFlag"),
        qtscript_create_QTextDocument_FindFlag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("FindFlags"),
        qtscript_create_QTextDocument_FindFlags_class(engine));
    ctor.setProperty(QString::fromLatin1("ResourceType"),
        qtscript_create_QTextDocument_ResourceType_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("MetaInformation"),
        qtscript_create_QTextDocument_MetaInformation_class(engine, ctor));
    return ctor;
}
