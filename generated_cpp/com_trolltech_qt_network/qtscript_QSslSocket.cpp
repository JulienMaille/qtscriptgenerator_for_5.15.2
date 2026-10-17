#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>
#include <__package_shared.h>

#include <qsslsocket.h>
#include <QVariant>
#include <qanystringview.h>
#include <qauthenticator.h>
#include <qbytearray.h>
#include <qcoreevent.h>
#include <qhostaddress.h>
#include <qnetworkproxy.h>
#include <qobject.h>
#include <qsslcertificate.h>
#include <qsslcipher.h>
#include <qsslconfiguration.h>
#include <qsslerror.h>
#include <qsslkey.h>

#include "qtscriptshell_QSslSocket.h"

static const char * const qtscript_QSslSocket_function_names[] = {
    "QSslSocket"
    // static
    , "activeBackend"
    , "availableBackends"
    , "isProtocolSupported"
    , "setActiveBackend"
    , "sslLibraryBuildVersionNumber"
    , "sslLibraryBuildVersionString"
    , "sslLibraryVersionNumber"
    , "sslLibraryVersionString"
    , "supportedProtocols"
    , "supportsSsl"
    // prototype
    , "connectToHostEncrypted"
    , "continueInterruptedHandshake"
    , "encrypted"
    , "encryptedBytesAvailable"
    , "encryptedBytesToWrite"
    , "encryptedBytesWritten"
    , "handshakeInterruptedOnError"
    , "ignoreSslErrors"
    , "isEncrypted"
    , "localCertificate"
    , "localCertificateChain"
    , "mode"
    , "modeChanged"
    , "newSessionTicketReceived"
    , "peerCertificate"
    , "peerCertificateChain"
    , "peerVerifyDepth"
    , "peerVerifyError"
    , "peerVerifyMode"
    , "peerVerifyName"
    , "privateKey"
    , "protocol"
    , "sessionCipher"
    , "sessionProtocol"
    , "setLocalCertificate"
    , "setLocalCertificateChain"
    , "setPeerVerifyDepth"
    , "setPeerVerifyMode"
    , "setPeerVerifyName"
    , "setPrivateKey"
    , "setProtocol"
    , "setSslConfiguration"
    , "sslConfiguration"
    , "sslErrors"
    , "sslHandshakeErrors"
    , "startClientEncryption"
    , "startServerEncryption"
    , "waitForEncrypted"
    , "toString"
};

static const char * const qtscript_QSslSocket_function_signatures[] = {
    "QObject parent"
    // static
    , ""
    , ""
    , "SslProtocol protocol, String backendName"
    , "String backendName"
    , ""
    , ""
    , ""
    , ""
    , "String backendName"
    , ""
    // prototype
    , "String hostName, quint16 port, OpenMode mode, NetworkLayerProtocol protocol\nString hostName, quint16 port, String sslPeerName, OpenMode mode, NetworkLayerProtocol protocol"
    , ""
    , ""
    , ""
    , ""
    , "qint64 totalBytes"
    , "QSslError error"
    , "\nList errors"
    , ""
    , ""
    , ""
    , ""
    , "SslMode newMode"
    , ""
    , ""
    , ""
    , ""
    , "QSslError error"
    , ""
    , ""
    , ""
    , ""
    , ""
    , ""
    , "QSslCertificate certificate\nString fileName, EncodingFormat format"
    , "List localChain"
    , "int depth"
    , "PeerVerifyMode mode"
    , "String hostName"
    , "QSslKey key\nString fileName, KeyAlgorithm algorithm, EncodingFormat format, QByteArray passPhrase"
    , "SslProtocol protocol"
    , "QSslConfiguration config"
    , ""
    , "List errors"
    , ""
    , ""
    , ""
    , "int msecs"
""
};

