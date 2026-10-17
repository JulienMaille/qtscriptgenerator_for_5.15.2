#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qjsonvalue.h>
#include <QVariant>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qstringview.h>

static const char * const qtscript_QJsonValue_function_names[] = {
    "QJsonValue"
    // static
    , "fromVariant"
    // prototype
    , "isArray"
    , "isBool"
    , "isDouble"
    , "isNull"
    , "isObject"
    , "isString"
    , "isUndefined"
    , "operator_subscript"
    , "swap"
    , "toArray"
    , "toBool"
    , "toDouble"
    , "toInt"
    , "toInteger"
    , "toObject"
    , "toString"
    , "toVariant"
    , "type"
};

static const char * const qtscript_QJsonValue_function_signatures[] = {
    "QJsonArray a\nQJsonObject o\nType arg__1\nString s\nbool b\nQJsonArray a\nQJsonObject o\nQJsonValue other\nString s\nchar s\ndouble n\nint n\nqint64 v"
    // static
    , "Object variant"
    // prototype
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "String key\nQStringView key\nString key\nqsizetype i"
    , "QJsonValue other"
    , "\nQJsonArray defaultValue"
    , "bool defaultValue"
    , "double defaultValue"
    , "int defaultValue"
    , "qint64 defaultValue"
    , "\nQJsonObject defaultValue"
    , "\nString defaultValue"
    , ""
    , ""
};

static const int qtscript_QJsonValue_function_lengths[] = {
    1
    // static
    , 1
    // prototype
    , 0
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
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
};

static QScriptValue qtscript_QJsonValue_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QJsonValue::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QJsonValue*)
Q_DECLARE_METATYPE(QJsonValue::Type)
Q_DECLARE_METATYPE(QLatin1String)
Q_DECLARE_METATYPE(QStringView)
Q_DECLARE_METATYPE(qsizetype)
Q_DECLARE_METATYPE(const char*)

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
// QJsonValue::Type
//

static const QJsonValue::Type qtscript_QJsonValue_Type_values[] = {
    QJsonValue::Null
    , QJsonValue::Bool
    , QJsonValue::Double
    , QJsonValue::String
    , QJsonValue::Array
    , QJsonValue::Object
    , QJsonValue::Undefined
};

static const char * const qtscript_QJsonValue_Type_keys[] = {
    "Null"
    , "Bool"
    , "Double"
    , "String"
    , "Array"
    , "Object"
    , "Undefined"
};

