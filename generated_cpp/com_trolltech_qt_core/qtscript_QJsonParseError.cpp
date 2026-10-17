#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qjsondocument.h>
#include <QVariant>

#include "qtscriptshell_QJsonParseError.h"

static const char * const qtscript_QJsonParseError_function_names[] = {
    "QJsonParseError"
    // static
    // prototype
    , "errorString"
    , "toString"
};

static const char * const qtscript_QJsonParseError_function_signatures[] = {
    ""
    // static
    // prototype
    , ""
""
};

static const int qtscript_QJsonParseError_function_lengths[] = {
    0
    // static
    // prototype
    , 0
    , 0
};

static QScriptValue qtscript_QJsonParseError_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QJsonParseError::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QJsonParseError)
Q_DECLARE_METATYPE(QJsonParseError*)
Q_DECLARE_METATYPE(QtScriptShell_QJsonParseError)
Q_DECLARE_METATYPE(QtScriptShell_QJsonParseError*)
Q_DECLARE_METATYPE(QJsonParseError::ParseError)

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
// QJsonParseError::ParseError
//

static const QJsonParseError::ParseError qtscript_QJsonParseError_ParseError_values[] = {
    QJsonParseError::NoError
    , QJsonParseError::UnterminatedObject
    , QJsonParseError::MissingNameSeparator
    , QJsonParseError::UnterminatedArray
    , QJsonParseError::MissingValueSeparator
    , QJsonParseError::IllegalValue
    , QJsonParseError::TerminationByNumber
    , QJsonParseError::IllegalNumber
    , QJsonParseError::IllegalEscapeSequence
    , QJsonParseError::IllegalUTF8String
    , QJsonParseError::UnterminatedString
    , QJsonParseError::MissingObject
    , QJsonParseError::DeepNesting
    , QJsonParseError::DocumentTooLarge
    , QJsonParseError::GarbageAtEnd
};

static const char * const qtscript_QJsonParseError_ParseError_keys[] = {
    "NoError"
    , "UnterminatedObject"
    , "MissingNameSeparator"
    , "UnterminatedArray"
    , "MissingValueSeparator"
    , "IllegalValue"
    , "TerminationByNumber"
    , "IllegalNumber"
    , "IllegalEscapeSequence"
    , "IllegalUTF8String"
    , "UnterminatedString"
    , "MissingObject"
    , "DeepNesting"
    , "DocumentTooLarge"
    , "GarbageAtEnd"
};

static QString qtscript_QJsonParseError_ParseError_toStringHelper(QJsonParseError::ParseError value)
{
    if ((value >= QJsonParseError::NoError) && (value <= QJsonParseError::GarbageAtEnd))
        return qtscript_QJsonParseError_ParseError_keys[static_cast<int>(value)-static_cast<int>(QJsonParseError::NoError)];
    return QString();
}

static QScriptValue qtscript_QJsonParseError_ParseError_toScriptValue(QScriptEngine *engine, const QJsonParseError::ParseError &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QJsonParseError"));
    return clazz.property(qtscript_QJsonParseError_ParseError_toStringHelper(value));
}

static void qtscript_QJsonParseError_ParseError_fromScriptValue(const QScriptValue &value, QJsonParseError::ParseError &out)
{
    out = qvariant_cast<QJsonParseError::ParseError>(value.toVariant());
}

static QScriptValue qtscript_construct_QJsonParseError_ParseError(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QJsonParseError::ParseError>())
        arg = static_cast<int>(qvariant_cast<QJsonParseError::ParseError>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("ParseError(): invalid enum value"));
    if ((arg >= QJsonParseError::NoError) && (arg <= QJsonParseError::GarbageAtEnd))
        return qScriptValueFromValue(engine,  static_cast<QJsonParseError::ParseError>(arg));
    return context->throwError(QString::fromLatin1("ParseError(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QJsonParseError_ParseError_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QJsonParseError::ParseError value = qscriptvalue_cast<QJsonParseError::ParseError>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QJsonParseError_ParseError_toString(QScriptContext *context, QScriptEngine *engine)
{
    QJsonParseError::ParseError value = qscriptvalue_cast<QJsonParseError::ParseError>(context->thisObject());
    return QScriptValue(engine, qtscript_QJsonParseError_ParseError_toStringHelper(value));
}

static QScriptValue qtscript_create_QJsonParseError_ParseError_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QJsonParseError_ParseError,
        qtscript_QJsonParseError_ParseError_valueOf, qtscript_QJsonParseError_ParseError_toString);
    qScriptRegisterMetaType<QJsonParseError::ParseError>(engine, qtscript_QJsonParseError_ParseError_toScriptValue,
        qtscript_QJsonParseError_ParseError_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 15; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QJsonParseError_ParseError_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QJsonParseError_ParseError_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonParseError_ParseError_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QJsonParseError
//

static QScriptValue qtscript_QJsonParseError_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 1;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QJsonParseError* _q_self = qscriptvalue_cast<QJsonParseError*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QJsonParseError.%0(): this object is not a QJsonParseError")
            .arg(qtscript_QJsonParseError_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->errorString();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 1: {
    QString result = QString::fromLatin1("QJsonParseError");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonParseError_throw_ambiguity_error_helper(context,
        qtscript_QJsonParseError_function_names[_id+1],
        qtscript_QJsonParseError_function_signatures[_id+1]);
}

static QScriptValue qtscript_QJsonParseError_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QJsonParseError(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QJsonParseError _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue((QJsonParseError)_q_cpp_result));
        _q_cpp_result.__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonParseError_throw_ambiguity_error_helper(context,
        qtscript_QJsonParseError_function_names[_id],
        qtscript_QJsonParseError_function_signatures[_id]);
}

QScriptValue qtscript_create_QJsonParseError_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QJsonParseError*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QJsonParseError*)0));
    for (int i = 0; i < 2; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonParseError_prototype_call, qtscript_QJsonParseError_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QJsonParseError_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    {
        QScriptValue errorGetter = engine->newFunction(
            [](QScriptContext *ctx, QScriptEngine *eng) -> QScriptValue {
                QJsonParseError *self = qscriptvalue_cast<QJsonParseError*>(ctx->thisObject());
                if (!self)
                    return ctx->throwError(QScriptContext::TypeError,
                        QString::fromLatin1("QJsonParseError.error: expected a QJsonParseError"));
                return qScriptValueFromValue(eng, self->error);
            }, 0);
        proto.setProperty(QString::fromLatin1("error"), errorGetter,
            QScriptValue::PropertyGetter | QScriptValue::SkipInEnumeration);
        QScriptValue offsetGetter = engine->newFunction(
            [](QScriptContext *ctx, QScriptEngine *eng) -> QScriptValue {
                QJsonParseError *self = qscriptvalue_cast<QJsonParseError*>(ctx->thisObject());
                if (!self)
                    return ctx->throwError(QScriptContext::TypeError,
                        QString::fromLatin1("QJsonParseError.offset: expected a QJsonParseError"));
                return QScriptValue(eng, self->offset);
            }, 0);
        proto.setProperty(QString::fromLatin1("offset"), offsetGetter,
            QScriptValue::PropertyGetter | QScriptValue::SkipInEnumeration);
    }
    

    engine->setDefaultPrototype(qMetaTypeId<QJsonParseError>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QJsonParseError*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QJsonParseError_static_call, proto, qtscript_QJsonParseError_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("ParseError"),
        qtscript_create_QJsonParseError_ParseError_class(engine, ctor));
    return ctor;
}