static const int qtscript_QSslSocket_function_lengths[] = {
    1
    // static
    , 0
    , 0
    , 2
    , 1
    , 0
    , 0
    , 0
    , 0
    , 1
    , 0
    // prototype
    , 5
    , 0
    , 0
    , 0
    , 0
    , 1
    , 1
    , 1
    , 0
    , 0
    , 0
    , 0
    , 1
    , 0
    , 0
    , 0
    , 0
    , 1
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 2
    , 1
    , 1
    , 1
    , 1
    , 4
    , 1
    , 1
    , 0
    , 1
    , 0
    , 0
    , 0
    , 1
    , 0
};

static QScriptValue qtscript_QSslSocket_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QSslSocket::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QSslSocket*)
Q_DECLARE_METATYPE(QtScriptShell_QSslSocket*)
Q_DECLARE_METATYPE(QSslSocket::PeerVerifyMode)
Q_DECLARE_METATYPE(QSslSocket::SslMode)
Q_DECLARE_METATYPE(QFlags<QIODevice::OpenModeFlag>)
Q_DECLARE_METATYPE(QAbstractSocket::NetworkLayerProtocol)
Q_DECLARE_METATYPE(QSslError)
Q_DECLARE_METATYPE(QList<QSslCertificate >)
Q_DECLARE_METATYPE(QSslKey)
Q_DECLARE_METATYPE(QSsl::SslProtocol)
Q_DECLARE_METATYPE(QSslCipher)
Q_DECLARE_METATYPE(QSsl::EncodingFormat)
Q_DECLARE_METATYPE(QSsl::KeyAlgorithm)
Q_DECLARE_METATYPE(QList<QSsl::SslProtocol >)
Q_DECLARE_METATYPE(QTcpSocket*)

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
// QSslSocket::PeerVerifyMode
//

static const QSslSocket::PeerVerifyMode qtscript_QSslSocket_PeerVerifyMode_values[] = {
    QSslSocket::VerifyNone
    , QSslSocket::QueryPeer
    , QSslSocket::VerifyPeer
    , QSslSocket::AutoVerifyPeer
};

static const char * const qtscript_QSslSocket_PeerVerifyMode_keys[] = {
    "VerifyNone"
    , "QueryPeer"
    , "VerifyPeer"
    , "AutoVerifyPeer"
};

static QString qtscript_QSslSocket_PeerVerifyMode_toStringHelper(QSslSocket::PeerVerifyMode value)
{
    if ((value >= QSslSocket::VerifyNone) && (value <= QSslSocket::AutoVerifyPeer))
        return qtscript_QSslSocket_PeerVerifyMode_keys[static_cast<int>(value)-static_cast<int>(QSslSocket::VerifyNone)];
    return QString();
}

static QScriptValue qtscript_QSslSocket_PeerVerifyMode_toScriptValue(QScriptEngine *engine, const QSslSocket::PeerVerifyMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QSslSocket"));
    return clazz.property(qtscript_QSslSocket_PeerVerifyMode_toStringHelper(value));
}

static void qtscript_QSslSocket_PeerVerifyMode_fromScriptValue(const QScriptValue &value, QSslSocket::PeerVerifyMode &out)
{
    out = qvariant_cast<QSslSocket::PeerVerifyMode>(value.toVariant());
}