static QString qtscript_QJsonValue_Type_toStringHelper(QJsonValue::Type value)
{
    for (int i = 0; i < 7; ++i) {
        if (qtscript_QJsonValue_Type_values[i] == value)
            return QString::fromLatin1(qtscript_QJsonValue_Type_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_QJsonValue_Type_toScriptValue(QScriptEngine *engine, const QJsonValue::Type &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QJsonValue"));
    return clazz.property(qtscript_QJsonValue_Type_toStringHelper(value));
}

static void qtscript_QJsonValue_Type_fromScriptValue(const QScriptValue &value, QJsonValue::Type &out)
{
    out = qvariant_cast<QJsonValue::Type>(value.toVariant());
}

static QScriptValue qtscript_construct_QJsonValue_Type(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QJsonValue::Type>())
        arg = static_cast<int>(qvariant_cast<QJsonValue::Type>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("Type(): invalid enum value"));
    for (int i = 0; i < 7; ++i) {
        if (qtscript_QJsonValue_Type_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<QJsonValue::Type>(arg));
    }
    return context->throwError(QString::fromLatin1("Type(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QJsonValue_Type_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QJsonValue::Type value = qscriptvalue_cast<QJsonValue::Type>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QJsonValue_Type_toString(QScriptContext *context, QScriptEngine *engine)
{
    QJsonValue::Type value = qscriptvalue_cast<QJsonValue::Type>(context->thisObject());
    return QScriptValue(engine, qtscript_QJsonValue_Type_toStringHelper(value));
}

static QScriptValue qtscript_create_QJsonValue_Type_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QJsonValue_Type,
        qtscript_QJsonValue_Type_valueOf, qtscript_QJsonValue_Type_toString);
    qScriptRegisterMetaType<QJsonValue::Type>(engine, qtscript_QJsonValue_Type_toScriptValue,
        qtscript_QJsonValue_Type_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 7; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QJsonValue_Type_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QJsonValue_Type_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonValue_Type_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QJsonValue
//

static QScriptValue qtscript_QJsonValue_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 18;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QJsonValue* _q_self = qscriptvalue_cast<QJsonValue*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QJsonValue.%0(): this object is not a QJsonValue")
            .arg(qtscript_QJsonValue_function_names[_id+2]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isArray();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isBool();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isDouble();
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
        bool _q_result = _q_self->isString();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isUndefined();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 7:
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

    case 8:
    if (context->argumentCount() == 1) {
        QJsonValue* _q_arg0_ptr = qscriptvalue_cast<QJsonValue*>(context->argument(0));
        if (!_q_arg0_ptr)
            return context->throwError(QScriptContext::TypeError,
                QString::fromLatin1("QJsonValue.swap(): argument 1 must be a QJsonValue"));
        QJsonValue& _q_arg0 = *_q_arg0_ptr;
        _q_self->swap(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        QJsonArray _q_result = _q_self->toArray();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
        QJsonArray _q_result = _q_self->toArray(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->toBool();
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        bool _q_arg0 = context->argument(0).toBoolean();
        bool _q_result = _q_self->toBool(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 0) {
        double _q_result = _q_self->toDouble();
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        double _q_arg0 = context->argument(0).toNumber();
        double _q_result = _q_self->toDouble(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->toInt();
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        int _q_result = _q_self->toInt(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 13:
    if (context->argumentCount() == 0) {
        qint64 _q_result = _q_self->toInteger();
        return qScriptValueFromValue(context->engine(), QVariant::fromValue(_q_result));
    }
    if (context->argumentCount() == 1) {
        qint64 _q_arg0 = qscriptvalue_cast<qint64>(context->argument(0));
        qint64 _q_result = _q_self->toInteger(_q_arg0);
        return qScriptValueFromValue(context->engine(), QVariant::fromValue(_q_result));
    }
    break;

    case 14:
    if (context->argumentCount() == 0) {
        QJsonObject _q_result = _q_self->toObject();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
        QJsonObject _q_result = _q_self->toObject(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 15:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->toString();
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        QString _q_result = _q_self->toString(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 16:
    if (context->argumentCount() == 0) {
        QVariant _q_result = _q_self->toVariant();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 17:
    if (context->argumentCount() == 0) {
        QJsonValue::Type _q_result = _q_self->type();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonValue_throw_ambiguity_error_helper(context,
        qtscript_QJsonValue_function_names[_id+2],
        qtscript_QJsonValue_function_signatures[_id+2]);
}

static QScriptValue qtscript_QJsonValue_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QJsonValue(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QJsonValue _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QJsonArray>() == context->argument(0).toVariant().userType())) {
            QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QJsonObject>() == context->argument(0).toVariant().userType())) {
            QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QJsonValue::Type>() == context->argument(0).toVariant().userType())) {
            QJsonValue::Type _q_arg0 = qscriptvalue_cast<QJsonValue::Type>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (context->argument(0).isBoolean()) {
            bool _q_arg0 = context->argument(0).toBoolean();
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QJsonArray>() == context->argument(0).toVariant().userType())) {
            QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QJsonObject>() == context->argument(0).toVariant().userType())) {
            QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<QJsonValue>() == context->argument(0).toVariant().userType())) {
            QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (qscriptvalue_cast<const char*>(context->argument(0))) {
            QByteArray _q_arg0_ba = context->argument(0).toString().toLatin1();
            const char* _q_arg0 = _q_arg0_ba.constData();
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (context->argument(0).isNumber()) {
            double _q_arg0 = context->argument(0).toNumber();
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (context->argument(0).isNumber()) {
            int _q_arg0 = context->argument(0).toInt32();
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if ((qMetaTypeId<qint64>() == context->argument(0).toVariant().userType())) {
            qint64 _q_arg0 = qscriptvalue_cast<qint64>(context->argument(0));
            QJsonValue _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        }
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        QVariant _q_arg0 = context->argument(0).toVariant();
        QJsonValue _q_result = QJsonValue::fromVariant(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonValue_throw_ambiguity_error_helper(context,
        qtscript_QJsonValue_function_names[_id],
        qtscript_QJsonValue_function_signatures[_id]);
}

QScriptValue qtscript_create_QJsonValue_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QJsonValue*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QJsonValue*)0));
    for (int i = 0; i < 18; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonValue_prototype_call, qtscript_QJsonValue_function_lengths[i+2]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QJsonValue_function_names[i+2]),
            fun, QScriptValue::SkipInEnumeration);
    }

    engine->setDefaultPrototype(qMetaTypeId<QJsonValue>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QJsonValue*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QJsonValue_static_call, proto, qtscript_QJsonValue_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));
    for (int i = 0; i < 1; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonValue_static_call,
            qtscript_QJsonValue_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i+1)));
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonValue_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    ctor.setProperty(QString::fromLatin1("Type"),
        qtscript_create_QJsonValue_Type_class(engine, ctor));
    return ctor;
}
