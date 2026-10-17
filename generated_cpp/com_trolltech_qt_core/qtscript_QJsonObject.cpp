#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qjsonobject.h>
#include <QVariant>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qstringview.h>

static const char * const qtscript_QJsonObject_function_names[] = {
    "QJsonObject"
    // static
    , "fromVariantHash"
    , "fromVariantMap"
    // prototype
    , "contains"
    , "count"
    , "empty"
    , "isEmpty"
    , "keys"
    , "length"
    , "operator_subscript"
    , "remove"
    , "size"
    , "swap"
    , "take"
    , "toVariantHash"
    , "toVariantMap"
    , "value"
    , "toString"
};

static const char * const qtscript_QJsonObject_function_signatures[] = {
    "\nQJsonObject other"
    // static
    , "HashMap map"
    , "SortedMap map"
    // prototype
    , "String key\nQStringView key\nString key"
    , ""
    , ""
    , ""
    , ""
    , ""
    , "String key\nQStringView key\nString key"
    , "String key\nQStringView key\nString key"
    , ""
    , "QJsonObject other"
    , "String key\nQStringView key\nString key"
    , ""
    , ""
    , "String key\nQStringView key\nString key"
""
};

static const int qtscript_QJsonObject_function_lengths[] = {
    1
    // static
    , 1
    , 1
    // prototype
    , 1
    , 0
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
    , 0
    , 1
    , 1
    , 0
    , 0
    , 1
    , 0
};

static QScriptValue qtscript_QJsonObject_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QJsonObject::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QJsonObject*)
Q_DECLARE_METATYPE(QLatin1String)
Q_DECLARE_METATYPE(QStringView)
Q_DECLARE_METATYPE(qsizetype)

//
// QJsonObject
//

static QScriptValue qtscript_QJsonObject_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 14;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QJsonObject* _q_self = qscriptvalue_cast<QJsonObject*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QJsonObject.%0(): this object is not a QJsonObject")
            .arg(qtscript_QJsonObject_function_names[_id+3]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            bool _q_result = _q_self->contains(_q_arg0);
            return QScriptValue(context->engine(), _q_result);
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            bool _q_result = _q_self->contains(_q_arg0);
            return QScriptValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            bool _q_result = _q_self->contains(_q_arg0);
            return QScriptValue(context->engine(), _q_result);
        }
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        qsizetype _q_result = _q_self->count();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->empty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isEmpty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        QStringList _q_result = _q_self->keys();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        qsizetype _q_result = _q_self->length();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QJsonValue _q_result = _q_self->operator[](_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 7:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            _q_self->remove(_q_arg0);
            return context->engine()->undefinedValue();
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            _q_self->remove(_q_arg0);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            _q_self->remove(_q_arg0);
            return context->engine()->undefinedValue();
        }
    }
    break;

    case 8:
    if (context->argumentCount() == 0) {
        qsizetype _q_result = _q_self->size();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 9:
    if (context->argumentCount() == 1) {
        QJsonObject* _q_arg0_ptr = qscriptvalue_cast<QJsonObject*>(context->argument(0));
        if (!_q_arg0_ptr)
            return context->throwError(QScriptContext::TypeError,
                QString::fromLatin1("QJsonObject.swap(): argument 1 must be a QJsonObject"));
        QJsonObject& _q_arg0 = *_q_arg0_ptr;
        _q_self->swap(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 10:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            QJsonValue _q_result = _q_self->take(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            QJsonValue _q_result = _q_self->take(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QJsonValue _q_result = _q_self->take(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 11:
    if (context->argumentCount() == 0) {
        QHash<QString,QVariant > _q_result = _q_self->toVariantHash();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 0) {
        QMap<QString,QVariant > _q_result = _q_self->toVariantMap();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 13:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QLatin1String>() == context->argument(0).toVariant().userType())) {
            QLatin1String _q_arg0 = qscriptvalue_cast<QLatin1String>(context->argument(0));
            QJsonValue _q_result = _q_self->value(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((context->argument(0).isString() || context->argument(0).isVariant())) {
            QString _q_arg0_str = context->argument(0).toString();
            QStringView _q_arg0(_q_arg0_str);
            QJsonValue _q_result = _q_self->value(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            QJsonValue _q_result = _q_self->value(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 14: {
    QString result;
    QDebug d(&result);
    d << *_q_self;
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonObject_throw_ambiguity_error_helper(context,
        qtscript_QJsonObject_function_names[_id+3],
        qtscript_QJsonObject_function_signatures[_id+3]);
}

static QScriptValue qtscript_QJsonObject_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QJsonObject(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QJsonObject _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    } else if (context->argumentCount() == 1) {
        QJsonObject _q_arg0 = qscriptvalue_cast<QJsonObject>(context->argument(0));
        QJsonObject _q_cpp_result(_q_arg0);
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        QHash<QString,QVariant> _q_arg0 = qscriptvalue_cast<QHash<QString,QVariant> >(context->argument(0));
        QJsonObject _q_result = QJsonObject::fromVariantHash(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        QMap<QString,QVariant> _q_arg0 = qscriptvalue_cast<QMap<QString,QVariant> >(context->argument(0));
        QJsonObject _q_result = QJsonObject::fromVariantMap(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonObject_throw_ambiguity_error_helper(context,
        qtscript_QJsonObject_function_names[_id],
        qtscript_QJsonObject_function_signatures[_id]);
}

QScriptValue qtscript_create_QJsonObject_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QJsonObject*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QJsonObject*)0));
    for (int i = 0; i < 15; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonObject_prototype_call, qtscript_QJsonObject_function_lengths[i+3]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QJsonObject_function_names[i+3]),
            fun, QScriptValue::SkipInEnumeration);
    }

    {
        QScriptValue insertFun = engine->newFunction(
            [](QScriptContext *ctx, QScriptEngine *eng) -> QScriptValue {
                QJsonObject *self = qscriptvalue_cast<QJsonObject*>(ctx->thisObject());
                if (!self)
                    return ctx->throwError(QScriptContext::TypeError,
                        QString::fromLatin1("QJsonObject.insert(): expected a QJsonObject"));
                if (ctx->argumentCount() != 2 || !(ctx->argument(0).isString()))
                    return ctx->throwError(QScriptContext::TypeError,
                        QString::fromLatin1("QJsonObject.insert(): expected a key string and a QJsonValue"));
                const QString key = ctx->argument(0).toString();
                QJsonValue value = qscriptvalue_cast<QJsonValue>(ctx->argument(1));
                self->insert(key, value);
                return eng->undefinedValue();
            }, 2);
        proto.setProperty(QString::fromLatin1("insert"), insertFun,
            QScriptValue::SkipInEnumeration);
    }

    engine->setDefaultPrototype(qMetaTypeId<QJsonObject>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QJsonObject*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QJsonObject_static_call, proto, qtscript_QJsonObject_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));
    for (int i = 0; i < 2; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonObject_static_call,
            qtscript_QJsonObject_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i+1)));
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonObject_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    return ctor;
}
