#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qtextedit.h>
#include <QTextCursor>
#include <QTextEdit>
#include <QVariant>
#include <QtScript/QRegExp>
#include <qaction.h>
#include <qanystringview.h>
#include <qbitmap.h>
#include <qbytearray.h>
#include <qcolor.h>
#include <qcoreevent.h>
#include <qcursor.h>
#include <qevent.h>
#include <qfont.h>
#include <qgraphicseffect.h>
#include <qgraphicsproxywidget.h>
#include <qicon.h>
#include <qkeysequence.h>
#include <qlayout.h>
#include <qlocale.h>
#include <qmargins.h>
#include <qmenu.h>
#include <qmimedata.h>
#include <qobject.h>
#include <qpaintdevice.h>
#include <qpaintengine.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qrect.h>
#include <qregion.h>
#include <qscrollbar.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qtextcursor.h>
#include <qtextdocument.h>
#include <qtextformat.h>
#include <qurl.h>
#include <qwidget.h>

#include "qtscriptshell_QTextEdit.h"

static const char * const qtscript_QTextEdit_function_names[] = {
    "QTextEdit"
    // static
    // prototype
    , "acceptRichText"
    , "alignment"
    , "anchorAt"
    , "append"
    , "autoFormatting"
    , "canPaste"
    , "clear"
    , "copy"
    , "copyAvailable"
    , "createStandardContextMenu"
    , "currentCharFormat"
    , "currentCharFormatChanged"
    , "currentFont"
    , "cursorForPosition"
    , "cursorPositionChanged"
    , "cursorRect"
    , "cursorWidth"
    , "cut"
    , "document"
    , "documentTitle"
    , "ensureCursorVisible"
    , "extraSelections"
    , "find"
    , "fontFamily"
    , "fontItalic"
    , "fontPointSize"
    , "fontUnderline"
    , "fontWeight"
    , "inputMethodQuery"
    , "insertHtml"
    , "insertPlainText"
    , "isReadOnly"
    , "isUndoRedoEnabled"
    , "lineWrapColumnOrWidth"
    , "lineWrapMode"
    , "loadResource"
    , "mergeCurrentCharFormat"
    , "moveCursor"
    , "overwriteMode"
    , "paste"
    , "placeholderText"
    , "redo"
    , "redoAvailable"
    , "scrollToAnchor"
    , "selectAll"
    , "selectionChanged"
    , "setAcceptRichText"
    , "setAlignment"
    , "setAutoFormatting"
    , "setCurrentCharFormat"
    , "setCurrentFont"
    , "setCursorWidth"
    , "setDocument"
    , "setDocumentTitle"
    , "setExtraSelections"
    , "setFontFamily"
    , "setFontItalic"
    , "setFontPointSize"
    , "setFontUnderline"
    , "setFontWeight"
    , "setHtml"
    , "setLineWrapColumnOrWidth"
    , "setLineWrapMode"
    , "setMarkdown"
    , "setOverwriteMode"
    , "setPlaceholderText"
    , "setPlainText"
    , "setReadOnly"
    , "setTabChangesFocus"
    , "setTabStopDistance"
    , "setText"
    , "setTextBackgroundColor"
    , "setTextColor"
    , "setTextCursor"
    , "setTextInteractionFlags"
    , "setUndoRedoEnabled"
    , "setWordWrapMode"
    , "tabChangesFocus"
    , "tabStopDistance"
    , "textBackgroundColor"
    , "textChanged"
    , "textColor"
    , "textCursor"
    , "textInteractionFlags"
    , "toHtml"
    , "toPlainText"
    , "undo"
    , "undoAvailable"
    , "wordWrapMode"
    , "zoomIn"
    , "zoomOut"
    , "toString"
};