static QScriptValue qtscript_construct_QSslSocket_PeerVerifyMode(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QSslSocket::PeerVerifyMode>())
        arg = static_cast<int>(qvariant_cast<QSslSocket::PeerVerifyMode>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("PeerVerifyMode(): invalid enum value"));
    if ((arg >= QSslSocket::VerifyNone) && (arg <= QSslSocket::AutoVerifyPeer))
        return qScriptValueFromValue(engine,  static_cast<QSslSocket::PeerVerifyMode>(arg));
    return context->throwError(QString::fromLatin1("PeerVerifyMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QSslSocket_PeerVerifyMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QSslSocket::PeerVerifyMode value = qscriptvalue_cast<QSslSocket::PeerVerifyMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QSslSocket_PeerVerifyMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    QSslSocket::PeerVerifyMode value = qscriptvalue_cast<QSslSocket::PeerVerifyMode>(context->thisObject());
    return QScriptValue(engine, qtscript_QSslSocket_PeerVerifyMode_toStringHelper(value));
}

static QScriptValue qtscript_create_QSslSocket_PeerVerifyMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QSslSocket_PeerVerifyMode,
        qtscript_QSslSocket_PeerVerifyMode_valueOf, qtscript_QSslSocket_PeerVerifyMode_toString);
    qScriptRegisterMetaType<QSslSocket::PeerVerifyMode>(engine, qtscript_QSslSocket_PeerVerifyMode_toScriptValue,
        qtscript_QSslSocket_PeerVerifyMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QSslSocket_PeerVerifyMode_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QSslSocket_PeerVerifyMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QSslSocket_PeerVerifyMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QSslSocket::SslMode
//

static const QSslSocket::SslMode qtscript_QSslSocket_SslMode_values[] = {
    QSslSocket::UnencryptedMode
    , QSslSocket::SslClientMode
    , QSslSocket::SslServerMode
};

static const char * const qtscript_QSslSocket_SslMode_keys[] = {
    "UnencryptedMode"
    , "SslClientMode"
    , "SslServerMode"
};

static QString qtscript_QSslSocket_SslMode_toStringHelper(QSslSocket::SslMode value)
{
    if ((value >= QSslSocket::UnencryptedMode) && (value <= QSslSocket::SslServerMode))
        return qtscript_QSslSocket_SslMode_keys[static_cast<int>(value)-static_cast<int>(QSslSocket::UnencryptedMode)];
    return QString();
}

static QScriptValue qtscript_QSslSocket_SslMode_toScriptValue(QScriptEngine *engine, const QSslSocket::SslMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("QSslSocket"));
    return clazz.property(qtscript_QSslSocket_SslMode_toStringHelper(value));
}

static void qtscript_QSslSocket_SslMode_fromScriptValue(const QScriptValue &value, QSslSocket::SslMode &out)
{
    out = qvariant_cast<QSslSocket::SslMode>(value.toVariant());
}

