#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qmatrix4x4.h>
#include <QVariant>
#include <qgenericmatrix.h>
#include <qmatrix4x4.h>
#include <qpoint.h>
#include <qquaternion.h>
#include <qrect.h>
#include <qtransform.h>
#include <qvectornd.h>

static const char * const qtscript_QMatrix4x4_function_names[] = {
    "QMatrix4x4"
    // static
    // prototype
    , "column"
    , "constData"
    , "copyDataTo"
    , "data"
    , "determinant"
    , "fill"
    , "flags"
    , "flipCoordinates"
    , "frustum"
    , "inverted"
    , "isAffine"
    , "isIdentity"
    , "lookAt"
    , "map"
    , "mapRect"
    , "mapVector"
    , "normalMatrix"
    , "operator_add_assign"
    , "operator_cast_"
    , "operator_divide_assign"
    , "equals"
    , "operator_multiply_assign"
    , "operator_subtract_assign"
    , "optimize"
    , "ortho"
    , "perspective"
    , "projectedRotate"
    , "rotate"
    , "row"
    , "scale"
    , "setColumn"
    , "setRow"
    , "setToIdentity"
    , "toTransform"
    , "translate"
    , "transposed"
    , "viewport"
    , "toString"
};

static const char * const qtscript_QMatrix4x4_function_signatures[] = {
    "\nQTransform transform\nfloat values\nfloat values, int cols, int rows\nfloat m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44"
    // static
    // prototype
    , "int index"
    , ""
    , "float values"
    , ""
    , ""
    , "float value"
    , ""
    , ""
    , "float left, float right, float bottom, float top, float nearPlane, float farPlane"
    , "bool invertible"
    , ""
    , ""
    , "QVector3D eye, QVector3D center, QVector3D up"
    , "QPoint point\nQPointF point\nQVector3D point\nQVector4D point"
    , "QRect rect\nQRectF rect"
    , "QVector3D vector"
    , ""
    , "QMatrix4x4 other"
    , "int row, int column"
    , "float divisor"
    , "QMatrix4x4 other"
    , "QMatrix4x4 other\nfloat factor"
    , "QMatrix4x4 other"
    , ""
    , "QRect rect\nQRectF rect\nfloat left, float right, float bottom, float top, float nearPlane, float farPlane"
    , "float verticalAngle, float aspectRatio, float nearPlane, float farPlane"
    , "float angle, float x, float y, float z\nfloat angle, float x, float y, float z, float distanceToPlane"
    , "QQuaternion quaternion\nfloat angle, QVector3D vector\nfloat angle, float x, float y, float z"
    , "int index"
    , "QVector3D vector\nfloat factor\nfloat x, float y\nfloat x, float y, float z"
    , "int index, QVector4D value"
    , "int index, QVector4D value"
    , ""
    , "\nfloat distanceToPlane"
    , "QVector3D vector\nfloat x, float y\nfloat x, float y, float z"
    , ""
    , "QRectF rect\nfloat left, float bottom, float width, float height, float nearPlane, float farPlane"
""
};

static const int qtscript_QMatrix4x4_function_lengths[] = {
    16
    // static
    // prototype
    , 1
    , 0
    , 1
    , 0
    , 0
    , 1
    , 0
    , 0
    , 6
    , 1
    , 0
    , 0
    , 3
    , 1
    , 1
    , 1
    , 0
    , 1
    , 2
    , 1
    , 1
    , 1
    , 1
    , 0
    , 6
    , 4
    , 5
    , 4
    , 1
    , 3
    , 2
    , 2
    , 0
    , 1
    , 3
    , 0
    , 6
    , 0
};

static QScriptValue qtscript_QMatrix4x4_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QMatrix4x4::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QMatrix4x4)
Q_DECLARE_METATYPE(QMatrix4x4*)
Q_DECLARE_METATYPE(QMatrix4x4::Flag)
Q_DECLARE_METATYPE(QFlags<QMatrix4x4::Flag>)
Q_DECLARE_METATYPE(QVector4D)
Q_DECLARE_METATYPE(const float*)
Q_DECLARE_METATYPE(float*)
Q_DECLARE_METATYPE(bool*)
Q_DECLARE_METATYPE(QVector3D)
Q_DECLARE_METATYPE(QQuaternion)
Q_DECLARE_METATYPE(QTransform)

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
// QMatrix4x4::Flag
//