static const char * const qtscript_QTextEdit_function_signatures[] = {
    "QWidget parent\nString text, QWidget parent"
    // static
    // prototype
    , ""
    , ""
    , "QPoint pos"
    , "String text"
    , ""
    , ""
    , ""
    , ""
    , "bool b"
    , "\nQPoint position"
    , ""
    , "QTextCharFormat format"
    , ""
    , "QPoint pos"
    , ""
    , "\nQTextCursor cursor"
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "QRegularExpression exp, FindFlags options\nString exp, FindFlags options"
    , ""
    , ""
    , ""
    , ""
    , ""
    , "InputMethodQuery query, Object argument\nInputMethodQuery arg__1"
    , "String text"
    , "String text"
    , ""
    , ""
    , ""
    , ""
    , "int type, QUrl name"
    , "QTextCharFormat modifier"
    , "MoveOperation operation, MoveMode mode"
    , ""
    , ""
    , ""
    , ""
    , "bool b"
    , "String name"
    , ""
    , ""
    , "bool accept"
    , "Alignment a"
    , "AutoFormatting features"
    , "QTextCharFormat format"
    , "QFont f"
    , "int width"
    , "QTextDocument document"
    , "String title"
    , "List selections"
    , "String fontFamily"
    , "bool b"
    , "qreal s"
    , "bool b"
    , "int w"
    , "String text"
    , "int w"
    , "LineWrapMode mode"
    , "String markdown"
    , "bool overwrite"
    , "String placeholderText"
    , "String text"
    , "bool ro"
    , "bool b"
    , "qreal distance"
    , "String text"
    , "QColor c"
    , "QColor c"
    , "QTextCursor cursor"
    , "TextInteractionFlags flags"
    , "bool enable"
    , "WrapMode policy"
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
    , "bool b"
    , ""
    , "int range"
    , "int range"
""
};

static const int qtscript_QTextEdit_function_lengths[] = {
    2
    // static
    // prototype
    , 0
    , 0
    , 1
    , 1
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
    , 0
    , 1
    , 0
    , 1
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
    , 0
    , 0
    , 0
    , 0
    , 2
    , 1
    , 1
    , 0
    , 0
    , 0
    , 0
    , 2
    , 1
    , 2
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
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
    , 1
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
    , 0
    , 0
    , 0
    , 1
    , 0
    , 1
    , 1
    , 0
};

static QScriptValue qtscript_QTextEdit_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QTextEdit::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QTextEdit*)
Q_DECLARE_METATYPE(QtScriptShell_QTextEdit*)
Q_DECLARE_METATYPE(QTextEdit::AutoFormattingFlag)
Q_DECLARE_METATYPE(QFlags<QTextEdit::AutoFormattingFlag>)
Q_DECLARE_METATYPE(QTextEdit::LineWrapMode)
Q_DECLARE_METATYPE(QFlags<Qt::AlignmentFlag>)
Q_DECLARE_METATYPE(QMenu*)
Q_DECLARE_METATYPE(QTextCharFormat)
Q_DECLARE_METATYPE(QFont)
Q_DECLARE_METATYPE(QTextCursor)
Q_DECLARE_METATYPE(QTextDocument*)
Q_DECLARE_METATYPE(QTextEdit::ExtraSelection)
Q_DECLARE_METATYPE(QList<QTextEdit::ExtraSelection >)
Q_DECLARE_METATYPE(QFlags<QTextDocument::FindFlag>)
Q_DECLARE_METATYPE(Qt::InputMethodQuery)
Q_DECLARE_METATYPE(QTextCursor::MoveOperation)
Q_DECLARE_METATYPE(QTextCursor::MoveMode)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QFlags<Qt::TextInteractionFlag>)
Q_DECLARE_METATYPE(QTextOption::WrapMode)
Q_DECLARE_METATYPE(QWidget*)
Q_DECLARE_METATYPE(QAbstractScrollArea*)

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
// QTextEdit::AutoFormattingFlag
//

static const QTextEdit::AutoFormattingFlag qtscript_QTextEdit_AutoFormattingFlag_values[] = {
    QTextEdit::AutoAll
    , QTextEdit::AutoNone
    , QTextEdit::AutoBulletList
};

static const char * const qtscript_QTextEdit_AutoFormattingFlag_keys[] = {
    "AutoAll"
    , "AutoNone"
    , "AutoBulletList"
};

static QString qtscript_QTextEdit_AutoFormattingFlag_toStringHelper(QTextEdit::AutoFormattingFlag value)
{
    if ((value >= QTextEdit::AutoAll) && (value <= QTextEdit::AutoBulletList))
        return qtscript_QTextEdit_AutoFormattingFlag_keys[static_cast<int>(value)-static_cast<int>(QTextEdit::AutoAll)];
    return QString();
}

