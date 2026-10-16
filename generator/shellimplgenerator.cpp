/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Script Generator project on Qt Labs.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "shellimplgenerator.h"
#include "reporthandler.h"
#include "fileout.h"
#include <algorithm>

// Qt 6: endl() lives in namespace Qt since 6.0; restore bare use.
using Qt::endl;

extern void declareFunctionMetaTypes(QTextStream &stream,
                                     const AbstractMetaFunctionList &functions,
                                     QSet<QString> &registeredTypeNames);

// Defined in abstractmetabuilder.cpp: first declared enumerator of an enum
// type entry, spelled for C++ (QValidator::Invalid, ...).
extern QString enumFailureLiteral(const EnumTypeEntry *entry);

// Qt 6: a C++ virtual that takes a non-const reference to a value or
// primitive type expects the callee to write back into it
// (QValidator::fixup(QString&), QAbstractSpinBox::validate(QString&,int&)).
// QtScript receives copies, so the script override carries results back
// through its return value. The generated shell unpacks them again:
//   void return + exactly one out-arg of type QString&
//       -> the returned string (fixup);
//   enum return + out-args
//       -> object {state = <return>, <argName> = <out-arg>...} (validate).
// Unraised results keep the plain-call behavior, so the contract only
// matters to overrides that return the documented shapes.
static bool outArgEngagesStringPack(const AbstractMetaFunction *fun,
                                    const QList<QPair<QString, AbstractMetaType *> > &outArgs)
{
    if (fun->type() != 0)
        return false;
    if (outArgs.size() != 1)
        return false;
    const TypeEntry *te = outArgs.first().second->typeEntry();
    return te && te->qualifiedCppName() == QLatin1String("QString");
}

static bool outArgEngagesObjectPack(const QList<QPair<QString, AbstractMetaType *> > &outArgs,
                                    const QString &failureLiteral)
{
    return !failureLiteral.isEmpty() && outArgs.size() > 0;
}

static void writeOutArgUnpack(QTextStream &s, const QString &argName, const AbstractMetaType *type)
{
    const QString spelled = type->typeEntry()->qualifiedCppName();
    s << "                if( _q_" << argName << ".";
    if (spelled == QLatin1String("QString")) {
        s << "isString() ) " << argName << " = _q_" << argName << ".toString();" << endl;
    } else if (spelled == QLatin1String("int")) {
        s << "isNumber() ) " << argName << " = _q_" << argName << ".toInt32();" << endl;
    } else if (spelled == QLatin1String("uint")) {
        s << "isNumber() ) " << argName << " = _q_" << argName << ".toUInt32();" << endl;
    } else if (spelled == QLatin1String("bool")) {
        s << "isBool() ) " << argName << " = _q_" << argName << ".toBool();" << endl;
    } else {
        s << "isValid() ) " << argName << " = qscriptvalue_cast<";
        ShellGenerator::writeTypeInfo(s, type, Generator::Option(Generator::SkipName));
        s << ">(_q_" << argName << ");" << endl;
    }
}

QString ShellImplGenerator::fileNameForClass(const AbstractMetaClass *meta_class) const
{
    return QString("qtscriptshell_%1.cpp").arg(meta_class->name());
}

static bool include_less_than(const Include &a, const Include &b)
{
    return a.name < b.name;
}

static void writeHelperCode(QTextStream &s, const AbstractMetaClass *)
{
    s << "#define QTSCRIPT_IS_GENERATED_FUNCTION(fun) ((fun.data().toUInt32() & 0xFFFF0000) == 0xBABE0000)" << endl;
    s << endl;
}

void writeQtScriptQtBindingsLicense(QTextStream &stream);

