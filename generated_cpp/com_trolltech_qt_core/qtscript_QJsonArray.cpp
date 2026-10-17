#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qjsonarray.h>
#include <QVariant>
#include <qjsonarray.h>
#include <qjsonvalue.h>

static const char * const qtscript_QJsonArray_function_names[] = {
    "QJsonArray"
    // static
    , "fromStringList"
    , "fromVariantList"
    // prototype
    , "append"
    , "at"
    , "contains"
    , "count"
    , "empty"
    , "first"
    , "insert"
    , "isEmpty"
    , "last"
    , "operator_add"
    , "operator_add_assign"
    , "operator_shift_left"
    , "operator_subscript"
    , "pop_back"
    , "pop_front"
    , "prepend"
    , "push_back"
    , "push_front"
    , "removeAt"
    , "removeFirst"
    , "removeLast"
    , "replace"
    , "size"
    , "swap"
    , "takeAt"
    , "toVariantList"
    , "toString"
};

static const char * const qtscript_QJsonArray_function_signatures[] = {
    "\nQJsonArray other"
    // static
    , "List list"
    , "List list"
    // prototype
    , "QJsonValue value"
    , "qsizetype i"
    , "QJsonValue element"
    , ""
    , ""
    , ""
    , "qsizetype i, QJsonValue value"
    , ""
    , ""
    , "QJsonValue v"
    , "QJsonValue v"
    , "QJsonValue v"
    , "qsizetype i"
    , ""
    , ""
    , "QJsonValue value"
    , "QJsonValue t"
    , "QJsonValue t"
    , "qsizetype i"
    , ""
    , ""
    , "qsizetype i, QJsonValue value"
    , ""
    , "QJsonArray other"
    , "qsizetype i"
    , ""
""
};

static const int qtscript_QJsonArray_function_lengths[] = {
    1
    // static
    , 1
    , 1
    // prototype
    , 1
    , 1
    , 1
    , 0
    , 0
    , 0
    , 2
    , 0
    , 0
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
    , 1
    , 1
    , 1
    , 1
    , 0
    , 0
    , 2
    , 0
    , 1
    , 1
    , 0
    , 0
};

static QScriptValue qtscript_QJsonArray_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QJsonArray::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QJsonArray*)
Q_DECLARE_METATYPE(qsizetype)

//
// QJsonArray
//

static QScriptValue qtscript_QJsonArray_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 26;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QJsonArray* _q_self = qscriptvalue_cast<QJsonArray*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QJsonArray.%0(): this object is not a QJsonArray")
            .arg(qtscript_QJsonArray_function_names[_id+3]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        _q_self->append(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        QJsonValue _q_result = _q_self->at(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        bool _q_result = _q_self->contains(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        qsizetype _q_result = _q_self->count();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->empty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        QJsonValue _q_result = _q_self->first();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 2) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        QJsonValue _q_arg1 = qscriptvalue_cast<QJsonValue>(context->argument(1));
        _q_self->insert(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isEmpty();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 8:
    if (context->argumentCount() == 0) {
        QJsonValue _q_result = _q_self->last();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 9:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        QJsonArray _q_result = _q_self->operator+(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        QJsonArray& _q_result = _q_self->operator+=(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        QJsonArray& _q_result = _q_self->operator<<(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 1) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        QJsonValue _q_result = _q_self->operator[](_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 13:
    if (context->argumentCount() == 0) {
        _q_self->pop_back();
        return context->engine()->undefinedValue();
    }
    break;

    case 14:
    if (context->argumentCount() == 0) {
        _q_self->pop_front();
        return context->engine()->undefinedValue();
    }
    break;

    case 15:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        _q_self->prepend(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 16:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        _q_self->push_back(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 17:
    if (context->argumentCount() == 1) {
        QJsonValue _q_arg0 = qscriptvalue_cast<QJsonValue>(context->argument(0));
        _q_self->push_front(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 18:
    if (context->argumentCount() == 1) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        _q_self->removeAt(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 19:
    if (context->argumentCount() == 0) {
        _q_self->removeFirst();
        return context->engine()->undefinedValue();
    }
    break;

    case 20:
    if (context->argumentCount() == 0) {
        _q_self->removeLast();
        return context->engine()->undefinedValue();
    }
    break;

    case 21:
    if (context->argumentCount() == 2) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        QJsonValue _q_arg1 = qscriptvalue_cast<QJsonValue>(context->argument(1));
        _q_self->replace(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 22:
    if (context->argumentCount() == 0) {
        qsizetype _q_result = _q_self->size();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 23:
    if (context->argumentCount() == 1) {
        QJsonArray* _q_arg0_ptr = qscriptvalue_cast<QJsonArray*>(context->argument(0));
        if (!_q_arg0_ptr)
            return context->throwError(QScriptContext::TypeError,
                QString::fromLatin1("QJsonArray.swap(): argument 1 must be a QJsonArray"));
        QJsonArray& _q_arg0 = *_q_arg0_ptr;
        _q_self->swap(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 24:
    if (context->argumentCount() == 1) {
        qsizetype _q_arg0 = qscriptvalue_cast<qsizetype>(context->argument(0));
        QJsonValue _q_result = _q_self->takeAt(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 25:
    if (context->argumentCount() == 0) {
        QList<QVariant > _q_result = _q_self->toVariantList();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 26: {
    QString result;
    QDebug d(&result);
    d << *_q_self;
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonArray_throw_ambiguity_error_helper(context,
        qtscript_QJsonArray_function_names[_id+3],
        qtscript_QJsonArray_function_signatures[_id+3]);
}

static QScriptValue qtscript_QJsonArray_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QJsonArray(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QJsonArray _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    } else if (context->argumentCount() == 1) {
        QJsonArray _q_arg0 = qscriptvalue_cast<QJsonArray>(context->argument(0));
        QJsonArray _q_cpp_result(_q_arg0);
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        QStringList _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        QJsonArray _q_result = QJsonArray::fromStringList(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        QList<QVariant> _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        QJsonArray _q_result = QJsonArray::fromVariantList(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QJsonArray_throw_ambiguity_error_helper(context,
        qtscript_QJsonArray_function_names[_id],
        qtscript_QJsonArray_function_signatures[_id]);
}

QScriptValue qtscript_create_QJsonArray_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QJsonArray*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QJsonArray*)0));
    for (int i = 0; i < 27; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonArray_prototype_call, qtscript_QJsonArray_function_lengths[i+3]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QJsonArray_function_names[i+3]),
            fun, QScriptValue::SkipInEnumeration);
    }

    engine->setDefaultPrototype(qMetaTypeId<QJsonArray>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QJsonArray*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QJsonArray_static_call, proto, qtscript_QJsonArray_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));
    for (int i = 0; i < 2; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QJsonArray_static_call,
            qtscript_QJsonArray_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i+1)));
        ctor.setProperty(QString::fromLatin1(qtscript_QJsonArray_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    return ctor;
}