static QScriptValue qtscript_QTextEdit_AutoFormattingFlag_toScriptValue(QScriptEngine *engine, const QTextEdit::AutoFormattingFlag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextEdit"));
    return clazz.property(qtscript_QTextEdit_AutoFormattingFlag_toStringHelper(value));
}

static void qtscript_QTextEdit_AutoFormattingFlag_fromScriptValue(const QScriptValue &value, QTextEdit::AutoFormattingFlag &out)
{
    out = qvariant_cast<QTextEdit::AutoFormattingFlag>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextEdit_AutoFormattingFlag(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextEdit::AutoFormattingFlag>())
        arg = static_cast<int>(qvariant_cast<QTextEdit::AutoFormattingFlag>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("AutoFormattingFlag(): invalid enum value"));
    if ((arg >= QTextEdit::AutoAll) && (arg <= QTextEdit::AutoBulletList))
        return qScriptValueFromValue(engine,  static_cast<QTextEdit::AutoFormattingFlag>(arg));
    return context->throwError(QString::fromLatin1("AutoFormattingFlag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextEdit_AutoFormattingFlag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::AutoFormattingFlag value = qscriptvalue_cast<QTextEdit::AutoFormattingFlag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextEdit_AutoFormattingFlag_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::AutoFormattingFlag value = qscriptvalue_cast<QTextEdit::AutoFormattingFlag>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextEdit_AutoFormattingFlag_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextEdit_AutoFormattingFlag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextEdit_AutoFormattingFlag,
        qtscript_QTextEdit_AutoFormattingFlag_valueOf, qtscript_QTextEdit_AutoFormattingFlag_toString);
    qScriptRegisterMetaType<QTextEdit::AutoFormattingFlag>(engine, qtscript_QTextEdit_AutoFormattingFlag_toScriptValue,
        qtscript_QTextEdit_AutoFormattingFlag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextEdit_AutoFormattingFlag_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextEdit_AutoFormattingFlag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextEdit_AutoFormattingFlag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextEdit::AutoFormatting
//

static QScriptValue qtscript_QTextEdit_AutoFormatting_toScriptValue(QScriptEngine *engine, const QTextEdit::AutoFormatting &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_QTextEdit_AutoFormatting_fromScriptValue(const QScriptValue &value, QTextEdit::AutoFormatting &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<QTextEdit::AutoFormatting>())
        out = qvariant_cast<QTextEdit::AutoFormatting>(var);
    else if (var.userType() == qMetaTypeId<QTextEdit::AutoFormattingFlag>())
        out = qvariant_cast<QTextEdit::AutoFormattingFlag>(var);
    else
        out = {};
}

static QScriptValue qtscript_construct_QTextEdit_AutoFormatting(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::AutoFormatting result{};
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<QTextEdit::AutoFormatting>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<QTextEdit::AutoFormattingFlag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("AutoFormatting(): argument %0 is not of type AutoFormattingFlag").arg(i));
            }
            result |= qvariant_cast<QTextEdit::AutoFormattingFlag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_QTextEdit_AutoFormatting_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::AutoFormatting value = qscriptvalue_cast<QTextEdit::AutoFormatting>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextEdit_AutoFormatting_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::AutoFormatting value = qscriptvalue_cast<QTextEdit::AutoFormatting>(context->thisObject());
    QString result;
    for (int i = 0; i < 3; ++i) {
        if ((value & qtscript_QTextEdit_AutoFormattingFlag_values[i]) == qtscript_QTextEdit_AutoFormattingFlag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_QTextEdit_AutoFormattingFlag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_QTextEdit_AutoFormatting_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<QTextEdit::AutoFormatting>() == otherObj.value<QTextEdit::AutoFormatting>())));
}

static QScriptValue qtscript_create_QTextEdit_AutoFormatting_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_QTextEdit_AutoFormatting, qtscript_QTextEdit_AutoFormatting_valueOf,
        qtscript_QTextEdit_AutoFormatting_toString, qtscript_QTextEdit_AutoFormatting_equals);
    qScriptRegisterMetaType<QTextEdit::AutoFormatting>(engine, qtscript_QTextEdit_AutoFormatting_toScriptValue,
        qtscript_QTextEdit_AutoFormatting_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}

//
// QTextEdit::LineWrapMode
//