void ShellImplGenerator::write(QTextStream &s, const AbstractMetaClass *meta_class)
{
    if (FileOut::license)
        writeQtScriptQtBindingsLicense(s);

    QString packName = meta_class->package().replace(".", "_");

    priGenerator->addSource(packName, fileNameForClass(meta_class));

    s << "#include \"qtscriptshell_" << meta_class->name() << ".h\"" << endl << endl;

    if (!meta_class->generateShellClass())
        return;

    s << "#include <QtScript/QScriptEngine>" << endl;

    IncludeList list = meta_class->typeEntry()->extraIncludes();
    std::sort(list.begin(), list.end(), include_less_than);
    foreach (const Include &inc, list) {
        if (inc.type == Include::TargetLangImport)
            continue;

        s << "#include ";
        if (inc.type == Include::LocalPath)
            s << "\"";
        else
            s << "<";

        s << inc.name;

        if (inc.type == Include::LocalPath)
            s << "\"";
        else
            s << ">";

        s << endl;
    }
    s << endl;

    writeHelperCode(s, meta_class);

    // find constructors
    AbstractMetaFunctionList ctors;
    ctors = meta_class->queryFunctions(AbstractMetaClass::Constructors
                                       | AbstractMetaClass::WasVisible
                                       | AbstractMetaClass::NotRemovedFromTargetLang);
    // find member functions
    AbstractMetaFunctionList functions = meta_class->queryFunctions(
        AbstractMetaClass:: VirtualFunctions | AbstractMetaClass::WasVisible
        | AbstractMetaClass::NotRemovedFromTargetLang
        );

    // write metatype declarations
    {
        QSet<QString> registeredTypeNames = m_qmetatype_declared_typenames;
        declareFunctionMetaTypes(s, functions, registeredTypeNames);
        s << endl;
    }

    // write constructors
    foreach (const AbstractMetaFunction *ctor, ctors) {
        s << "QtScriptShell_" << meta_class->name() << "::";
        writeFunctionSignature(s, ctor, 0, QString(), Option(OriginalName | ShowStatic));
        s << endl;
        s << "    : " << meta_class->qualifiedCppName() << "(";
        AbstractMetaArgumentList args = ctor->arguments();
        for (int i = 0; i < args.size(); ++i) {
            if (i > 0)
                s << ", ";
            s << args.at(i)->argumentName();
        }
        s << ")" << " {}" << endl << endl;
    }

    // write destructor
    s << "QtScriptShell_" << meta_class->name() << "::"
      << "~QtScriptShell_" << meta_class->name() << "()";
    if (!meta_class->destructorException().isEmpty())
        s << " " << meta_class->destructorException();
    s << " {}" << endl << endl;

    // write member functions
    for (int i = 0; i < functions.size(); ++i) {
        AbstractMetaFunction *fun = functions.at(i);
        writeFunctionSignature(s, fun, meta_class, QString(),
                               Option(OriginalName | ShowStatic | UnderscoreSpaces),
                               "QtScriptShell_");
        s << endl << "{" << endl;
        QString scriptFunctionName = fun->name();
        {
            QPropertySpec *read = 0;
            for (const AbstractMetaClass *cls = meta_class; !read && cls; cls = cls->baseClass())
                read = cls->propertySpecForRead(fun->name());
            if (read && (read->name() == fun->name())) {
                // use different name to avoid infinite recursion
                // ### not sure if this is the best solution though...
                scriptFunctionName.prepend("_qs_");
            }
        }

        AbstractMetaArgumentList args = fun->arguments();

        // Out-argument recognition: non-const reference to a value or
        // primitive type (never pointer). Out-args among removed arguments
        // are not engaged.
        QList<QPair<QString, AbstractMetaType *> > outArgs;
        for (int j = 0; j < args.size(); ++j) {
            if (fun->argumentRemoved(j + 1))
                continue;
            AbstractMetaType *at = args.at(j)->type();
            if (!at || !at->isReference() || at->isConstant() || at->indirections() != 0)
                continue;
            const TypeEntry *te = at->typeEntry();
            if (!te || te->isNamespace())
                continue;
            if (!te->isValue() && !te->isPrimitive())
                continue;
            outArgs << QPair<QString, AbstractMetaType *>(args.at(j)->argumentName(), at);
        }

        QString failureLiteral;
        if (fun->type() && fun->type()->typeEntry() && fun->type()->typeEntry()->isEnum()
            && fun->type()->indirections() == 0 && !fun->type()->isReference())
            failureLiteral = enumFailureLiteral((EnumTypeEntry *) fun->type()->typeEntry());
        bool stringPack = outArgEngagesStringPack(fun, outArgs);
        bool objectPack = !stringPack && outArgEngagesObjectPack(outArgs, failureLiteral);

        s << "    QScriptValue _q_function = __qtscript_self.property(\""
          << scriptFunctionName << "\");" << endl;
        s << "    if (!_q_function.isFunction() || QTSCRIPT_IS_GENERATED_FUNCTION(_q_function)" << endl
          << "        || (__qtscript_self.propertyFlags(\"" << scriptFunctionName << "\") & QScriptValue::QObjectMember)) {" << endl;

        s << "        ";
        if (fun->isAbstract()) {
            s << "qFatal(\"" << meta_class->name() << "::" << fun->name()
              << "() is abstract!\");" << endl;
        } else {
            // call the C++ implementation
            if (fun->type())
                s << "return ";
            s << meta_class->qualifiedCppName() << "::" << fun->originalName() << "(";
            for (int i = 0; i < args.size(); ++i) {
                if (i > 0)
                    s << ", ";
                s << args.at(i)->argumentName();
            }
            s << ");" << endl;
        }

        s << "    } else {" << endl;

        if (stringPack || objectPack) {
            // Script override with out-argument write-back (fixup/validate
            // contract). Matches the call layout of the plain branch except
            // that it captures the result value.
            s << "        QScriptEngine *_q_engine = __qtscript_self.engine();" << endl;
            s << "        QScriptValue _q_result =" << endl;
            s << "            _q_function.call(__qtscript_self," << endl;
            int visibleArgs = 0;
            for (int j = 0; j < args.size(); ++j) {
                if (fun->argumentRemoved(j + 1))
                    continue;
                ++visibleArgs;
            }
            int written = 0;
            for (int j = 0; j < args.size(); ++j) {
                if (fun->argumentRemoved(j + 1))
                    continue;
                if (written == 0)
                    s << "                             QScriptValueList() << qScriptValueFromValue(_q_engine, ";
                else
                    s << "                                                << qScriptValueFromValue(_q_engine, ";
                s << args.at(j)->argumentName() << ")";
                if (written == visibleArgs - 1)
                    s << ");" << endl;
                else
                    s << endl;
                ++written;
            }
            if (stringPack) {
                s << "        if( !_q_engine->hasUncaughtException() && _q_result.isString() ) "
                  << outArgs.first().first << " = _q_result.toString();" << endl;
            } else {
                s << "        if( _q_engine->hasUncaughtException() ) return " << failureLiteral << ";" << endl;
                s << "        if( _q_result.isObject() )" << endl;
                s << "        {" << endl;
                s << "            QScriptValue _q_state = _q_result.property(\"state\");" << endl;
                s << "            if( _q_engine->hasUncaughtException() ) return " << failureLiteral << ";" << endl;
                s << "            if( _q_state.isValid() )" << endl;
                s << "            {" << endl;
                s << "                ";
                writeTypeInfo(s, fun->type(), Option(SkipName));
                s << " _q_value = qscriptvalue_cast<";
                writeTypeInfo(s, fun->type(), Option(SkipName));
                s << ">(_q_state);" << endl;
                s << "                if( _q_engine->hasUncaughtException() ) return " << failureLiteral << ";" << endl;
                for (int j = 0; j < outArgs.size(); ++j) {
                    s << "                QScriptValue _q_" << outArgs.at(j).first
                      << " = _q_result.property(\"" << outArgs.at(j).first << "\");" << endl;
                    s << "                if( _q_engine->hasUncaughtException() ) return " << failureLiteral << ";" << endl;
                }
                for (int j = 0; j < outArgs.size(); ++j)
                    writeOutArgUnpack(s, outArgs.at(j).first, outArgs.at(j).second);
                s << "                return _q_value;" << endl;
                s << "            }" << endl;
                s << "        }" << endl;
                s << "        return qscriptvalue_cast<";
                writeTypeInfo(s, fun->type(), Option(SkipName));
                s << ">(_q_result);" << endl;
            }
        } else {
            // call the script function
            if (args.size() > 0)
                s << "        QScriptEngine *_q_engine = __qtscript_self.engine();" << endl;
            s << "        ";
            if (fun->type()) {
                s << "return qscriptvalue_cast<";
                writeTypeInfo(s, fun->type());
                s << ">(";
            }
            s << "_q_function.call(__qtscript_self";
            if (args.size() > 0) {
                s << "," << endl;
                s << "            QScriptValueList()";
                int i = 0;
                for (int j = 0; j < args.size(); ++j) {
                    if (fun->argumentRemoved(j+1))
                        continue;
                    s << endl << "            << ";
                    s << "qScriptValueFromValue(_q_engine, ";
                    AbstractMetaType *atype = args.at(j)->type();
                    QString asig = atype->cppSignature();
                    bool constCastArg = asig.endsWith('*') && asig.startsWith("const ");
                    if (constCastArg) {
                        s << "const_cast<" << asig << ">(";
                    }
                    s << args.at(i)->argumentName() << ")";
                    if (constCastArg)
                        s << ")";
                    ++i;
                }
            }
            s << ")";
            if (fun->type())
                s << ")";
            s << ";" << endl;
        }

        s << "    }" << endl;

        s << "}" << endl << endl;
    }
}

void ShellImplGenerator::writeInjectedCode(QTextStream &s, const AbstractMetaClass *meta_class)
{
    CodeSnipList code_snips = meta_class->typeEntry()->codeSnips();
    foreach (const CodeSnip &cs, code_snips) {
        if (cs.language == TypeSystem::ShellCode) {
            s << cs.code() << endl;
        }
    }
}