static QScriptValue qtscript_construct_QSslSocket_SslMode(QScriptContext *context, QScriptEngine *engine)
{
    const QScriptValue _q_arg = context->argument(0);
    int arg = 0;
    if (_q_arg.isNumber())
        arg = _q_arg.toInt32();
    else if (_q_arg.isVariant()
             && _q_arg.toVariant().userType() == qMetaTypeId<QSslSocket::SslMode>())
        arg = static_cast<int>(qvariant_cast<QSslSocket::SslMode>(_q_arg.toVariant()));
    else
        return context->throwError(QString::fromLatin1("SslMode(): invalid enum value"));
    if ((arg >= QSslSocket::UnencryptedMode) && (arg <= QSslSocket::SslServerMode))
        return qScriptValueFromValue(engine,  static_cast<QSslSocket::SslMode>(arg));
    return context->throwError(QString::fromLatin1("SslMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_QSslSocket_SslMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    QSslSocket::SslMode value = qscriptvalue_cast<QSslSocket::SslMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_QSslSocket_SslMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    QSslSocket::SslMode value = qscriptvalue_cast<QSslSocket::SslMode>(context->thisObject());
    return QScriptValue(engine, qtscript_QSslSocket_SslMode_toStringHelper(value));
}

static QScriptValue qtscript_create_QSslSocket_SslMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_QSslSocket_SslMode,
        qtscript_QSslSocket_SslMode_valueOf, qtscript_QSslSocket_SslMode_toString);
    qScriptRegisterMetaType<QSslSocket::SslMode>(engine, qtscript_QSslSocket_SslMode_toScriptValue,
        qtscript_QSslSocket_SslMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        QScriptValue ev = engine->newVariant(qVariantFromValue(qtscript_QSslSocket_SslMode_values[i]));
        clazz.setProperty(QString::fromLatin1(qtscript_QSslSocket_SslMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
        ctor.setProperty(QString::fromLatin1(qtscript_QSslSocket_SslMode_keys[i]), ev,
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// QSslSocket
//

static QScriptValue qtscript_QSslSocket_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 38;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QSslSocket* _q_self = qscriptvalue_cast<QSslSocket*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QSslSocket.%0(): this object is not a QSslSocket")
            .arg(qtscript_QSslSocket_function_names[_id+11]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 2) {
        QString _q_arg0 = context->argument(0).toString();
        quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
        _q_self->connectToHostEncrypted(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 3) {
        if (context->argument(0).isString()
            && context->argument(1).isNumber()
            && ((qMetaTypeId<QFlags<QIODevice::OpenModeFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QIODevice::OpenModeFlag>() == context->argument(2).toVariant().userType()))) {
            QString _q_arg0 = context->argument(0).toString();
            quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
            QFlags<QIODevice::OpenModeFlag> _q_arg2 = qscriptvalue_cast<QFlags<QIODevice::OpenModeFlag> >(context->argument(2));
            _q_self->connectToHostEncrypted(_q_arg0, _q_arg1, _q_arg2);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isString()
            && context->argument(1).isNumber()
            && context->argument(2).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
            QString _q_arg2 = context->argument(2).toString();
            _q_self->connectToHostEncrypted(_q_arg0, _q_arg1, _q_arg2);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 4) {
        if (context->argument(0).isString()
            && context->argument(1).isNumber()
            && ((qMetaTypeId<QFlags<QIODevice::OpenModeFlag> >() == context->argument(2).toVariant().userType()) || (qMetaTypeId<QIODevice::OpenModeFlag>() == context->argument(2).toVariant().userType()))
            && (qMetaTypeId<QAbstractSocket::NetworkLayerProtocol>() == context->argument(3).toVariant().userType())) {
            QString _q_arg0 = context->argument(0).toString();
            quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
            QFlags<QIODevice::OpenModeFlag> _q_arg2 = qscriptvalue_cast<QFlags<QIODevice::OpenModeFlag> >(context->argument(2));
            QAbstractSocket::NetworkLayerProtocol _q_arg3 = qscriptvalue_cast<QAbstractSocket::NetworkLayerProtocol>(context->argument(3));
            _q_self->connectToHostEncrypted(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isString()
            && context->argument(1).isNumber()
            && context->argument(2).isString()
            && ((qMetaTypeId<QFlags<QIODevice::OpenModeFlag> >() == context->argument(3).toVariant().userType()) || (qMetaTypeId<QIODevice::OpenModeFlag>() == context->argument(3).toVariant().userType()))) {
            QString _q_arg0 = context->argument(0).toString();
            quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
            QString _q_arg2 = context->argument(2).toString();
            QFlags<QIODevice::OpenModeFlag> _q_arg3 = qscriptvalue_cast<QFlags<QIODevice::OpenModeFlag> >(context->argument(3));
            _q_self->connectToHostEncrypted(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 5) {
        QString _q_arg0 = context->argument(0).toString();
        quint16 _q_arg1 = static_cast<quint16>(context->argument(1).toUInt32());
        QString _q_arg2 = context->argument(2).toString();
        QFlags<QIODevice::OpenModeFlag> _q_arg3 = qscriptvalue_cast<QFlags<QIODevice::OpenModeFlag> >(context->argument(3));
        QAbstractSocket::NetworkLayerProtocol _q_arg4 = qscriptvalue_cast<QAbstractSocket::NetworkLayerProtocol>(context->argument(4));
        _q_self->connectToHostEncrypted(_q_arg0, _q_arg1, _q_arg2, _q_arg3, _q_arg4);
        return context->engine()->undefinedValue();
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        _q_self->continueInterruptedHandshake();
        return context->engine()->undefinedValue();
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        _q_self->encrypted();
        return context->engine()->undefinedValue();
    }
    break;

    case 3:
    if (context->argumentCount() == 0) {
        qint64 _q_result = _q_self->encryptedBytesAvailable();
        return qScriptValueFromValue(context->engine(), QVariant::fromValue(_q_result));
    }
    break;

    case 4:
    if (context->argumentCount() == 0) {
        qint64 _q_result = _q_self->encryptedBytesToWrite();
        return qScriptValueFromValue(context->engine(), QVariant::fromValue(_q_result));
    }
    break;

    case 5:
    if (context->argumentCount() == 1) {
        qint64 _q_arg0 = qscriptvalue_cast<qint64>(context->argument(0));
        _q_self->encryptedBytesWritten(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 6:
    if (context->argumentCount() == 1) {
        QSslError _q_arg0 = qscriptvalue_cast<QSslError>(context->argument(0));
        _q_self->handshakeInterruptedOnError(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        _q_self->ignoreSslErrors();
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 1) {
        QList<QSslError> _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        _q_self->ignoreSslErrors(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 8:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->isEncrypted();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        QSslCertificate _q_result = _q_self->localCertificate();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        QList<QSslCertificate > _q_result = _q_self->localCertificateChain();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 11:
    if (context->argumentCount() == 0) {
        QSslSocket::SslMode _q_result = _q_self->mode();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 12:
    if (context->argumentCount() == 1) {
        QSslSocket::SslMode _q_arg0 = qscriptvalue_cast<QSslSocket::SslMode>(context->argument(0));
        _q_self->modeChanged(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 13:
    if (context->argumentCount() == 0) {
        _q_self->newSessionTicketReceived();
        return context->engine()->undefinedValue();
    }
    break;

    case 14:
    if (context->argumentCount() == 0) {
        QSslCertificate _q_result = _q_self->peerCertificate();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 15:
    if (context->argumentCount() == 0) {
        QList<QSslCertificate > _q_result = _q_self->peerCertificateChain();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 16:
    if (context->argumentCount() == 0) {
        int _q_result = _q_self->peerVerifyDepth();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 17:
    if (context->argumentCount() == 1) {
        QSslError _q_arg0 = qscriptvalue_cast<QSslError>(context->argument(0));
        _q_self->peerVerifyError(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 18:
    if (context->argumentCount() == 0) {
        QSslSocket::PeerVerifyMode _q_result = _q_self->peerVerifyMode();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 19:
    if (context->argumentCount() == 0) {
        QString _q_result = _q_self->peerVerifyName();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 20:
    if (context->argumentCount() == 0) {
        QSslKey _q_result = _q_self->privateKey();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 21:
    if (context->argumentCount() == 0) {
        QSsl::SslProtocol _q_result = _q_self->protocol();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 22:
    if (context->argumentCount() == 0) {
        QSslCipher _q_result = _q_self->sessionCipher();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 23:
    if (context->argumentCount() == 0) {
        QSsl::SslProtocol _q_result = _q_self->sessionProtocol();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 24:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QSslCertificate>() == context->argument(0).toVariant().userType())) {
            QSslCertificate _q_arg0 = qscriptvalue_cast<QSslCertificate>(context->argument(0));
            _q_self->setLocalCertificate(_q_arg0);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            _q_self->setLocalCertificate(_q_arg0);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 2) {
        QString _q_arg0 = context->argument(0).toString();
        QSsl::EncodingFormat _q_arg1 = qscriptvalue_cast<QSsl::EncodingFormat>(context->argument(1));
        _q_self->setLocalCertificate(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    break;

    case 25:
    if (context->argumentCount() == 1) {
        QList<QSslCertificate> _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        _q_self->setLocalCertificateChain(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 26:
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        _q_self->setPeerVerifyDepth(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 27:
    if (context->argumentCount() == 1) {
        QSslSocket::PeerVerifyMode _q_arg0 = qscriptvalue_cast<QSslSocket::PeerVerifyMode>(context->argument(0));
        _q_self->setPeerVerifyMode(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 28:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        _q_self->setPeerVerifyName(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 29:
    if (context->argumentCount() == 1) {
        if ((qMetaTypeId<QSslKey>() == context->argument(0).toVariant().userType())) {
            QSslKey _q_arg0 = qscriptvalue_cast<QSslKey>(context->argument(0));
            _q_self->setPrivateKey(_q_arg0);
            return context->engine()->undefinedValue();
        } else if (context->argument(0).isString()) {
            QString _q_arg0 = context->argument(0).toString();
            _q_self->setPrivateKey(_q_arg0);
            return context->engine()->undefinedValue();
        }
    }
    if (context->argumentCount() == 2) {
        QString _q_arg0 = context->argument(0).toString();
        QSsl::KeyAlgorithm _q_arg1 = qscriptvalue_cast<QSsl::KeyAlgorithm>(context->argument(1));
        _q_self->setPrivateKey(_q_arg0, _q_arg1);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 3) {
        QString _q_arg0 = context->argument(0).toString();
        QSsl::KeyAlgorithm _q_arg1 = qscriptvalue_cast<QSsl::KeyAlgorithm>(context->argument(1));
        QSsl::EncodingFormat _q_arg2 = qscriptvalue_cast<QSsl::EncodingFormat>(context->argument(2));
        _q_self->setPrivateKey(_q_arg0, _q_arg1, _q_arg2);
        return context->engine()->undefinedValue();
    }
    if (context->argumentCount() == 4) {
        QString _q_arg0 = context->argument(0).toString();
        QSsl::KeyAlgorithm _q_arg1 = qscriptvalue_cast<QSsl::KeyAlgorithm>(context->argument(1));
        QSsl::EncodingFormat _q_arg2 = qscriptvalue_cast<QSsl::EncodingFormat>(context->argument(2));
        QByteArray _q_arg3 = qscriptvalue_cast<QByteArray>(context->argument(3));
        _q_self->setPrivateKey(_q_arg0, _q_arg1, _q_arg2, _q_arg3);
        return context->engine()->undefinedValue();
    }
    break;

    case 30:
    if (context->argumentCount() == 1) {
        QSsl::SslProtocol _q_arg0 = qscriptvalue_cast<QSsl::SslProtocol>(context->argument(0));
        _q_self->setProtocol(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 31:
    if (context->argumentCount() == 1) {
        QSslConfiguration _q_arg0 = qscriptvalue_cast<QSslConfiguration>(context->argument(0));
        _q_self->setSslConfiguration(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 32:
    if (context->argumentCount() == 0) {
        QSslConfiguration _q_result = _q_self->sslConfiguration();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 33:
    if (context->argumentCount() == 1) {
        QList<QSslError> _q_arg0;
        qScriptValueToSequence(context->argument(0), _q_arg0);
        _q_self->sslErrors(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 34:
    if (context->argumentCount() == 0) {
        QList<QSslError > _q_result = _q_self->sslHandshakeErrors();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 35:
    if (context->argumentCount() == 0) {
        _q_self->startClientEncryption();
        return context->engine()->undefinedValue();
    }
    break;

    case 36:
    if (context->argumentCount() == 0) {
        _q_self->startServerEncryption();
        return context->engine()->undefinedValue();
    }
    break;

    case 37:
    if (context->argumentCount() == 0) {
        bool _q_result = _q_self->waitForEncrypted();
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        int _q_arg0 = context->argument(0).toInt32();
        bool _q_result = _q_self->waitForEncrypted(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 38: {
    QString result = QString::fromLatin1("QSslSocket");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QSslSocket_throw_ambiguity_error_helper(context,
        qtscript_QSslSocket_function_names[_id+11],
        qtscript_QSslSocket_function_signatures[_id+11]);
}

static QScriptValue qtscript_QSslSocket_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QSslSocket(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QSslSocket* _q_cpp_result = new QtScriptShell_QSslSocket();
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QSslSocket*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        QObject* _q_arg0 = context->argument(0).toQObject();
        QtScriptShell_QSslSocket* _q_cpp_result = new QtScriptShell_QSslSocket(_q_arg0);
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QSslSocket*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    case 1:
    if (context->argumentCount() == 0) {
        QString _q_result = QSslSocket::activeBackend();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 2:
    if (context->argumentCount() == 0) {
        QList<QString > _q_result = QSslSocket::availableBackends();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 3:
    if (context->argumentCount() == 1) {
        QSsl::SslProtocol _q_arg0 = qscriptvalue_cast<QSsl::SslProtocol>(context->argument(0));
        bool _q_result = QSslSocket::isProtocolSupported(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    if (context->argumentCount() == 2) {
        QSsl::SslProtocol _q_arg0 = qscriptvalue_cast<QSsl::SslProtocol>(context->argument(0));
        QString _q_arg1 = context->argument(1).toString();
        bool _q_result = QSslSocket::isProtocolSupported(_q_arg0, _q_arg1);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 4:
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        bool _q_result = QSslSocket::setActiveBackend(_q_arg0);
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 5:
    if (context->argumentCount() == 0) {
        long _q_result = QSslSocket::sslLibraryBuildVersionNumber();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 6:
    if (context->argumentCount() == 0) {
        QString _q_result = QSslSocket::sslLibraryBuildVersionString();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 7:
    if (context->argumentCount() == 0) {
        long _q_result = QSslSocket::sslLibraryVersionNumber();
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 8:
    if (context->argumentCount() == 0) {
        QString _q_result = QSslSocket::sslLibraryVersionString();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    case 9:
    if (context->argumentCount() == 0) {
        QList<QSsl::SslProtocol > _q_result = QSslSocket::supportedProtocols();
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    if (context->argumentCount() == 1) {
        QString _q_arg0 = context->argument(0).toString();
        QList<QSsl::SslProtocol > _q_result = QSslSocket::supportedProtocols(_q_arg0);
        return qScriptValueFromSequence(context->engine(), _q_result);
    }
    break;

    case 10:
    if (context->argumentCount() == 0) {
        bool _q_result = QSslSocket::supportsSsl();
        return QScriptValue(context->engine(), _q_result);
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QSslSocket_throw_ambiguity_error_helper(context,
        qtscript_QSslSocket_function_names[_id],
        qtscript_QSslSocket_function_signatures[_id]);
}

static QScriptValue qtscript_QSslSocket_toScriptValue(QScriptEngine *engine, QSslSocket* const &in)
{
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

static void qtscript_QSslSocket_fromScriptValue(const QScriptValue &value, QSslSocket* &out)
{
    out = qobject_cast<QSslSocket*>(value.toQObject());
}

QScriptValue qtscript_create_QSslSocket_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QSslSocket*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QSslSocket*)0));
    proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QTcpSocket*>()));
    for (int i = 0; i < 39; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QSslSocket_prototype_call, qtscript_QSslSocket_function_lengths[i+11]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QSslSocket_function_names[i+11]),
            fun, QScriptValue::SkipInEnumeration);
    }

    qScriptRegisterMetaType<QSslSocket*>(engine, qtscript_QSslSocket_toScriptValue, 
        qtscript_QSslSocket_fromScriptValue, proto);

    QScriptValue ctor = engine->newFunction(qtscript_QSslSocket_static_call, proto, qtscript_QSslSocket_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));
    for (int i = 0; i < 10; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QSslSocket_static_call,
            qtscript_QSslSocket_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i+1)));
        ctor.setProperty(QString::fromLatin1(qtscript_QSslSocket_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    ctor.setProperty(QString::fromLatin1("PeerVerifyMode"),
        qtscript_create_QSslSocket_PeerVerifyMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("SslMode"),
        qtscript_create_QSslSocket_SslMode_class(engine, ctor));

    {
        QScriptValue supportedCiphersFun = engine->newFunction(
            [](QScriptContext *ctx, QScriptEngine *eng) -> QScriptValue {
                (void) ctx;
                QList<QSslCipher> ciphers = QSslConfiguration::supportedCiphers();
                return qScriptValueFromSequence(eng, ciphers);
            }, 0);
        ctor.setProperty(QString::fromLatin1("supportedCiphers"), supportedCiphersFun, QScriptValue::SkipInEnumeration);
    }
    
    return ctor;
}