static const QTextEdit::LineWrapMode qtscript_QTextEdit_LineWrapMode_values[] = {
    QTextEdit::NoWrap
    , QTextEdit::WidgetWidth
    , QTextEdit::FixedPixelWidth
    , QTextEdit::FixedColumnWidth
};

static const char * const qtscript_QTextEdit_LineWrapMode_keys[] = {
    "NoWrap"
    , "WidgetWidth"
    , "FixedPixelWidth"
    , "FixedColumnWidth"
};

static QString qtscript_QTextEdit_LineWrapMode_toStringHelper(QTextEdit::LineWrapMode value)
{
    if ((value >= QTextEdit::NoWrap) && (value <= QTextEdit::FixedColumnWidth))
        return qtscript_QTextEdit_LineWrapMode_keys[static_cast<int>(value)-static_cast<int>(QTextEdit::NoWrap)];
    return QString();
}

static QScriptValue qtscript_QTextEdit_LineWrapMode_toScriptValue(QScriptEngine *engine, const QTextEdit::LineWrapMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QTextEdit"));
    return clazz.property(qtscript_QTextEdit_LineWrapMode_toStringHelper(value));
}

static void qtscript_QTextEdit_LineWrapMode_fromScriptValue(const QScriptValue &value, QTextEdit::LineWrapMode &out)
{
    out = qvariant_cast<QTextEdit::LineWrapMode>(value.toVariant());
}