static const QMatrix4x4::Flag qtscript_QMatrix4x4_Flag_values[] = {
    QMatrix4x4::Identity
    , QMatrix4x4::Translation
    , QMatrix4x4::Scale
    , QMatrix4x4::Rotation2D
    , QMatrix4x4::Rotation
    , QMatrix4x4::Perspective
    , QMatrix4x4::General
};

static const char * const qtscript_QMatrix4x4_Flag_keys[] = {
    "Identity"
    , "Translation"
    , "Scale"
    , "Rotation2D"
    , "Rotation"
    , "Perspective"
    , "General"
};

static QString qtscript_QMatrix4x4_Flag_toStringHelper(QMatrix4x4::Flag value)
{
    for (int i = 0; i < 7; ++i) {
        if (qtscript_QMatrix4x4_Flag_values[i] == value)
            return QString::fromLatin1(qtscript_QMatrix4x4_Flag_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_QMatrix4x4_Flag_toScriptValue(QScriptEngine *engine, const QMatrix4x4::Flag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QMatrix4x4"));
    return clazz.property(qtscript_QMatrix4x4_Flag_toStringHelper(value));
}

static void qtscript_QMatrix4x4_Flag_fromScriptValue(const QScriptValue &value, QMatrix4x4::Flag &out)
{
    out = qvariant_cast<QMatrix4x4::Flag>(value.toVariant());
}

static QScriptValue qtscript_construct_QMatrix4x4_Flag(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QMatrix4x4::Flag>())
        arg = static_cast<int>(qvariant_cast<QMatrix4x4::Flag>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("Flag(): invalid enum value"));
    for (int i = 0; i < 7; ++i) {
        if (qtscript_QMatrix4x4_Flag_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<QMatrix4x4::Flag>(arg));
    }
    return context->throwError(QString::fromLatin1("Flag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QMatrix4x4_Flag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QMatrix4x4::Flag value = qscriptvalue_cast<QMatrix4x4::Flag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QMatrix4x4_Flag_toString(QScriptContext *context, QScriptEngine *engine)
{
    QMatrix4x4::Flag value = qscriptvalue_cast<QMatrix4x4::Flag>(context->thisObject());
    return QScriptValue(engine, qtscript_QMatrix4x4_Flag_toStringHelper(value));
}

static QScriptValue qtscript_create_QMatrix4x4_Flag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QMatrix4x4_Flag,
        qtscript_QMatrix4x4_Flag_valueOf, qtscript_QMatrix4x4_Flag_toString);
    qScriptRegisterMetaType<QMatrix4x4::Flag>(engine, qtscript_QMatrix4x4_Flag_toScriptValue,
        qtscript_QMatrix4x4_Flag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 7; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QMatrix4x4_Flag_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QMatrix4x4_Flag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QMatrix4x4_Flag_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QMatrix4x4::Flags
//

static QScriptValue qtscript_QMatrix4x4_Flags_toScriptValue(QScriptEngine *engine, const QMatrix4x4::Flags &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_QMatrix4x4_Flags_fromScriptValue(const QScriptValue &value, QMatrix4x4::Flags &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<QMatrix4x4::Flags>())
        out = qvariant_cast<QMatrix4x4::Flags>(var);
    else if (var.userType() == qMetaTypeId<QMatrix4x4::Flag>())
        out = qvariant_cast<QMatrix4x4::Flag>(var);
    else
        out = {};
}

static QScriptValue qtscript_construct_QMatrix4x4_Flags(QScriptContext *context, QScriptEngine *engine)
{
    QMatrix4x4::Flags result{};
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<QMatrix4x4::Flags>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<QMatrix4x4::Flag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("Flags(): argument %0 is not of type Flag").arg(i));
            }
            result |= qvariant_cast<QMatrix4x4::Flag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_QMatrix4x4_Flags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QMatrix4x4::Flags value = qscriptvalue_cast<QMatrix4x4::Flags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QMatrix4x4_Flags_toString(QScriptContext *context, QScriptEngine *engine)
{
    QMatrix4x4::Flags value = qscriptvalue_cast<QMatrix4x4::Flags>(context->thisObject());
    QString result;
    for (int i = 0; i < 7; ++i) {
        if ((value & qtscript_QMatrix4x4_Flag_values[i]) == qtscript_QMatrix4x4_Flag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_QMatrix4x4_Flag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_QMatrix4x4_Flags_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<QMatrix4x4::Flags>() == otherObj.value<QMatrix4x4::Flags>())));
}

static QScriptValue qtscript_create_QMatrix4x4_Flags_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_QMatrix4x4_Flags, qtscript_QMatrix4x4_Flags_valueOf,
        qtscript_QMatrix4x4_Flags_toString, qtscript_QMatrix4x4_Flags_equals);
    qScriptRegisterMetaType<QMatrix4x4::Flags>(engine, qtscript_QMatrix4x4_Flags_toScriptValue,
        qtscript_QMatrix4x4_Flags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}

//
// QMatrix4x4
//

static QScriptValue qtscript_QMatrix4x4_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 37;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QMatrix4x4* _q_self = qscriptvalue_cast<QMatrix4x4*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QMatrix4x4.%0(): this object is not a QMatrix4x4")
            .arg(qtscript_QMatrix4x4_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QVector4D _q_result = _q_self->column(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        const float* _q_result = const_cast<const float*>(_q_self->constData());
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 1) {
        float* _q_arg0 = qscriptvalue_cast<float*>(context->argument(0));
        _q_self->copyDataTo(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        float* _q_result = _q_self->data();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        double _q_result = _q_self->determinant();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 1) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        _q_self->fill(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 6:
    if (context->argumentCount() == 0) {
        QFlags<QMatrix4x4::Flag> _q_result = _q_self->flags();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        _q_self->flipCoordinates();
        return context->engine()->undefinedValue();
    }
    break;

    case 8:
    if (context->argumentCount() == 6) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        float _q_arg5 = qscriptvalue_cast<float>(context->argument(5));
        _q_self->frustum(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4, _q_arg5);
        return context->engine()->undefinedValue();
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        QMatrix4x4 _q_result = _q_self->inverted();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        bool* _q_arg0 = qscriptvalue_cast<bool*>(context->argument(0));
        QMatrix4x4 _q_result = _q_self->inverted(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isAffine();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isIdentity();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 3) {
        QVector3D _q_arg0 = qscriptvalue_cast<QVector3D>(context->argument(0));
        QVector3D _q_arg1 = qscriptvalue_cast<QVector3D>(context->argument(1));
        QVector3D _q_arg2 = qscriptvalue_cast<QVector3D>(context->argument(2));
        _q_self->lookAt(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    break;

    case 13:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QPoint>() == context->argument(0).toVariant().userType())) {
            QPoint _q_arg0 = qscriptvalue_cast<QPoint>(context->argument(0));
            QPoint _q_result = _q_self->map(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((qMetaTypeId<QPointF>() == context->argument(0).toVariant().userType())) {
            QPointF _q_arg0 = qscriptvalue_cast<QPointF>(context->argument(0));
            QPointF _q_result = _q_self->map(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((qMetaTypeId<QVector3D>() == context->argument(0).toVariant().userType())) {
            QVector3D _q_arg0 = qscriptvalue_cast<QVector3D>(context->argument(0));
            QVector3D _q_result = _q_self->map(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((qMetaTypeId<QVector4D>() == context->argument(0).toVariant().userType())) {
            QVector4D _q_arg0 = qscriptvalue_cast<QVector4D>(context->argument(0));
            QVector4D _q_result = _q_self->map(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 14:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QRect>() == context->argument(0).toVariant().userType())) {
            QRect _q_arg0 = qscriptvalue_cast<QRect>(context->argument(0));
            QRect _q_result = _q_self->mapRect(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if ((qMetaTypeId<QRectF>() == context->argument(0).toVariant().userType())) {
            QRectF _q_arg0 = qscriptvalue_cast<QRectF>(context->argument(0));
            QRectF _q_result = _q_self->mapRect(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 15:
    if (context->argumentCount() == 1) {
        QVector3D _q_arg0 = qscriptvalue_cast<QVector3D>(context->argument(0));
        QVector3D _q_result = _q_self->mapVector(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 16:
    if (context->argumentCount() == 0) {
        QMatrix3x3 _q_result = _q_self->normalMatrix();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 17:
    if (context->argumentCount() == 1) {
        QMatrix4x4 _q_arg0 = qscriptvalue_cast<QMatrix4x4>(context->argument(0));
        QMatrix4x4& _q_result = _q_self->operator+=(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 18:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        int _q_arg1 = context->argument(1).toInt32();
        float& _q_result = _q_self->operator()(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 19:
    if (context->argumentCount() == 1) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        QMatrix4x4& _q_result = _q_self->operator/=(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 20:
    if (context->argumentCount() == 1) {
        QMatrix4x4 _q_arg0 = qscriptvalue_cast<QMatrix4x4>(context->argument(0));
        bool _q_result = _q_self->operator==(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 21:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QMatrix4x4>() == context->argument(0).toVariant().userType())) {
            QMatrix4x4 _q_arg0 = qscriptvalue_cast<QMatrix4x4>(context->argument(0));
            QMatrix4x4& _q_result = _q_self->operator*=(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        } else if (context->argument(0).isNumber()) {
            float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
            QMatrix4x4& _q_result = _q_self->operator*=(_q_arg0);
            return qScriptValueFromValue(context->engine(), _q_result);
        }
    }
    break;

    case 22:
    if (context->argumentCount() == 1) {
        QMatrix4x4 _q_arg0 = qscriptvalue_cast<QMatrix4x4>(context->argument(0));
        QMatrix4x4& _q_result = _q_self->operator-=(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 23:
    if (context->argumentCount() == 0) {
        _q_self->optimize();
        return context->engine()->undefinedValue();
    }
    break;

    case 24:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QRect>() == context->argument(0).toVariant().userType())) {
            QRect _q_arg0 = qscriptvalue_cast<QRect>(context->argument(0));
            _q_self->ortho(_q_arg0);
            return context->engine()->undefinedValue();
        } else if ((qMetaTypeId<QRectF>() == context->argument(0).toVariant().userType())) {
            QRectF _q_arg0 = qscriptvalue_cast<QRectF>(context->argument(0));
            _q_self->ortho(_q_arg0);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 6) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        float _q_arg5 = qscriptvalue_cast<float>(context->argument(5));
        _q_self->ortho(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4, _q_arg5);
        return context->engine()->undefinedValue();
    }
    break;

    case 25:
    if (context->argumentCount() == 4) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        _q_self->perspective(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
        return context->engine()->undefinedValue();
    }
    break;

    case 26:
    if (context->argumentCount() == 4) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        _q_self->projectedRotate(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 5) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        _q_self->projectedRotate(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4);
        return context->engine()->undefinedValue();
    }
    break;

    case 27:
    if (context->argumentCount() == 1) {
        QQuaternion _q_arg0 = qscriptvalue_cast<QQuaternion>(context->argument(0));
        _q_self->rotate(_q_arg0);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 2) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        QVector3D _q_arg1 = qscriptvalue_cast<QVector3D>(context->argument(1));
        _q_self->rotate(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 3) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        _q_self->rotate(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 4) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        _q_self->rotate(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
        return context->engine()->undefinedValue();
    }
    break;

    case 28:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        QVector4D _q_result = _q_self->row(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 29:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QVector3D>() == context->argument(0).toVariant().userType())) {
            QVector3D _q_arg0 = qscriptvalue_cast<QVector3D>(context->argument(0));
            _q_self->scale(_q_arg0);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isNumber()) {
            float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
            _q_self->scale(_q_arg0);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 2) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        _q_self->scale(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 3) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        _q_self->scale(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    break;

    case 30:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        QVector4D _q_arg1 = qscriptvalue_cast<QVector4D>(context->argument(1));
        _q_self->setColumn(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 31:
    if (context->argumentCount() == 2) {
        int _q_arg0 = context->argument(0).toInt32();
        QVector4D _q_arg1 = qscriptvalue_cast<QVector4D>(context->argument(1));
        _q_self->setRow(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 32:
    if (context->argumentCount() == 0) {
        _q_self->setToIdentity();
        return context->engine()->undefinedValue();
    }
    break;

    case 33:
    if (context->argumentCount() == 0) {
        QTransform _q_result = _q_self->toTransform();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        QTransform _q_result = _q_self->toTransform(_q_arg0);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 34:
    if (context->argumentCount() == 1) {
        QVector3D _q_arg0 = qscriptvalue_cast<QVector3D>(context->argument(0));
        _q_self->translate(_q_arg0);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 2) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        _q_self->translate(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 3) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        _q_self->translate(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    break;

    case 35:
    if (context->argumentCount() == 0) {
        QMatrix4x4 _q_result = _q_self->transposed();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 36:
    if (context->argumentCount() == 1) {
        QRectF _q_arg0 = qscriptvalue_cast<QRectF>(context->argument(0));
        _q_self->viewport(_q_arg0);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 4) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        _q_self->viewport(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 5) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        _q_self->viewport(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 6) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        float _q_arg5 = qscriptvalue_cast<float>(context->argument(5));
        _q_self->viewport(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4, _q_arg5);
        return context->engine()->undefinedValue();
    }
    break;

    case 37: {
    QString result;
    QDebug d(&result);
    d << *_q_self;
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QMatrix4x4_throw_ambiguity_error_helper(context,
        qtscript_QMatrix4x4_function_names[_id+1],
        qtscript_QMatrix4x4_function_signatures[_id+1]);
}

static QScriptValue qtscript_QMatrix4x4_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QMatrix4x4(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QMatrix4x4 _q_cpp_result;
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QTransform>() == context->argument(0).toVariant().userType())) {
            QTransform _q_arg0 = qscriptvalue_cast<QTransform>(context->argument(0));
            QMatrix4x4 _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        } else if (qscriptvalue_cast<const float*>(context->argument(0))) {
            const float* _q_arg0 = qscriptvalue_cast<const float*>(context->argument(0));
            QMatrix4x4 _q_cpp_result(_q_arg0);
            QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
            return _q_result;
        }
    } else if (context->argumentCount() == 3) {
        const float* _q_arg0 = qscriptvalue_cast<const float*>(context->argument(0));
        int _q_arg1 = context->argument(1).toInt32();
        int _q_arg2 = context->argument(2).toInt32();
        QMatrix4x4 _q_cpp_result(_q_arg0, _q_arg1, _q_arg2);
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    } else if (context->argumentCount() == 16) {
        float _q_arg0 = qscriptvalue_cast<float>(context->argument(0));
        float _q_arg1 = qscriptvalue_cast<float>(context->argument(1));
        float _q_arg2 = qscriptvalue_cast<float>(context->argument(2));
        float _q_arg3 = qscriptvalue_cast<float>(context->argument(3));
        float _q_arg4 = qscriptvalue_cast<float>(context->argument(4));
        float _q_arg5 = qscriptvalue_cast<float>(context->argument(5));
        float _q_arg6 = qscriptvalue_cast<float>(context->argument(6));
        float _q_arg7 = qscriptvalue_cast<float>(context->argument(7));
        float _q_arg8 = qscriptvalue_cast<float>(context->argument(8));
        float _q_arg9 = qscriptvalue_cast<float>(context->argument(9));
        float _q_arg10 = qscriptvalue_cast<float>(context->argument(10));
        float _q_arg11 = qscriptvalue_cast<float>(context->argument(11));
        float _q_arg12 = qscriptvalue_cast<float>(context->argument(12));
        float _q_arg13 = qscriptvalue_cast<float>(context->argument(13));
        float _q_arg14 = qscriptvalue_cast<float>(context->argument(14));
        float _q_arg15 = qscriptvalue_cast<float>(context->argument(15));
        QMatrix4x4 _q_cpp_result(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4, _q_arg5, _q_arg6, _q_arg7, _q_arg8, _q_arg9, _q_arg10, _q_arg11, _q_arg12, _q_arg13, _q_arg14, _q_arg15);
        QScriptValue _q_result = context->engine()->newVariant(context->thisObject(), qVariantFromValue(_q_cpp_result));
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QMatrix4x4_throw_ambiguity_error_helper(context,
        qtscript_QMatrix4x4_function_names[_id],
        qtscript_QMatrix4x4_function_signatures[_id]);
}

QScriptValue qtscript_create_QMatrix4x4_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QMatrix4x4*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QMatrix4x4*)0));
    for (int i = 0; i < 38; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QMatrix4x4_prototype_call, qtscript_QMatrix4x4_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QMatrix4x4_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    engine->setDefaultPrototype(qMetaTypeId<QMatrix4x4>(), proto);
    engine->setDefaultPrototype(qMetaTypeId<QMatrix4x4*>(), proto);

    QScriptValue ctor = engine->newFunction(qtscript_QMatrix4x4_static_call, proto, qtscript_QMatrix4x4_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("Flag"),
        qtscript_create_QMatrix4x4_Flag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Flags"),
        qtscript_create_QMatrix4x4_Flags_class(engine));
    return ctor;
}
