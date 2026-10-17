#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qjsondocument.h>
#include <QVariant>
#include <qbytearray.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qstringview.h>

#include "qtscriptshell_QJsonDocument.h"

static const char * const qtscript_QJsonDocument_function_names[] = {
    "QJsonDocument"
    // static
    , "fromJson"
    , "fromVariant"
    // prototype
    , "array"
    , "isArray"
    , "isEmpty"
    , "isNull"
    , "isObject"
    , "object"
    , "operator_subscript"
    , "setArray"
    , "setObject"
    , "swap"
    , "toJson"
    , "toVariant"
    , "toString"
};

static const char * const qtscript_QJsonDocument_function_signatures[] = {
    "\nQJsonArray array\nQJsonDocument other\nQJsonObject object"
    // static
    , "QByteArray json, QJsonParseError error"
    , "Object variant"
    // prototype
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "String key\nQStringView key\nString key\nqsizetype i"
    , "QJsonArray array"
    , "QJsonObject object"
    , "QJsonDocument other"
    , "JsonFormat format"
    , ""
""
};

static const int qtscript_QJsonDocument_function_lengths[] = {
    1
    // static
    , 2
    , 1
    // prototype
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
};

static QScriptValue qtscript_QJsonDocument_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QJsonDocument::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QJsonDocument*)
Q_DECLARE_METATYPE(QtScriptShell_QJsonDocument)
Q_DECLARE_METATYPE(QtScriptShell_QJsonDocument*)
Q_DECLARE_METATYPE(QJsonDocument::JsonFormat)
Q_DECLARE_METATYPE(QLatin1String)
Q_DECLARE_METATYPE(QStringView)
Q_DECLARE_METATYPE(qsizetype)
Q_DECLARE_METATYPE(QJsonParseError*)

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

//
// QJsonDocument::JsonFormat
//

static const QJsonDocument::JsonFormat qtscript_QJsonDocument_JsonFormat_values[] = {
    QJsonDocument::Indented
    , QJsonDocument::Compact
};

static const char * const qtscript_QJsonDocument_JsonFormat_keys[] = {
    "Indented"
    , "Compact"
};

static QString qtscript_QJsonDocument_JsonFormat_toStringHelper(QJsonDocument::JsonFormat value)
{
    if ((value >= QJsonDocument::Indented) && (value <= QJsonDocument::Compact))
        return qtscript_QJsonDocument_JsonFormat_keys[static_cast<int>(value)-static_cast<int>(QJsonDocument::Indented)];
    return QString();
}

static QScriptValue qtscript_QJsonDocument_JsonFormat_toScriptValue(QScriptEngine *engine, const QJsonDocument::JsonFormat &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QJsonDocument"));
    return clazz.property(qtscript_QJsonDocument_JsonFormat_toStringHelper(value));
}

static void qtscript_QJsonDocument_JsonFormat_fromScriptValue(const QScriptValue &value, QJsonDocument::JsonFormat &out)
{
    out = qvariant_cast<QJsonDocument::JsonFormat>(value.toVariant());
}