static QScriptValue qtscript_construct_QTextEdit_LineWrapMode(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QTextEdit::LineWrapMode>())
        arg = static_cast<int>(qvariant_cast<QTextEdit::LineWrapMode>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("LineWrapMode(): invalid enum value"));
    if ((arg >= QTextEdit::NoWrap) && (arg <= QTextEdit::FixedColumnWidth))
        return qScriptValueFromValue(engine,  static_cast<QTextEdit::LineWrapMode>(arg));
    return context->throwError(QString::fromLatin1("LineWrapMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QTextEdit_LineWrapMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::LineWrapMode value = qscriptvalue_cast<QTextEdit::LineWrapMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QTextEdit_LineWrapMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    QTextEdit::LineWrapMode value = qscriptvalue_cast<QTextEdit::LineWrapMode>(context->thisObject());
    return QScriptValue(engine, qtscript_QTextEdit_LineWrapMode_toStringHelper(value));
}

static QScriptValue qtscript_create_QTextEdit_LineWrapMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QTextEdit_LineWrapMode,
        qtscript_QTextEdit_LineWrapMode_valueOf, qtscript_QTextEdit_LineWrapMode_toString);
    qScriptRegisterMetaType<QTextEdit::LineWrapMode>(engine, qtscript_QTextEdit_LineWrapMode_toScriptValue,
        qtscript_QTextEdit_LineWrapMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QTextEdit_LineWrapMode_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QTextEdit_LineWrapMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QTextEdit_LineWrapMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QTextEdit
//

static QScriptValue qtscript_QTextEdit_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 91;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QTextEdit* _q_self = qscriptvalue_cast<QTextEdit*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QTextEdit.%0(): this object is not a QTextEdit")
            .arg(qtscript_QTextEdit_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->acceptRichText();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        QFlags<Qt::AlignmentFlag> _q_result = _q_self->alignment();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        QPoint _q_arg0 = qscriptvalue_cast<QPoint>(context->argument(0));
        QString _q_result = _q_self->anchorAt(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->append(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        QFlags<QTextEdit::AutoFormattingFlag> _q_result = _q_self->autoFormatting();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->canPaste();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 0) {
        _q_self->clear();
        return context->engine()->undefinedValue();
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        _q_self->copy();
        return context->engine()->undefinedValue();
    }
    break;

    case 8:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->copyAvailable(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        QMenu* _q_result = _q_self->createStandardContextMenu();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QPoint _q_arg0 = qscriptvalue_cast<QPoint>(context->argument(0));
        QMenu* _q_result = _q_self->createStandardContextMenu(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        QTextCharFormat _q_result = _q_self->currentCharFormat();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 1) {
        QTextCharFormat _q_arg0 = qscriptvalue_cast<QTextCharFormat>(context->argument(0));
        _q_self->currentCharFormatChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 12:
    if (context->argumentCount() == 0) {
        QFont _q_result = _q_self->currentFont();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 13:
    if (context->argumentCount() == 1) {
        QPoint _q_arg0 = qscriptvalue_cast<QPoint>(context->argument(0));
        QTextCursor _q_result = _q_self->cursorForPosition(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 14:
    if (context->argumentCount() == 0) {
        _q_self->cursorPositionChanged();
        return context->engine()->undefinedValue();
    }
    break;

    case 15:
    if (context->argumentCount() == 0) {
        QRect _q_result = _q_self->cursorRect();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QTextCursor _q_arg0 = qscriptvalue_cast<QTextCursor>(context->argument(0));
        QRect _q_result = _q_self->cursorRect(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 16:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->cursorWidth();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 17:
    if (context->argumentCount() == 0) {
        _q_self->cut();
        return context->engine()->undefinedValue();
    }
    break;

    case 18:
    if (context->argumentCount() == 0) {
        QTextDocument* _q_result = _q_self->document();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 19:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->documentTitle();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 20:
    if (context->argumentCount() == 0) {
        _q_self->ensureCursorVisible();
        return context->engine()->undefinedValue();
    }
    break;

    case 21:
    if (context->argumentCount() == 0) {
        QList<QTextEdit::ExtraSelection > _q_result = _q_self->extraSelections();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 22:
    if (context->argumentCount() == 1) {
        if (context->argument(0).isRegExp()) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            bool _q_result = _q_self->find(_q_arg0);
            return QScriptValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            bool _q_result = _q_self->find(_q_arg0);
            return QScriptValue(context->engine(), _q_result);
        }
    }
    if (context->argumentCount() == 2) {
        if (context->argument(0).isRegExp()
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(1).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(1).toVariant().userType()))) {
            QRegExp _q_arg0_rx = context->argument(0).toRegExp();
            QRegularExpression::PatternOptions _q_arg0_rxopts;
            if (_q_arg0_rx.caseSensitivity() == Qt::CaseInsensitive)
                _q_arg0_rxopts |= QRegularExpression::CaseInsensitiveOption;
            QRegularExpression _q_arg0(_q_arg0_rx.pattern(), _q_arg0_rxopts);
            QFlags<QTextDocument::FindFlag> _q_arg1 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(1));
            bool _q_result = _q_self->find(_q_arg0, _q_arg1);
            return QScriptValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()
            && ((qMetaTypeId<QFlags<QTextDocument::FindFlag> >() == context->argument(1).toVariant().userType()) || (qMetaTypeId<QTextDocument::FindFlag>() == context->argument(1).toVariant().userType()))) {
            QString _q_arg0 = context->argument(0).toString();
            QFlags<QTextDocument::FindFlag> _q_arg1 = qscriptvalue_cast<QFlags<QTextDocument::FindFlag> >(context->argument(1));
            bool _q_result = _q_self->find(_q_arg0, _q_arg1);
            return QScriptValue(context->engine(), _q_result);
        }
    }
    break;

    case 23:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->fontFamily();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 24:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->fontItalic();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 25:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->fontPointSize();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 26:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->fontUnderline();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 27:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->fontWeight();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 28:
    if (context->argumentCount() == 1) {
        Qt::InputMethodQuery _q_arg0 = qscriptvalue_cast<Qt::InputMethodQuery>(context->argument(0));
        QVariant _q_result = static_cast<QWidget*>(_q_self)->inputMethodQuery(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 2) {
        Qt::InputMethodQuery _q_arg0 = qscriptvalue_cast<Qt::InputMethodQuery>(context->argument(0));
        QVariant _q_arg1 = context->argument(1).toVariant();
        QVariant _q_result = _q_self->inputMethodQuery(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 29:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->insertHtml(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 30:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->insertPlainText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 31:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isReadOnly();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 32:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isUndoRedoEnabled();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 33:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->lineWrapColumnOrWidth();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 34:
    if (context->argumentCount() == 0) {
        QTextEdit::LineWrapMode _q_result = _q_self->lineWrapMode();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 35:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        QUrl _q_arg1 = qscriptvalue_cast<QUrl>(context->argument(1));
        QVariant _q_result = _q_self->loadResource(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 36:
    if (context->argumentCount() == 1) {
        QTextCharFormat _q_arg0 = qscriptvalue_cast<QTextCharFormat>(context->argument(0));
        _q_self->mergeCurrentCharFormat(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 37:
    if (context->argumentCount() == 1) {
        QTextCursor::MoveOperation _q_arg0 = qscriptvalue_cast<QTextCursor::MoveOperation>(context->argument(0));
        _q_self->moveCursor(_q_arg0);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 2) {
        QTextCursor::MoveOperation _q_arg0 = qscriptvalue_cast<QTextCursor::MoveOperation>(context->argument(0));
        QTextCursor::MoveMode _q_arg1 = qscriptvalue_cast<QTextCursor::MoveMode>(context->argument(1));
        _q_self->moveCursor(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 38:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->overwriteMode();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 39:
    if (context->argumentCount() == 0) {
        _q_self->paste();
        return context->engine()->undefinedValue();
    }
    break;

    case 40:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->placeholderText();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 41:
    if (context->argumentCount() == 0) {
        _q_self->redo();
        return context->engine()->undefinedValue();
    }
    break;

    case 42:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->redoAvailable(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 43:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->scrollToAnchor(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 44:
    if (context->argumentCount() == 0) {
        _q_self->selectAll();
        return context->engine()->undefinedValue();
    }
    break;

    case 45:
    if (context->argumentCount() == 0) {
        _q_self->selectionChanged();
        return context->engine()->undefinedValue();
    }
    break;

    case 46:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setAcceptRichText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 47:
    if (context->argumentCount() == 1) {
        QFlags<Qt::AlignmentFlag> _q_arg0 = qscriptvalue_cast<QFlags<Qt::AlignmentFlag> >(context->argument(0));
        _q_self->setAlignment(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 48:
    if (context->argumentCount() == 1) {
        QFlags<QTextEdit::AutoFormattingFlag> _q_arg0 = qscriptvalue_cast<QFlags<QTextEdit::AutoFormattingFlag> >(context->argument(0));
        _q_self->setAutoFormatting(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 49:
    if (context->argumentCount() == 1) {
        QTextCharFormat _q_arg0 = qscriptvalue_cast<QTextCharFormat>(context->argument(0));
        _q_self->setCurrentCharFormat(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 50:
    if (context->argumentCount() == 1) {
        QFont _q_arg0 = qscriptvalue_cast<QFont>(context->argument(0));
        _q_self->setCurrentFont(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 51:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->setCursorWidth(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 52:
    if (context->argumentCount() == 1) {
        QTextDocument* _q_arg0 = qscriptvalue_cast<QTextDocument*>(context->argument(0));
        _q_self->setDocument(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 53:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setDocumentTitle(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 54:
    if (context->argumentCount() == 1) {
        QList<QTextEdit::ExtraSelection> _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        _q_self->setExtraSelections(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 55:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setFontFamily(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 56:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setFontItalic(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 57:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setFontPointSize(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 58:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setFontUnderline(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 59:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->setFontWeight(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 60:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setHtml(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 61:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->setLineWrapColumnOrWidth(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 62:
    if (context->argumentCount() == 1) {
        QTextEdit::LineWrapMode _q_arg0 = qscriptvalue_cast<QTextEdit::LineWrapMode>(context->argument(0));
        _q_self->setLineWrapMode(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 63:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setMarkdown(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 64:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setOverwriteMode(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 65:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setPlaceholderText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 66:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setPlainText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 67:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setReadOnly(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 68:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setTabChangesFocus(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 69:
    if (context->argumentCount() == 1) {
        qreal _q_arg0 = qscriptvalue_cast<qreal>(context->argument(0));
        _q_self->setTabStopDistance(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 70:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setText(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 71:
    if (context->argumentCount() == 1) {
        QColor _q_arg0 = qscriptvalue_cast<QColor>(context->argument(0));
        _q_self->setTextBackgroundColor(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 72:
    if (context->argumentCount() == 1) {
        QColor _q_arg0 = qscriptvalue_cast<QColor>(context->argument(0));
        _q_self->setTextColor(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 73:
    if (context->argumentCount() == 1) {
        QTextCursor _q_arg0 = qscriptvalue_cast<QTextCursor>(context->argument(0));
        _q_self->setTextCursor(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 74:
    if (context->argumentCount() == 1) {
        QFlags<Qt::TextInteractionFlag> _q_arg0 = qscriptvalue_cast<QFlags<Qt::TextInteractionFlag> >(context->argument(0));
        _q_self->setTextInteractionFlags(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 75:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->setUndoRedoEnabled(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 76:
    if (context->argumentCount() == 1) {
        QTextOption::WrapMode _q_arg0 = qscriptvalue_cast<QTextOption::WrapMode>(context->argument(0));
        _q_self->setWordWrapMode(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 77:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->tabChangesFocus();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 78:
    if (context->argumentCount() == 0) {
        qreal _q_result = _q_self->tabStopDistance();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 79:
    if (context->argumentCount() == 0) {
        QColor _q_result = _q_self->textBackgroundColor();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 80:
    if (context->argumentCount() == 0) {
        _q_self->textChanged();
        return context->engine()->undefinedValue();
    }
    break;

    case 81:
    if (context->argumentCount() == 0) {
        QColor _q_result = _q_self->textColor();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 82:
    if (context->argumentCount() == 0) {
        QTextCursor _q_result = _q_self->textCursor();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 83:
    if (context->argumentCount() == 0) {
        QFlags<Qt::TextInteractionFlag> _q_result = _q_self->textInteractionFlags();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 84:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toHtml();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 85:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toPlainText();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 86:
    if (context->argumentCount() == 0) {
        _q_self->undo();
        return context->engine()->undefinedValue();
    }
    break;

    case 87:
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        _q_self->undoAvailable(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 88:
    if (context->argumentCount() == 0) {
        QTextOption::WrapMode _q_result = _q_self->wordWrapMode();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 89:
    if (context->argumentCount() == 0) {
        _q_self->zoomIn();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->zoomIn(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 90:
    if (context->argumentCount() == 0) {
        _q_self->zoomOut();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->zoomOut(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 91: {
    QString result = QString::fromLatin1("QTextEdit");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QTextEdit_throw_ambiguity_error_helper(context,
        qtscript_QTextEdit_function_names[_id+1],
        qtscript_QTextEdit_function_signatures[_id+1]);
}

static QScriptValue qtscript_QTextEdit_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QTextEdit(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QTextEdit* _q_cpp_result = new QtScriptShell_QTextEdit();
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextEdit*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if (qscriptvalue_cast<QWidget*>(context->argument(0))) {
            QWidget* _q_arg0 = qscriptvalue_cast<QWidget*>(context->argument(0));
            QtScriptShell_QTextEdit* _q_cpp_result = new QtScriptShell_QTextEdit(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextEdit*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QtScriptShell_QTextEdit* _q_cpp_result = new QtScriptShell_QTextEdit(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextEdit*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        }
    } else if (context->argumentCount() == 2) {
        QString _q_arg0 = context->argument(0).toString();
        QWidget* _q_arg1 = qscriptvalue_cast<QWidget*>(context->argument(1));
        QtScriptShell_QTextEdit* _q_cpp_result = new QtScriptShell_QTextEdit(_q_arg0, _q_arg1);
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QTextEdit*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QTextEdit_throw_ambiguity_error_helper(context,
        qtscript_QTextEdit_function_names[_id],
        qtscript_QTextEdit_function_signatures[_id]);
}

static QScriptValue qtscript_QTextEdit_toScriptValue(QScriptEngine *engine, QTextEdit* const &in)
{
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

static void qtscript_QTextEdit_fromScriptValue(const QScriptValue &value, QTextEdit* &out)
{
    out = qobject_cast<QTextEdit*>(value.toQObject());
}

QScriptValue qtscript_create_QTextEdit_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QTextEdit*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QTextEdit*)0));
    proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QAbstractScrollArea*>()));
    for (int i = 0; i < 92; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QTextEdit_prototype_call, qtscript_QTextEdit_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QTextEdit_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    qScriptRegisterMetaType<QTextEdit*>(engine, qtscript_QTextEdit_toScriptValue, 
        qtscript_QTextEdit_fromScriptValue, proto);

    QScriptValue ctor = engine->newFunction(qtscript_QTextEdit_static_call, proto, qtscript_QTextEdit_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("AutoFormattingFlag"),
        qtscript_create_QTextEdit_AutoFormattingFlag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("AutoFormatting"),
        qtscript_create_QTextEdit_AutoFormatting_class(engine));
    ctor.setProperty(QString::fromLatin1("LineWrapMode"),
        qtscript_create_QTextEdit_LineWrapMode_class(engine, ctor));
    return ctor;
}
