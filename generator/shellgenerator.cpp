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

#include "shellgenerator.h"
#include "reporthandler.h"

#include <QRegularExpression>

#include "metaqtscript.h"

bool ShellGenerator::shouldGenerate(const AbstractMetaClass *meta_class) const
{
    uint cg = meta_class->typeEntry()->codeGeneration();
    return ((cg & TypeEntry::GenerateCode) != 0) && meta_class->generateShellClass();
}

void ShellGenerator::writeTypeInfo(QTextStream &s, const AbstractMetaType *type, Option options)
{
    if ((options & OriginalTypeDescription) && !type->originalTypeDescription().isEmpty()) {
        s << type->originalTypeDescription();
        return;
    }

    if (type->isArray()) {
        writeTypeInfo(s, type->arrayElementType(), options);
        if (options & ArrayAsPointer) {
            s << "*";
        } else {
            s << "[" << type->arrayElementCount() << "]";
        }
        return;
    }

    const TypeEntry *te = type->typeEntry();

    if (type->isConstant() && !(options & ExcludeConst))
        s << "const ";

    if ((options & EnumAsInts) && (te->isEnum() || te->isFlags())) {
        s << "int";
    } else if (te->isFlags()) {
        s << ((FlagsTypeEntry *) te)->originalName();
    } else {
        s << fixCppTypeName(te->qualifiedCppName());
    }

    if (type->instantiations().size() > 0
        && (!type->isContainer() 
            || (static_cast<const ContainerTypeEntry *>(te))->type() != ContainerTypeEntry::StringListContainer)) {
        s << '<';
        QList<AbstractMetaType *> args = type->instantiations();
        bool nested_template = false;
        for (int i=0; i<args.size(); ++i) {
            if (i != 0)
                s << ", ";
            nested_template |= args.at(i)->isContainer();
            writeTypeInfo(s, args.at(i));
        }
        if (nested_template)
            s << ' ';
        s << '>';
    }

    s << QString(type->indirections(), '*');

    if (type->isReference() && !(options & ExcludeReference))
        s << "&";

    if (!(options & SkipName))
        s << ' ';
}


void ShellGenerator::writeFunctionArguments(QTextStream &s,
                                          const AbstractMetaArgumentList &arguments,
                                          Option option,
                                          int numArguments)
{
    if (numArguments < 0) numArguments = arguments.size();

    // Qt 6: C++ requires trailing defaults to be contiguous: if any argument
    // from position i on lacks a default, no earlier argument may carry one
    // either. Precompute per-argument default text (original preferred, else
    // the translated-fallback below), then emit only the trailing run.
    QList<QString> defaultFor;
    for (int i = 0; i < numArguments; ++i) {
        defaultFor.append(QString());
        if (!(option & IncludeDefaultExpression))
            continue;
        AbstractMetaArgument *arg = arguments.at(i);
        if (!arg->originalDefaultValueExpression().isEmpty()) {
            QString expr = arg->originalDefaultValueExpression();
            if (arg->type()->typeEntry()->isEnum() && expr.indexOf("::") < 0)
                expr = ((EnumTypeEntry *)arg->type()->typeEntry())->qualifier()
                       + QLatin1String("::") + expr;
            defaultFor[i] = expr;
        } else if (!arg->defaultValueExpression().isEmpty()) {
            // Translated-fallback for DOM-lost originals (e.g. QPixmap's
            // filename ctor): reuse only provably C++-valid spellings.
            QString expr = arg->defaultValueExpression();
            const TypeEntry *entry = arg->type() ? arg->type()->typeEntry() : 0;
            bool isPointer = arg->type() && arg->type()->indirections() > 0;
            if (entry && entry->isPrimitive()) {
                QRegularExpression numBool("^(true|false|[-+]?[0-9][0-9.]*)$");
                if (numBool.match(expr).hasMatch())
                    defaultFor[i] = expr;
            } else if (isPointer) {
                if (expr == QLatin1String("null") || expr == QLatin1String("0")
                    || expr == QLatin1String("nullptr"))
                    defaultFor[i] = QLatin1String("nullptr");
                else {
                    // Qualified no-arg factory calls (e.g.
                    // QInputDevice::primaryKeyboard()) are valid defaults.
                    QRegularExpression factory("^[\\w:]+[(][)]$");
                    if (factory.match(expr).hasMatch())
                        defaultFor[i] = expr;
                }
            } else if (entry && (entry->isEnum() || entry->isFlags())
                       && expr.indexOf("::") >= 0 && expr.indexOf('.') < 0) {
                defaultFor[i] = expr;
            }
        }
    }
    int firstDefault = numArguments;
    for (int i = numArguments - 1; i >= 0; --i) {
        if (defaultFor.at(i).isEmpty())
            break;
        firstDefault = i;
    }

    for (int i=0; i<numArguments; ++i) {
        if (i != 0)
            s << ", ";
        AbstractMetaArgument *arg = arguments.at(i);
        writeTypeInfo(s, arg->type(), option);
        if (!(option & SkipName))
            s << " " << arg->argumentName();
        if (i >= firstDefault)
            s << " = " << defaultFor.at(i);
    }
}

/*!
 * Writes the function \a meta_function signature to the textstream \a s.
 *
 * The \a name_prefix can be used to give the function name a prefix,
 * like "__public_" or "__override_" and \a classname_prefix can
 * be used to give the class name a prefix.
 *
 * The \a option flags can be used to tweak various parameters, such as
 * showing static, original vs renamed name, underscores for space etc.
 *
 * The \a extra_arguments list is a list of extra arguments on the
 * form "bool static_call".
 */

void ShellGenerator::writeFunctionSignature(QTextStream &s,
                                          const AbstractMetaFunction *meta_function,
                                          const AbstractMetaClass *implementor,
                                          const QString &name_prefix,
                                          Option option,
                                          const QString &classname_prefix,
                                          const QStringList &extra_arguments,
                                          int numArguments)
{
// ### remove the implementor
    AbstractMetaType *function_type = meta_function->type();

    if (meta_function->isStatic() && (option & ShowStatic))
        s << "static ";

    if ((option & SkipReturnType) == 0) {
        if (function_type) {
            writeTypeInfo(s, function_type, option);
            s << " ";
        } else if (!meta_function->isConstructor()) {
            s << "void ";
        }
    }

    if (implementor) {
        if (classname_prefix.isEmpty())
            s << shellClassName(implementor) << "::";
        else
            s << classname_prefix << implementor->name() << "::";
    }


    QString function_name;
    if (option & OriginalName)
        function_name = meta_function->originalName();
    else
        function_name = meta_function->name();

    if (option & UnderscoreSpaces)
        function_name = function_name.replace(' ', '_');

    if (meta_function->isConstructor())
        function_name = shellClassName(meta_function->ownerClass());

    s << name_prefix << function_name;

    if (meta_function->attributes() & AbstractMetaAttributes::SetterFunction)
        s << "_setter";
    else if (meta_function->attributes() & AbstractMetaAttributes::GetterFunction)
        s << "_getter";

    s << "(";

   writeFunctionArguments(s, meta_function->arguments(), option, numArguments);

    // The extra arguments...
    for (int i=0; i<extra_arguments.size(); ++i) {
        if (i > 0 || meta_function->arguments().size() != 0)
            s << ", ";
        s << extra_arguments.at(i);
    }

    s << ")";
    if (meta_function->isConstant())
        s << " const";

    if (!meta_function->exception().isEmpty())
        s << " " << meta_function->exception();
}