static QScriptValue qtscript_construct_QJsonDocument_JsonFormat(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QJsonDocument::JsonFormat>())
        arg = static_cast<int>(qvariant_cast<QJsonDocument::JsonFormat>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("JsonFormat(): invalid enum value"));
    if ((arg >= QJsonDocument::Indented) && (arg <= QJsonDocument::Compact))
        return qScriptValueFromValue(engine,  static_cast<QJsonDocument::JsonFormat>(arg));
    return context->throwError(QString::fromLatin1("JsonFormat(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QJsonDocument_JsonFormat_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QJsonDocument::JsonFormat value = qscriptvalue_cast<QJsonDocument::JsonFormat>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QJsonDocument_JsonFormat_toString(QScriptContext *context, QScriptEngine *engine)
{
    QJsonDocument::JsonFormat value = qscriptvalue_cast<QJsonDocument::JsonFormat>(context->thisObject());
    return QScriptValue(engine, qtscript_QJsonDocument_JsonFormat_toStringHelper(value));
}

static QScriptValue qtscript_create_QJsonDocument_JsonFormat_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QJsonDocument_JsonFormat,
        qtscript_QJsonDocument_JsonFormat_valueOf, qtscript_QJsonDocument_JsonFormat_toString);
    qScriptRegisterMetaType<QJsonDocument::JsonFormat>(engine, qtscript_QJsonDocument_JsonFormat_toScriptValue,
        qtscript_QJsonDocument_JsonFormat_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QJsonDocument_JsonFormat_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QJsonDocument_JsonFormat_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonDocument_JsonFormat_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QJsonDocument
//

static QScriptValue qtscript_QJsonDocument_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 12;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QJsonDocument* _q_self = qscriptvalue_cast<QJsonDocument*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QJsonDocument.%0(): this object is not a QJsonDocument")
            .arg(qtscript_QJsonDocument_function_names[_id+3]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 0) {
        QJsonArray _q_result = _q_self->array();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isArray();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isEmpty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isNull();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isObject();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        QJsonObject _q_result = _q_self->object();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            const QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            const QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            const QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((qMetaTypeId<qsizetype>() == context->argument(0).toVariant().userType())) {
            qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
            const QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 7:
    if (context->argumentCount() == 1) {
        QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
        _q_self->setArray(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 8:
    if (context->argumentCount() == 1) {
        QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
        _q_self->setObject(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 9:
    if (context->argumentCount() == 1) {
        QJsonDocument* _q_arg0_ptr = qscriptvalue_cast<QJsonDocument*>(context->argument(0));
        if (!_q_arg0_ptr)
            return context->throwError(QScriptContext::TypeError,
                QString::fromLatin1("QJsonDocument.swap(): argument 1 must be a QJsonDocument"));
        QJsonDocument& _q_arg0 = *_q_arg0_ptr;
        _q_self->swap(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        QByteArray _q_result = _q_self->toJson();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QJsonDocument::JsonFormat _q_arg0 = qscriptvalue_cast<QJsonDocument::JsonFormat>(context->argument(0));
        QByteArray _q_result = _q_self->toJson(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 0) {
        QVariant _q_result = _q_self->toVariant();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 12: {
    QString result;
    QDebug d(&result);
    d << *_q_self;
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonDocument_throw_ambiguity_error_helper(context,
        qtscript_QJsonDocument_function_names[_id+3],
        qtscript_QJsonDocument_function_signatures[_id+3]);
}

static QScriptValue qtscript_QJsonDocument_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QJsonDocument(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QJsonDocument _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue((QJsonDocument)_q_cpp_result));
        _q_cpp_result.__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QJsonArray>() == context->argument(0).toVariant().userType())) {
            QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
            QtScriptShell_QJsonDocument _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue((QJsonDocument)_q_cpp_result));
            _q_cpp_result.__qtscript_self = _q_result;
            return _q_result;
        } else if ((qMetaTypeId<QJsonDocument>() == context->argument(0).toVariant().userType())) {
            QJsonDocument _q_arg0 = qscriptvalue_cast<QJsonDocument>(context->argument(0));
            QtScriptShell_QJsonDocument _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue((QJsonDocument)_q_cpp_result));
            _q_cpp_result.__qtscript_self = _q_result;
            return _q_result;
        } else if ((qMetaTypeId<QJsonObject>() == context->argument(0).toVariant().userType())) {
            QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
            QtScriptShell_QJsonDocument _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue((QJsonDocument)_q_cpp_result));
            _q_cpp_result.__qtscript_self = _q_result;
            return _q_result;
        }
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        QByteArray _q_arg0 = qscriptvalue_cast<QByteArray>(context->argument(0));
        QJsonDocument _q_result = QJsonDocument::fromJson(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 2) {
        QByteArray _q_arg0 = qscriptvalue_cast<QByteArray>(context->argument(0));
        QJsonParseError* _q_arg1 = qscriptvalue_cast<QJsonParseError*>(context->argument(1));
        QJsonDocument _q_result = QJsonDocument::fromJson(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        QVariant _q_arg0 = context->argument(0).toVariant();
        QJsonDocument _q_result = QJsonDocument::fromVariant(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonDocument_throw_ambiguity_error_helper(context,
        qtscript_QJsonDocument_function_names[_id],
        qtscript_QJsonDocument_function_signatures[_id]);
}

QScriptValue qtscript_create_QJsonDocument_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QJsonDocument*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QJsonDocument*)0));
    for (int i = 0; i < 13; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonDocument_prototype_call, qtscript_QJsonDocument_function_lengths[i+3]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QJsonDocument_function_names[i+3]),
            fun, QScriptValue::SkipInEnumeration);
    }

    engine->setDefaultPrototype(qMetaTypeId<QJsonDocument>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QJsonDocument*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QJsonDocument_static_call, proto, qtscript_QJsonDocument_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));
    for (int i = 0; i < 2; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonDocument_static_call,
            qtscript_QJsonDocument_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i+1)));
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonDocument_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    ctor.setProperty(QString::fromLatin1("JsonFormat"),
        qtscript_create_QJsonDocument_JsonFormat_class(engine, ctor));
    return ctor;
}
