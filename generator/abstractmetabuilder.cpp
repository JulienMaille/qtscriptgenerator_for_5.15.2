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

#include "abstractmetabuilder.h"
#include "reporthandler.h"

#include "ast.h"
#include "binder.h"
#include "control.h"
#include "default_visitor.h"
#include "dumptree.h"
#include "lexer.h"
#include "parser.h"
#include "tokens.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStringConverter>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <algorithm>

// Qt 6: first declared enumerator of an enum type entry, spelled for C++
// (QValidator::Invalid, ...). Shell overrides use it as the failure value
// when a script override of an out-argument virtual raises an exception.
QHash<const EnumTypeEntry *, QString> &registeredEnumFailureLiterals()
{
    static QHash<const EnumTypeEntry *, QString> map;
    return map;
}

QString enumFailureLiteral(const EnumTypeEntry *entry)
{
    return entry ? registeredEnumFailureLiterals().value(entry) : QString();
}

// Qt 6: endl() lives in namespace Qt since 6.0; restore bare use.
using Qt::endl;

static QString strip_template_args(const QString &name)
{
    int pos = name.indexOf('<');
    return pos < 0 ? name : name.left(pos);
}

static QHash<QString, QString> *operator_names;
QString rename_operator(const QString &oper)
{
    QString op = oper.trimmed();
    if (!operator_names) {
        operator_names = new QHash<QString, QString>;

        operator_names->insert("+", "add");
        operator_names->insert("-", "subtract");
        operator_names->insert("*", "multiply");
        operator_names->insert("/", "divide");
        operator_names->insert("%", "modulo");
        operator_names->insert("&", "and");
        operator_names->insert("|", "or");
        operator_names->insert("^", "xor");
        operator_names->insert("~", "negate");
        operator_names->insert("<<", "shift_left");
        operator_names->insert(">>", "shift_right");

        // assigments
        operator_names->insert("=", "assign");
        operator_names->insert("+=", "add_assign");
        operator_names->insert("-=", "subtract_assign");
        operator_names->insert("*=", "multiply_assign");
        operator_names->insert("/=", "divide_assign");
        operator_names->insert("%=", "modulo_assign");
        operator_names->insert("&=", "and_assign");
        operator_names->insert("|=", "or_assign");
        operator_names->insert("^=", "xor_assign");
        operator_names->insert("<<=", "shift_left_assign");
        operator_names->insert(">>=", "shift_right_assign");

        // Logical
        operator_names->insert("&&", "logical_and");
        operator_names->insert("||", "logical_or");
        operator_names->insert("!", "not");

        // incr/decr
        operator_names->insert("++", "increment");
        operator_names->insert("--", "decrement");

        // compare
        operator_names->insert("<", "less");
        operator_names->insert(">", "greater");
        operator_names->insert("<=", "less_or_equal");
        operator_names->insert(">=", "greater_or_equal");
        operator_names->insert("!=", "not_equal");
        operator_names->insert("==", "equal");

        // other
        operator_names->insert("[]", "subscript");
        operator_names->insert("->", "pointer");
    }

    if (!operator_names->contains(op)) {
        TypeDatabase *tb = TypeDatabase::instance();

        TypeParser::Info typeInfo = TypeParser::parse(op);
        QString cast_to_name = typeInfo.qualified_name.join("::");
        TypeEntry *te = tb->findType(cast_to_name);
        if ((te && te->codeGeneration() == TypeEntry::GenerateNothing)
            || tb->isClassRejected(cast_to_name)) {
            return QString();
        } else if (te) {
            return "operator_cast_" + typeInfo.qualified_name.join("_");
        } else {
            // Qt 6: a conversion operator whose target type is not in the type
            // system cannot be bound. The parser records no return type for a
            // conversion operator (the type lives in its name), so the code
            // model falls back to the enclosing class: QByteArray's
            //     Q_IMPLICIT operator std::string_view() const noexcept
            // was emitted as
            //     QByteArray _q_result = _q_self->operator std::string_view();
            // which does not compile. Reject it, exactly like a cast target
            // that is explicitly not generated.
            ReportHandler::warning(QString("unknown operator '%1', generation disabled").arg(op));
            return QString();
        }
    }

    return "operator_" + operator_names->value(op);
}

AbstractMetaBuilder::AbstractMetaBuilder()
    : m_current_class(0)
{
}

void AbstractMetaBuilder::checkFunctionModifications()
{
    TypeDatabase *types = TypeDatabase::instance();
    SingleTypeEntryHash entryHash = types->entries();
    QList<TypeEntry *> entries = entryHash.values();
    foreach (TypeEntry *entry, entries) {
        if (entry == 0)
            continue;
        if (!entry->isComplex() || entry->codeGeneration() == TypeEntry::GenerateNothing)
            continue;

        ComplexTypeEntry *centry = static_cast<ComplexTypeEntry *>(entry);
        FunctionModificationList modifications = centry->functionModifications();

        foreach (FunctionModification modification, modifications) {
            QString signature = modification.signature;

            QString name = signature.trimmed();
            name = name.mid(0, signature.indexOf("("));

            AbstractMetaClass *clazz = m_meta_classes.findClass(centry->qualifiedCppName());
            if (clazz == 0)
                continue;

            AbstractMetaFunctionList functions = clazz->functions();
            bool found = false;
            QStringList possibleSignatures;
            foreach (AbstractMetaFunction *function, functions) {
                if (function->minimalSignature() == signature && function->implementingClass() == clazz) {
                    found = true;
                    break;
                }

                if (function->originalName() == name)
                    possibleSignatures.append(function->minimalSignature() + " in " + function->implementingClass()->name());
            }

            if (!found) {
                QString warning
                    = QString("signature '%1' for function modification in '%2' not found. Possible candidates: %3")
                        .arg(signature)
                        .arg(clazz->qualifiedCppName())
                        .arg(possibleSignatures.join(", "));

                ReportHandler::warning(warning);
            }
        }
    }
}

AbstractMetaClass *AbstractMetaBuilder::argumentToClass(ArgumentModelItem argument)
{
    AbstractMetaClass *returned = 0;
    bool ok = false;
    AbstractMetaType *type = translateType(argument->type(), &ok);
    if (ok && type != 0 && type->typeEntry() != 0 && type->typeEntry()->isComplex()) {
        const TypeEntry *entry = type->typeEntry();
        returned = m_meta_classes.findClass(entry->name());
    }
    delete type;
    return returned;
}

/**
 * Checks the argument of a hash function and flags the type if it is a complex type
 */
void AbstractMetaBuilder::registerHashFunction(FunctionModelItem function_item)
{
    ArgumentList arguments = function_item->arguments();
    if (arguments.size() == 1) {
        if (AbstractMetaClass *cls = argumentToClass(arguments.at(0)))
            cls->setHasHashFunction(true);
    }
}

/**
 * Check if a class has a debug stream operator that can be used as toString
 */

void AbstractMetaBuilder::registerToStringCapability(FunctionModelItem function_item)
{
    ArgumentList arguments = function_item->arguments();
    // Free-function shape: operator<<(QDebug, const X&). Qt 6 member shape:
    // QDebug::operator<<(const X&) — single argument, owner is QDebug.
    const int valueIndex = (arguments.size() == 1) ? 0 : 1;
    if (arguments.size() != 1 && arguments.size() != 2)
        return;
    if (arguments.size() == 2 && arguments.at(0)->type().toString() != "QDebug")
        return;
    {
        ArgumentModelItem arg = arguments.at(valueIndex);
        if (AbstractMetaClass *cls = argumentToClass(arg)) {
            if (arg->type().indirections() < 2) {
                cls->setToStringCapability(function_item);
            }
        }
    }
}

void AbstractMetaBuilder::traverseCompareOperator(FunctionModelItem item) {
    ArgumentList arguments = item->arguments();
    if (arguments.size() == 2 && item->accessPolicy() == CodeModel::Public) {
        AbstractMetaClass *comparer_class = argumentToClass(arguments.at(0));
        AbstractMetaClass *compared_class = argumentToClass(arguments.at(1));
        if (comparer_class != 0 && compared_class != 0) {
            AbstractMetaClass *old_current_class = m_current_class;
            m_current_class = comparer_class;

            AbstractMetaFunction *meta_function = traverseFunction(item);
            if (meta_function != 0 && !meta_function->isInvalid()) {
                // Strip away first argument, since that is the containing object
                AbstractMetaArgumentList arguments = meta_function->arguments();
                arguments.pop_front();
                meta_function->setArguments(arguments);

                meta_function->setFunctionType(AbstractMetaFunction::GlobalScopeFunction);

                meta_function->setOriginalAttributes(meta_function->attributes());
                setupFunctionDefaults(meta_function, comparer_class);

                comparer_class->addFunction(meta_function);
            } else if (meta_function != 0) {
                delete meta_function;
            }

            m_current_class = old_current_class;
        }
    }
}

void AbstractMetaBuilder::traverseStreamOperator(FunctionModelItem item)
{
    ArgumentList arguments = item->arguments();
    if (arguments.size() == 2 && item->accessPolicy() == CodeModel::Public) {
        AbstractMetaClass *streamClass = argumentToClass(arguments.at(0));
        AbstractMetaClass *streamedClass = argumentToClass(arguments.at(1));

        if (streamClass != 0 && streamedClass != 0
            && (streamClass->name() == "QDataStream" || streamClass->name() == "QTextStream")) {
            AbstractMetaClass *old_current_class = m_current_class;
            m_current_class = streamedClass;
            AbstractMetaFunction *streamFunction = traverseFunction(item);

            if (streamFunction != 0 && !streamFunction->isInvalid()) {
                QString name = item->name();
                streamFunction->setFunctionType(AbstractMetaFunction::GlobalScopeFunction);

                if (name.endsWith("<<"))
                    streamFunction->setName("writeTo");
                else
                    streamFunction->setName("readFrom");

                // Strip away last argument, since that is the containing object
                AbstractMetaArgumentList arguments = streamFunction->arguments();
                arguments.pop_back();
                streamFunction->setArguments(arguments);

                *streamFunction += AbstractMetaAttributes::Final;
                *streamFunction += AbstractMetaAttributes::Public;
                streamFunction->setOriginalAttributes(streamFunction->attributes());

                streamFunction->setType(0);

                setupFunctionDefaults(streamFunction, streamedClass);

                streamedClass->addFunction(streamFunction);
                streamedClass->typeEntry()->addExtraInclude(streamClass->typeEntry()->include());

                m_current_class = old_current_class;
            }
        }
    }
}

void AbstractMetaBuilder::fixQObjectForScope(TypeDatabase *types,
                                         NamespaceModelItem scope)
{
    foreach (ClassModelItem item, scope->classes()) {
        QString qualified_name = item->qualifiedName().join("::");
        TypeEntry *entry = types->findType(qualified_name);
        if (entry) {
            if (isQObject(qualified_name) && entry->isComplex()) {
                ((ComplexTypeEntry *) entry)->setQObject(true);
            }
        }
    }

    foreach (NamespaceModelItem item, scope->namespaceMap().values()) {
        if (scope != item)
          fixQObjectForScope(types, item);
    }
}

static bool class_less_than(AbstractMetaClass *a, AbstractMetaClass *b)
{
    return a->name() < b->name();
}


void AbstractMetaBuilder::sortLists()
{
   std::sort(m_meta_classes.begin(), m_meta_classes.end(), class_less_than);
   foreach (AbstractMetaClass *cls, m_meta_classes) {
        cls->sortFunctions();
   }
}

bool AbstractMetaBuilder::build()
{
    Q_ASSERT(!m_file_name.isEmpty());

    QFile file(m_file_name);

    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);
    // Qt 6: QTextStream defaults to UTF-8; setCodec() was removed.
    QByteArray contents = stream.readAll().toUtf8();
    file.close();

    Control control;
    Parser p(&control);
    pool __pool;

    TranslationUnitAST *ast = p.parse(contents, contents.size(), &__pool);

    CodeModel model;
    Binder binder(&model, p.location());
    m_dom = binder.run(ast);

    pushScope(model_dynamic_cast<ScopeModelItem>(m_dom));

    QHash<QString, ClassModelItem> typeMap = m_dom->classMap();

    // fix up QObject's in the type system..
    TypeDatabase *types = TypeDatabase::instance();
    fixQObjectForScope(types, model_dynamic_cast<NamespaceModelItem>(m_dom));


    // Start the generation...
    foreach (ClassModelItem item, typeMap.values()) {
        AbstractMetaClass *cls = traverseClass(item);
        addAbstractMetaClass(cls);
    }


    QHash<QString, NamespaceModelItem> namespaceMap = m_dom->namespaceMap();
    foreach (NamespaceModelItem item, namespaceMap.values()) {
        AbstractMetaClass *meta_class = traverseNamespace(item);
        if (meta_class)
            m_meta_classes << meta_class;
    }


    // Some trickery to support global-namespace enums...
    QHash<QString, EnumModelItem> enumMap = m_dom->enumMap();
    m_current_class = 0;
    foreach (EnumModelItem item, enumMap) {
        AbstractMetaEnum *meta_enum = traverseEnum(item, 0, QSet<QString>());

        if (meta_enum) {
            QString package = meta_enum->typeEntry()->javaPackage();
            QString globalName = TypeDatabase::globalNamespaceClassName(meta_enum->typeEntry());

            AbstractMetaClass *global = m_meta_classes.findClass(package + "." + globalName);
            if (!global) {
                ComplexTypeEntry *gte = new ObjectTypeEntry(globalName);
                gte->setTargetLangPackage(meta_enum->typeEntry()->javaPackage());
                gte->setCodeGeneration(meta_enum->typeEntry()->codeGeneration());
                global = createMetaClass();
                global->setTypeEntry(gte);
                *global += AbstractMetaAttributes::Final;
                *global += AbstractMetaAttributes::Public;
                *global += AbstractMetaAttributes::Fake;

                m_meta_classes << global;
            }

            global->addEnum(meta_enum);
            meta_enum->setEnclosingClass(global);
            meta_enum->typeEntry()->setQualifier(globalName);

            // Global enums should be public despite not having public
            // identifiers so we'll fix the original attributes here.
            meta_enum->setOriginalAttributes(meta_enum->attributes());
        }


    }


    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes.
    TypeAliasList typeAliases = m_dom->typeAliases();
    foreach (TypeAliasModelItem typeAlias, typeAliases) {
        AbstractMetaClass *cls = traverseTypeAlias(typeAlias);
        addAbstractMetaClass(cls);
    }




    foreach (AbstractMetaClass *cls, m_meta_classes) {
        if (!cls->isInterface() && !cls->isNamespace()) {
            setupInheritance(cls);
        }
    }


    foreach (AbstractMetaClass *cls, m_meta_classes) {
        cls->fixFunctions();

        if (cls->typeEntry() == 0) {
            ReportHandler::warning(QString("class '%1' does not have an entry in the type system")
                                   .arg(cls->name()));
        } else {
            // Qt 6: skip synthesis for private-only construction (the
            // synthesized public default would not compile).
            if (!cls->hasConstructors() && !cls->isFinalInCpp() && !cls->isInterface() && !cls->isNamespace()
                && !cls->hasOnlyPrivateConstructors())
                cls->addDefaultConstructor();
        }

        if (cls->isAbstract() && !cls->isInterface()) {
            cls->typeEntry()->setLookupName(cls->typeEntry()->targetLangName() + "$ConcreteWrapper");
        }
    }

    QList<TypeEntry *> entries = TypeDatabase::instance()->entries().values();
    foreach (const TypeEntry *entry, entries) {
        if (entry->isPrimitive())
            continue;

        if ((entry->isValue() || entry->isObject())
            && !entry->isString()
            && !entry->isChar()
            && !entry->isContainer()
            && !entry->isCustom()
            && !entry->isVariant()
            && !m_meta_classes.findClass(entry->qualifiedCppName())) {
            ReportHandler::warning(QString("type '%1' is specified in typesystem, but not defined. This could potentially lead to compilation errors.")
                                   .arg(entry->qualifiedCppName()));
        }

        if (entry->isEnum()) {
            QString pkg = entry->javaPackage();
            QString name = (pkg.isEmpty() ? QString() : pkg + ".")
                           + ((EnumTypeEntry *) entry)->javaQualifier();
            AbstractMetaClass *cls = m_meta_classes.findClass(name);

            if (!cls) {
                ReportHandler::warning(QString("namespace '%1' for enum '%2' is not declared")
                                       .arg(name).arg(entry->targetLangName()));
            } else {
                AbstractMetaEnum *e = cls->findEnum(entry->targetLangName());
                if (!e)
                    ReportHandler::warning(QString("enum '%1' is specified in typesystem, "
                                                   "but not declared")
                                           .arg(entry->qualifiedCppName()));
            }
        }
    }

    {
        FunctionList hash_functions = m_dom->findFunctions("qHash");
        foreach (FunctionModelItem item, hash_functions) {
            registerHashFunction(item);
        }
    }

    {
        FunctionList hash_functions = m_dom->findFunctions("operator<<");
        foreach (FunctionModelItem item, hash_functions) {
            registerToStringCapability(item);
        }
        // Qt 6: stream operators are member functions of QDebug (e.g.
        // QDebug::operator<<(const QByteArray &)), not free functions, so the
        // file-scope search above misses them. Scan QDebug's members for the
        // single-argument member form and register the same capability.
        ClassModelItem qdebugClass = m_dom->findClass(QLatin1String("QDebug"));
        if (qdebugClass) {
            foreach (FunctionModelItem item, qdebugClass->functions()) {
                if (item->name() != QLatin1String("operator<<"))
                    continue;
                if (item->arguments().size() != 1)
                    continue;
                registerToStringCapability(item);
            }
        }
    }

    {
        FunctionList compare_operators = m_dom->findFunctions("operator==")
                                         + m_dom->findFunctions("operator<=")
                                         + m_dom->findFunctions("operator>=")
                                         + m_dom->findFunctions("operator<")
                                         + m_dom->findFunctions("operator>");
        foreach (FunctionModelItem item, compare_operators) {
            traverseCompareOperator(item);
        }
    }

    {
        FunctionList stream_operators = m_dom->findFunctions("operator<<") + m_dom->findFunctions("operator>>");
        foreach (FunctionModelItem item, stream_operators) {
            traverseStreamOperator(item);
        }
    }

    figureOutEnumValues();
    figureOutDefaultEnumArguments();
    checkFunctionModifications();

    foreach (AbstractMetaClass *cls, m_meta_classes) {
        setupEquals(cls);
        setupComparable(cls);
        setupClonable(cls);
    }

    dumpLog();

    sortLists();

    return true;
}


void AbstractMetaBuilder::addAbstractMetaClass(AbstractMetaClass *cls)
{
    if (!cls)
        return;

    cls->setOriginalAttributes(cls->attributes());
    if (cls->typeEntry()->isContainer()) {
        m_templates << cls;
    } else {
        m_meta_classes << cls;
        // Qt 6: resolve any pending using-alias registrations whose target
        // is the class that was just added (aliases are recorded during
        // traversal, but targets may traverse later).
        QString justAdded = cls->qualifiedCppName();
        QMutableMapIterator<QString, QString> pit(m_pending_alias_targets);
        while (pit.hasNext()) {
            pit.next();
            if (pit.value() == justAdded) {
                m_aliased_meta_classes.insert(pit.key(), cls);
                pit.remove();
            }
        }
        if (cls->typeEntry()->designatedInterface()) {
            AbstractMetaClass *interface = cls->extractInterface();
            m_meta_classes << interface;
            ReportHandler::debugSparse(QString(" -> interface '%1'").arg(interface->name()));
        }
    }
}


AbstractMetaClass *AbstractMetaBuilder::traverseNamespace(NamespaceModelItem namespace_item)
{
    QString namespace_name = (!m_namespace_prefix.isEmpty() ? m_namespace_prefix + "::" : QString()) + namespace_item->name();
    NamespaceTypeEntry *type = TypeDatabase::instance()->findNamespaceType(namespace_name);

    if (TypeDatabase::instance()->isClassRejected(namespace_name)) {
        m_rejected_classes.insert(namespace_name, GenerationDisabled);
        return 0;
    }

    if (!type) {
        ReportHandler::warning(QString("namespace '%1' does not have a type entry")
                               .arg(namespace_name));
        return 0;
    }

    AbstractMetaClass *meta_class = createMetaClass();
    meta_class->setTypeEntry(type);

    *meta_class += AbstractMetaAttributes::Public;

    m_current_class = meta_class;

    ReportHandler::debugSparse(QString("namespace '%1.%2'")
                               .arg(meta_class->package())
                               .arg(namespace_item->name()));

    traverseEnums(model_dynamic_cast<ScopeModelItem>(namespace_item), meta_class, namespace_item->enumsDeclarations());
    traverseFunctions(model_dynamic_cast<ScopeModelItem>(namespace_item), meta_class);
//     traverseClasses(model_dynamic_cast<ScopeModelItem>(namespace_item));

    pushScope(model_dynamic_cast<ScopeModelItem>(namespace_item));
    m_namespace_prefix = currentScope()->qualifiedName().join("::");


    ClassList classes = namespace_item->classes();
    foreach (ClassModelItem cls, classes) {
        AbstractMetaClass *mjc = traverseClass(cls);
        addAbstractMetaClass(mjc);
    }

    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes.
    TypeAliasList typeAliases = namespace_item->typeAliases();
    foreach (TypeAliasModelItem typeAlias, typeAliases) {
        AbstractMetaClass *cls = traverseTypeAlias(typeAlias);
        addAbstractMetaClass(cls);
    }



    // Traverse namespaces recursively
    QList<NamespaceModelItem> inner_namespaces = namespace_item->namespaceMap().values();
    foreach (const NamespaceModelItem &ni, inner_namespaces) {
        AbstractMetaClass *mjc = traverseNamespace(ni);
        addAbstractMetaClass(mjc);
    }

    m_current_class = 0;


    popScope();
    m_namespace_prefix = currentScope()->qualifiedName().join("::");

    if (!type->include().isValid() && !type->isContainer()) {
        QFileInfo info(namespace_item->fileName());

        type->setInclude(Include(Include::IncludePath, info.fileName()));
    }

    return meta_class;
}

struct Operator
{
    enum Type { Plus, ShiftLeft, None };

    Operator() : type(None) { }

    int calculate(int x) {
        switch (type) {
        case Plus: return x + value;
        case ShiftLeft: return x << value;
        case None: return x;
        }
        return x;
    }

    Type type;
    int value;
};



Operator findOperator(QString *s) {
    const char *names[] = {
        "+",
        "<<"
    };

    for (int i=0; i<Operator::None; ++i) {
        QString name = QLatin1String(names[i]);
        QString str = *s;
        int splitPoint = str.indexOf(name);
        if (splitPoint > 0) {
            bool ok;
            QString right = str.mid(splitPoint + name.length());
            Operator op;
            op.value = right.toInt(&ok);
            if (ok) {
                op.type = Operator::Type(i);
                *s = str.left(splitPoint).trimmed();
                return op;
            }
        }
    }
    return Operator();
}

int AbstractMetaBuilder::figureOutEnumValue(const QString &stringValue,
                                        int oldValuevalue,
                                        AbstractMetaEnum *meta_enum,
                                        AbstractMetaFunction *meta_function)
{
    if (stringValue.isEmpty())
        return oldValuevalue;

    QStringList stringValues = stringValue.split("|");

    int returnValue = 0;

    bool matched = false;

    for (int i=0; i<stringValues.size(); ++i) {
        QString s = stringValues.at(i).trimmed();

        bool ok;
        int v;

        Operator op = findOperator(&s);

        if (s.length() > 0 && s.at(0) == QLatin1Char('0'))
            v = s.toUInt(&ok, 0);
        else
            v = s.toInt(&ok);

        if (ok) {
            matched = true;

        } else if (m_enum_values.contains(s)) {
            v = m_enum_values[s]->value();
            matched = true;

        } else {
            AbstractMetaEnumValue *ev = 0;

            if (meta_enum && (ev = meta_enum->values().find(s))) {
                v = ev->value();
                matched = true;

            } else if (meta_enum && (ev = meta_enum->enclosingClass()->findEnumValue(s, meta_enum))) {
                v = ev->value();
                matched = true;

            } else {
                if (meta_enum)
                    ReportHandler::warning("unhandled enum value: " + s + " in "
                                           + meta_enum->enclosingClass()->name() + "::"
                                           + meta_enum->name());
                else
                    ReportHandler::warning("unhandled enum value: Unknown enum");
            }
        }

        if (matched)
            returnValue |= op.calculate(v);
    }

    if (!matched) {
        QString warn = QString("unmatched enum %1").arg(stringValue);

        if (meta_function != 0) {
            warn += QString(" when parsing default value of '%1' in class '%2'")
                .arg(meta_function->name())
                .arg(meta_function->implementingClass()->name());
        }

        ReportHandler::warning(warn);
        returnValue = oldValuevalue;
    }

    return returnValue;
}

void AbstractMetaBuilder::figureOutEnumValuesForClass(AbstractMetaClass *meta_class,
                                                  QSet<AbstractMetaClass *> *classes)
{
    AbstractMetaClass *base = meta_class->baseClass();

    if (base != 0 && !classes->contains(base))
        figureOutEnumValuesForClass(base, classes);

    if (classes->contains(meta_class))
        return;

    AbstractMetaEnumList enums = meta_class->enums();
    foreach (AbstractMetaEnum *e, enums) {
        if (!e) {
            ReportHandler::warning("bad enum in class " + meta_class->name());
            continue;
        }
        AbstractMetaEnumValueList lst = e->values();
        int value = 0;
        for (int i=0; i<lst.size(); ++i) {
            value = figureOutEnumValue(lst.at(i)->stringValue(), value, e);
            lst.at(i)->setValue(value);
            value++;
        }

        // Check for duplicate values...
        EnumTypeEntry *ete = e->typeEntry();
        if (!ete->forceInteger()) {
            QHash<int, AbstractMetaEnumValue *> entries;
            foreach (AbstractMetaEnumValue *v, lst) {

                bool vRejected = ete->isEnumValueRejected(v->name());

                AbstractMetaEnumValue *current = entries.value(v->value());
                if (current) {
                    bool currentRejected = ete->isEnumValueRejected(current->name());
                    if (!currentRejected && !vRejected) {
                        ReportHandler::warning(
                            QString("duplicate enum values: %1::%2, %3 and %4 are %5, already rejected: (%6)")
                            .arg(meta_class->name())
                            .arg(e->name())
                            .arg(v->name())
                            .arg(entries[v->value()]->name())
                            .arg(v->value())
                            .arg(ete->enumValueRejections().join(", ")));
                        continue;
                    }
                }

                if (!vRejected)
                    entries[v->value()] = v;
            }

            // Entries now contain all the original entries, no
            // rejected ones... Use this to generate the enumValueRedirection table.
            foreach (AbstractMetaEnumValue *reject, lst) {
                if (!ete->isEnumValueRejected(reject->name()))
                    continue;

                AbstractMetaEnumValue *used = entries.value(reject->value());
                if (!used) {
                    ReportHandler::warning(
                        QString::fromLatin1("Rejected enum has no alternative...: %1::%2")
                        .arg(meta_class->name())
                        .arg(reject->name()));
                    continue;
                }
                ete->addEnumValueRedirection(reject->name(), used->name());
            }

        }
    }



    *classes += meta_class;
}


void AbstractMetaBuilder::figureOutEnumValues()
{
    // Keep a set of classes that we already traversed. We use this to
    // enforce that we traverse base classes prior to subclasses.
    QSet<AbstractMetaClass *> classes;
    foreach (AbstractMetaClass *c, m_meta_classes) {
        figureOutEnumValuesForClass(c, &classes);
    }
}

void AbstractMetaBuilder::figureOutDefaultEnumArguments()
{
    foreach (AbstractMetaClass *meta_class, m_meta_classes) {
        foreach (AbstractMetaFunction *meta_function, meta_class->functions()) {
            foreach (AbstractMetaArgument *arg, meta_function->arguments()) {

                QString expr = arg->defaultValueExpression();
                if (expr.isEmpty())
                    continue;

                if (!meta_function->replacedDefaultExpression(meta_function->implementingClass(),
                    arg->argumentIndex()+1).isEmpty()) {
                    continue;
                }

                QString new_expr = expr;
                if (arg->type()->isEnum()) {
                    QStringList lst = expr.split(QLatin1String("::"));
                    if (lst.size() == 1) {
                        QVector<AbstractMetaClass *> classes(1, meta_class);
                        AbstractMetaEnum *e = 0;
                        while (!classes.isEmpty() && e == 0) {
                            if (classes.front() != 0) {
                                classes << classes.front()->baseClass();

                                qDebug() << "Search enum value '" << expr << "' in '" << meta_class->name() << "' and its super classes";

                                AbstractMetaClassList interfaces = classes.front()->interfaces();
                                foreach (AbstractMetaClass *interface, interfaces) {
                                    if(classes.front() != interface->primaryInterfaceImplementor())
                                        classes << interface->primaryInterfaceImplementor();
                                }

                                e = classes.front()->findEnumForValue(expr);
                            }

                            classes.pop_front();
                        }

                        if (e != 0) {
                            new_expr = QString("%1.%2")
                                    .arg(e->typeEntry()->qualifiedTargetLangName())
                                    .arg(expr);
                        } else {
                            ReportHandler::warning("Cannot find enum constant for value '" + expr + "' in '" + meta_class->name() + "' or any of its super classes");
                        }
                    } else if (lst.size() == 2) {
                        AbstractMetaClass *cl = m_meta_classes.findClass(lst.at(0));
                        if (!cl) {
                            ReportHandler::warning("missing required class for enums: " + lst.at(0));
                            continue;
                        }
                        new_expr = QString("%1.%2.%3")
                                   .arg(cl->typeEntry()->qualifiedTargetLangName())
                                   .arg(arg->type()->name())
                                   .arg(lst.at(1));
                    } else {
                        ReportHandler::warning("bad default value passed to enum " + expr);
                    }

                } else if(arg->type()->isFlags()) {
                    const FlagsTypeEntry *flagsEntry =
                        static_cast<const FlagsTypeEntry *>(arg->type()->typeEntry());
                    EnumTypeEntry *enumEntry = flagsEntry->originator();
                    AbstractMetaEnum *meta_enum = m_meta_classes.findEnum(enumEntry);
                    if (!meta_enum) {
                        ReportHandler::warning("unknown required enum " + enumEntry->qualifiedCppName());
                        continue;
                    }

                    int value = figureOutEnumValue(expr, 0, meta_enum, meta_function);
                    new_expr = QString::number(value);

                } else if (arg->type()->isPrimitive()) {
                    AbstractMetaEnumValue *value = 0;
                    if (expr.contains("::"))
                        value = m_meta_classes.findEnumValue(expr);
                    if (!value)
                        value = meta_class->findEnumValue(expr, 0);

                    if (value) {
                        new_expr = QString::number(value->value());
                    } else if (expr.contains(QLatin1Char('+'))) {
                        new_expr = QString::number(figureOutEnumValue(expr, 0, 0));

                    }



                }

                arg->setDefaultValueExpression(new_expr);
            }
        }
    }
}


AbstractMetaEnum *AbstractMetaBuilder::traverseEnum(EnumModelItem enum_item, AbstractMetaClass *enclosing, const QSet<QString> &enumsDeclarations)
{
    // Skipping private enums.
    //
    // Qt 6: the typesystem ENTRY is authoritative for binding: an explicit
    // <enum-type> declaration means the enum must attach even when the
    // legacy access tracking mislabels the DOM item (e.g.
    // QStringConverter::Encoding, declared public in Qt 6 headers but
    // reported Private here). Only skip private enums with no entry.
    QString qualified_name = enum_item->qualifiedName().join("::");

    TypeEntry *type_entry = TypeDatabase::instance()->findType(qualified_name);
    if (enum_item->accessPolicy() == CodeModel::Private
        && (type_entry == 0 || !type_entry->isEnum())) {
        return 0;
    }
    QString enum_name = enum_item->name();

    QString class_name;
    if (m_current_class)
        class_name = m_current_class->typeEntry()->qualifiedCppName();

    if (TypeDatabase::instance()->isEnumRejected(class_name, enum_name)) {
        m_rejected_enums.insert(qualified_name, GenerationDisabled);
        return 0;
    }

    if (!type_entry || !type_entry->isEnum()) {
        // Qt 6: enum moved to a base class scope (e.g. QIODevice::OpenModeFlag
        // now declared in QIODeviceBase) while the typesystem keeps the
        // derived qualifier. Retry under the current class before giving up.
        if (m_current_class && m_current_class->typeEntry()) {
            QString derived = m_current_class->typeEntry()->qualifiedCppName()
                              + QLatin1String("::") + enum_name;
            TypeEntry *alt = TypeDatabase::instance()->findType(derived);
            if (alt && alt->isEnum())
                type_entry = alt;
        }
    }

    if (!type_entry || !type_entry->isEnum()) {
        QString context = m_current_class ? m_current_class->name() : QLatin1String("");
        ReportHandler::warning(QString("enum '%1' does not have a type entry or is not an enum")
                               .arg(qualified_name));
        m_rejected_enums.insert(qualified_name, NotInTypeSystem);
       return 0;
    }

    AbstractMetaEnum *meta_enum = createMetaEnum();
    meta_enum->setScoped(enum_item->isScoped());
    if (   enumsDeclarations.contains(qualified_name)
        || enumsDeclarations.contains(enum_name)) {
        meta_enum->setHasQEnumsDeclaration(true);
    }

    meta_enum->setTypeEntry((EnumTypeEntry *) type_entry);
    switch (enum_item->accessPolicy()) {
    case CodeModel::Public: *meta_enum += AbstractMetaAttributes::Public; break;
    case CodeModel::Protected: *meta_enum += AbstractMetaAttributes::Protected; break;
//     case CodeModel::Private: *meta_enum += AbstractMetaAttributes::Private; break;
    default: break;
    }
    // Qt 6: the explicit typesystem entry above already authorized this enum
    // even when the DOM access tracking mislabels it (QStringConverter::
    // Encoding). wasPublic() gates emission (shouldIgnoreEnum), and it reads
    // the ORIGINAL attributes snapshot taken below -- so mark public BEFORE
    // setOriginalAttributes() is called by our callers.
    //
    // setOriginalAttributes() snapshots attributes(), NOT visibility bits
    // alone: callers snapshot AFTER this point, so the Public bit set here
    // would leak in. Instead record the authorization on the TYPE ENTRY side
    // is not possible (entries are shared); the correct minimal change is to
    // make shouldIgnoreEnum() consult isPublic() (current attributes). Since
    // that helper is shared, do BOTH: set Public here AND fix the helper to
    // prefer current attributes (see classgenerator.cpp).
    *meta_enum += AbstractMetaAttributes::Public;

    ReportHandler::debugMedium(QString(" - traversing enum %1").arg(meta_enum->fullName()));

    foreach (EnumeratorModelItem value, enum_item->enumerators()) {

        AbstractMetaEnumValue *meta_enum_value = createMetaEnumValue();
        meta_enum_value->setName(value->name());
        // Deciding the enum value...

        meta_enum_value->setStringValue(value->value());
        meta_enum->addEnumValue(meta_enum_value);

        ReportHandler::debugFull("   - " + meta_enum_value->name() + " = "
                                 + QString::number(meta_enum_value->value()));

        // Add into global register...
        if (enclosing)
            m_enum_values[enclosing->name() + "::" + meta_enum_value->name()] = meta_enum_value;
        else
            m_enum_values[meta_enum_value->name()] = meta_enum_value;
    }

    QFileInfo info(enum_item->fileName());
    meta_enum->typeEntry()->setInclude(Include(Include::IncludePath, info.fileName()));

    // Qt 6: record the first declared enumerator spelled for C++ so shell
    // overrides can yield a failure value when a script override of an
    // out-argument virtual raises an exception (QValidator::Invalid).
    const AbstractMetaEnumValueList enum_values = meta_enum->values();
    if (!enum_values.isEmpty()) {
        const EnumTypeEntry *ete = meta_enum->typeEntry();
        QString literal = meta_enum->isScoped()
            ? ete->qualifiedCppName() + QLatin1String("::") + enum_values.first()->name()
            : ete->qualifier() + QLatin1String("::") + enum_values.first()->name();
        registeredEnumFailureLiterals().insert(ete, literal);
    }

    m_enums << meta_enum;

    return meta_enum;
}

AbstractMetaClass *AbstractMetaBuilder::traverseTypeAlias(TypeAliasModelItem typeAlias)
{
    QString class_name = strip_template_args(typeAlias->name());

    QString full_class_name = class_name;
    // we have an inner class
    if (m_current_class) {
        full_class_name = strip_template_args(m_current_class->typeEntry()->qualifiedCppName())
                          + "::" + full_class_name;
    }

    // If we haven't specified anything for the typedef, then we don't care
    ComplexTypeEntry *type = TypeDatabase::instance()->findComplexType(full_class_name);
    if (type == 0)
        return 0;

    if (type->isObject())
        static_cast<ObjectTypeEntry *>(type)->setQObject(isQObject(strip_template_args(typeAlias->type().qualifiedName().join("::"))));

    AbstractMetaClass *meta_class = createMetaClass();
    meta_class->setTypeAlias(true);
    meta_class->setTypeEntry(type);
    meta_class->setBaseClassNames(QStringList() << typeAlias->type().qualifiedName().join("::"));
    *meta_class += AbstractMetaAttributes::Public;

    // Set the default include file name
    if (!type->include().isValid() && !type->isContainer()) {
        QFileInfo info(typeAlias->fileName());

        type->setInclude(Include(Include::IncludePath, info.fileName()));
    }

    return meta_class;
}

AbstractMetaClass *AbstractMetaBuilder::traverseClass(ClassModelItem class_item)
{
    QString class_name = strip_template_args(class_item->name());
    QString full_class_name = class_name;

    // we have inner an class
    if (m_current_class) {
        full_class_name = strip_template_args(m_current_class->typeEntry()->qualifiedCppName())
                          + "::" + full_class_name;
    }

    ComplexTypeEntry *type = TypeDatabase::instance()->findComplexType(full_class_name);
    RejectReason reason = NoReason;

    if (full_class_name == "QMetaTypeId") {
        // QtScript: record which types have been declared
        int lpos = class_item->name().indexOf('<');
        int rpos = class_item->name().lastIndexOf('>');
        if ((lpos != -1) && (rpos != -1)) {
            QString declared_typename = class_item->name().mid(lpos+1, rpos - lpos-1);
            m_qmetatype_declared_typenames.insert(declared_typename);
        }
    }

    if (TypeDatabase::instance()->isClassRejected(full_class_name)) {
        reason = GenerationDisabled;
    } else if (!type) {
        TypeEntry *te = TypeDatabase::instance()->findType(full_class_name);
        if (te && !te->isComplex())
            reason = RedefinedToNotClass;
        else
            reason = NotInTypeSystem;
    } else if (type->codeGeneration() == TypeEntry::GenerateNothing) {
        reason = GenerationDisabled;
    }

    if (reason != NoReason) {
        m_rejected_classes.insert(full_class_name, reason);
        return 0;
    }

    if (type->isObject()) {
        ((ObjectTypeEntry *)type)->setQObject(isQObject(full_class_name));
    }

    AbstractMetaClass *meta_class = createMetaClass();
    meta_class->setTypeEntry(type);
    meta_class->setBaseClassNames(class_item->baseClasses());
    *meta_class += AbstractMetaAttributes::Public;

    // Qt 6: enums moved to base scopes (e.g. QIODevice::OpenModeFlag in
    // QIODeviceBase) must have their flags aliases registered BEFORE
    // traverseFunctions below, so QFlags-typed arguments written against the
    // base scope resolve during argument translation.
    registerBaseScopeFlagsAliases(class_item, full_class_name);

    AbstractMetaClass *old_current_class = m_current_class;
    m_current_class = meta_class;

    if (type->isContainer()) {
        ReportHandler::debugSparse(QString("container: '%1'").arg(full_class_name));
    } else {
        ReportHandler::debugSparse(QString("class: '%1'").arg(meta_class->fullName()));
    }

    TemplateParameterList template_parameters = class_item->templateParameters();
    QList<TypeEntry *> template_args;
    template_args.clear();
    for (int i=0; i<template_parameters.size(); ++i) {
        const TemplateParameterModelItem &param = template_parameters.at(i);
        TemplateArgumentEntry *param_type = new TemplateArgumentEntry(param->name());
        param_type->setOrdinal(i);
        template_args.append(param_type);
    }
    meta_class->setTemplateArguments(template_args);

    parseQ_Property(meta_class, class_item->propertyDeclarations());

    // Qt 6: C++11 type aliases (e.g. using NetworkLayerProtocol =
    // QAbstractSocket::NetworkLayerProtocol). The alias target's type entry
    // decides the path: if the alias qualifier itself has a typesystem entry
    // (enum-type for a same-named enum alias), record the alias so signatures
    // written against it resolve to the existing target meta class. Otherwise
    // the alias may name a real class (QList<T> specializations etc.) and is
    // handled with the other typedefs below. Registration happens BEFORE
    // traverseFunctions so function signatures can use the alias.
    TypeAliasList earlyAliases = class_item->typeAliases();
    TypeAliasList lateAliases;
    foreach (TypeAliasModelItem earlyAlias, earlyAliases) {
        QString aliasQualified = type->qualifiedCppName()
                                 + QLatin1String("::") + earlyAlias->name();
        TypeEntry *aliasEntry = TypeDatabase::instance()->findType(aliasQualified);
        if (aliasEntry && (aliasEntry->isEnum() || aliasEntry->isFlags())) {
            QString targetName = earlyAlias->type().qualifiedName().join(QLatin1String("::"));
            AbstractMetaClass *targetMeta = m_meta_classes.findClass(targetName);
            if (targetMeta)
                m_aliased_meta_classes.insert(aliasQualified, targetMeta);
            else
                m_pending_alias_targets.insert(aliasQualified, targetName);
        } else {
            lateAliases.append(earlyAlias);
        }
    }

    traverseFunctions(model_dynamic_cast<ScopeModelItem>(class_item), meta_class);
    traverseEnums(model_dynamic_cast<ScopeModelItem>(class_item), meta_class, class_item->enumsDeclarations());
    traverseFields(model_dynamic_cast<ScopeModelItem>(class_item), meta_class);

    // Inner classes
    {
        QList<ClassModelItem> inner_classes = class_item->classMap().values();
        foreach (const ClassModelItem &ci, inner_classes) {
            AbstractMetaClass *cl = traverseClass(ci);
            if (cl) {
                cl->setEnclosingClass(meta_class);
                m_meta_classes << cl;
            }
        }

    }

    // Go through all typedefs to see if we have defined any
    // specific typedefs to be used as classes. (Qt 6: enum-target aliases
    // were already consumed by the early pass above and are excluded here.)
    TypeAliasList typeAliases = lateAliases;
    foreach (TypeAliasModelItem typeAlias, typeAliases) {
        AbstractMetaClass *cls = traverseTypeAlias(typeAlias);
        if (cls != 0) {
            cls->setEnclosingClass(meta_class);
            addAbstractMetaClass(cls);
        }
    }


    m_current_class = old_current_class;

    // Set the default include file name
    if (!type->include().isValid() && !type->isContainer()) {
        QFileInfo info(class_item->fileName());

        type->setInclude(Include(Include::IncludePath, info.fileName()));
    }

    return meta_class;
}

AbstractMetaField *AbstractMetaBuilder::traverseField(VariableModelItem field, const AbstractMetaClass *cls)
{
    QString field_name = field->name();
    QString class_name = m_current_class->typeEntry()->qualifiedCppName();

    // Ignore friend decl.
    if (field->isFriend())
        return 0;

    if (field->accessPolicy() == CodeModel::Private)
        return 0;

    if (TypeDatabase::instance()->isFieldRejected(class_name, field_name)) {
        m_rejected_fields.insert(class_name + "::" + field_name, GenerationDisabled);
        return 0;
    }


    AbstractMetaField *meta_field = createMetaField();
    meta_field->setName(field_name);
    meta_field->setEnclosingClass(cls);

    bool ok;
    TypeInfo field_type = field->type();
    AbstractMetaType *meta_type = translateType(field_type, &ok);

    if (!meta_type || !ok) {
        ReportHandler::warning(QString("skipping field '%1::%2' with unmatched type '%3'")
                               .arg(m_current_class->name())
                               .arg(field_name)
                               .arg(TypeInfo::resolveType(field_type, currentScope()->toItem()).qualifiedName().join("::")));
        delete meta_field;
        return 0;
    }

    meta_field->setType(meta_type);

    uint attr = 0;
    if (field->isStatic())
        attr |= AbstractMetaAttributes::Static;

    CodeModel::AccessPolicy policy = field->accessPolicy();
    if (policy == CodeModel::Public)
        attr |= AbstractMetaAttributes::Public;
    else if (policy == CodeModel::Protected)
        attr |= AbstractMetaAttributes::Protected;
    else
        attr |= AbstractMetaAttributes::Private;
    meta_field->setAttributes(attr);

    return meta_field;
}

void AbstractMetaBuilder::traverseFields(ScopeModelItem scope_item, AbstractMetaClass *meta_class)
{
    foreach (VariableModelItem field, scope_item->variables()) {
        AbstractMetaField *meta_field = traverseField(field, meta_class);

        if (meta_field) {
            meta_field->setOriginalAttributes(meta_field->attributes());
            meta_class->addField(meta_field);
        }
    }
}

void AbstractMetaBuilder::setupFunctionDefaults(AbstractMetaFunction *meta_function, AbstractMetaClass *meta_class)
{
    // Set the default value of the declaring class. This may be changed
    // in fixFunctions later on
    meta_function->setDeclaringClass(meta_class);

    // Some of the queries below depend on the implementing class being set
    // to function properly. Such as function modifications
    meta_function->setImplementingClass(meta_class);

    if (meta_function->name() == "operator_equal")
        meta_class->setHasEqualsOperator(true);

    if (!meta_function->isFinalInTargetLang()
        && meta_function->isRemovedFrom(meta_class, TypeSystem::TargetLangCode)) {
        *meta_function += AbstractMetaAttributes::FinalInCpp;
    }
}

void AbstractMetaBuilder::traverseFunctions(ScopeModelItem scope_item, AbstractMetaClass *meta_class)
{
    foreach (FunctionModelItem function, scope_item->functions()) {
        AbstractMetaFunction *meta_function = traverseFunction(function);

        if (meta_function) {
            meta_function->setOriginalAttributes(meta_function->attributes());
            if (meta_class->isNamespace())
                *meta_function += AbstractMetaAttributes::Static;

            if (QPropertySpec *read = meta_class->propertySpecForRead(meta_function->name())) {
                if (read->type() == meta_function->type()->typeEntry()) {
                    *meta_function += AbstractMetaAttributes::PropertyReader;
                    meta_function->setPropertySpec(read);
//                     printf("%s is reader for %s\n",
//                            qPrintable(meta_function->name()),
//                            qPrintable(read->name()));
                }
            } else if (QPropertySpec *write =
                       meta_class->propertySpecForWrite(meta_function->name())) {
                if (write->type() == meta_function->arguments().at(0)->type()->typeEntry()) {
                    *meta_function += AbstractMetaAttributes::PropertyWriter;
                    meta_function->setPropertySpec(write);
//                     printf("%s is writer for %s\n",
//                            qPrintable(meta_function->name()),
//                            qPrintable(write->name()));
                }
            } else if (QPropertySpec *reset =
                       meta_class->propertySpecForReset(meta_function->name())) {
                *meta_function += AbstractMetaAttributes::PropertyResetter;
                meta_function->setPropertySpec(reset);
//                     printf("%s is resetter for %s\n",
//                            qPrintable(meta_function->name()),
//                            qPrintable(reset->name()));
            }


            // Qt 5 fix: reject move constructors. Qt 5 headers add
            // `X(X &&other)` next to the copy ctor; the parser folds the
            // rvalue-ref parameter into a self-typed value/ref parameter,
            // so the generated shell class gets BOTH `QtScriptShell_X(X)`
            // and `QtScriptShell_X(const X &)` and every forwarding call
            // with a temporary fails with C2668. Script-side, move vs copy
            // is indistinguishable anyway, so drop the extra ctor.
            if (meta_function->isConstructor() && !meta_function->isPrivate()) {
                const AbstractMetaArgumentList &move_check_args = meta_function->arguments();
                if (move_check_args.size() == 1
                    && move_check_args.at(0)->type()->typeEntry() == meta_class->typeEntry()
                    && move_check_args.at(0)->type()->indirections() == 0
                    && !move_check_args.at(0)->type()->isConstant()) {
                    meta_function->setInvalid(true);
                }
            }

            bool isInvalidDestructor = meta_function->isDestructor() && meta_function->isPrivate();
            bool isInvalidConstructor = meta_function->isConstructor()
                && (meta_function->isPrivate() || meta_function->isInvalid());
            // Qt 6: record private-only construction (e.g. QClipboard) so no
            // public default is synthesized later (it would not compile).
            if (meta_function->isConstructor() && meta_function->isPrivate())
                meta_class->setHasOnlyPrivateConstructors(true);
            if ((isInvalidDestructor || isInvalidConstructor)
                && !meta_class->hasNonPrivateConstructor()) {
                *meta_class += AbstractMetaAttributes::Final;
            } else if (meta_function->isConstructor() && !meta_function->isPrivate()) {
                *meta_class -= AbstractMetaAttributes::Final;
                meta_class->setHasNonPrivateConstructor(true);
            }

            // Classes with virtual destructors should always have a shell class
            // (since we aren't registering the destructors, we need this extra check)
            if (meta_function->isDestructor() && !meta_function->isFinal())
                meta_class->setForceShellClass(true);

            if (!meta_function->isDestructor()
                && !meta_function->isInvalid()
                && (!meta_function->isConstructor() || !meta_function->isPrivate())) {

                if (meta_class->typeEntry()->designatedInterface() && !meta_function->isPublic()
                    && !meta_function->isPrivate()) {
                    QString warn = QString("non-public function '%1' in interface '%2'")
                        .arg(meta_function->name()).arg(meta_class->name());
                    ReportHandler::warning(warn);

                    meta_function->setVisibility(AbstractMetaClass::Public);
                }

                setupFunctionDefaults(meta_function, meta_class);

                if (meta_function->isSignal() && meta_class->hasSignal(meta_function)) {
                    QString warn = QString("signal '%1' in class '%2' is overloaded.")
                        .arg(meta_function->name()).arg(meta_class->name());
                    ReportHandler::warning(warn);
                }

                if (meta_function->isSignal() && !meta_class->isQObject()) {
                    QString warn = QString("signal '%1' in non-QObject class '%2'")
                        .arg(meta_function->name()).arg(meta_class->name());
                    ReportHandler::warning(warn);
                }

                meta_class->addFunction(meta_function);
            } else if (meta_function->isDestructor()) {
                meta_class->setDestructorException(meta_function->exception());
                if (!meta_function->isPublic())
                    meta_class->setHasPublicDestructor(false);
            }
        }
    }
}

bool AbstractMetaBuilder::setupInheritance(AbstractMetaClass *meta_class)
{
    Q_ASSERT(!meta_class->isInterface());

    if (m_setup_inheritance_done.contains(meta_class))
        return true;
    m_setup_inheritance_done.insert(meta_class);

    QStringList base_classes = meta_class->baseClassNames();

    TypeDatabase *types = TypeDatabase::instance();

    // we only support our own containers and ONLY if there is only one baseclass
    if (base_classes.size() == 1 && base_classes.first().count('<') == 1) {
        QStringList scope = meta_class->typeEntry()->qualifiedCppName().split("::");
        scope.removeLast();
        for (int i=scope.size(); i>=0; --i) {
            QString prefix = i > 0 ? QStringList(scope.mid(0, i)).join("::") + "::" : QString();
            QString complete_name = prefix + base_classes.first();
            TypeParser::Info info = TypeParser::parse(complete_name);
            QString base_name = info.qualified_name.join("::");

            AbstractMetaClass *templ = 0;
            foreach (AbstractMetaClass *c, m_templates) {
                if (c->typeEntry()->name() == base_name) {
                    templ = c;
                    break;
                }
            }

            if (templ == 0)
                templ = m_meta_classes.findClass(base_name);

            if (templ) {
                setupInheritance(templ);
                inheritTemplate(meta_class, templ, info);
                return true;
            }
        }

        ReportHandler::warning(QString("template baseclass '%1' of '%2' is not known")
                                .arg(base_classes.first())
                                .arg(meta_class->name()));
        return false;
    }

    int primary = -1;
    int primaries = 0;
    for (int i=0; i<base_classes.size(); ++i) {

        if (types->isClassRejected(base_classes.at(i)))
            continue;

        TypeEntry *base_class_entry = types->findType(base_classes.at(i));
        if (!base_class_entry) {
            ReportHandler::warning(QString("class '%1' inherits from unknown base class '%2'")
                                   .arg(meta_class->name()).arg(base_classes.at(i)));
        }

        // true for primary base class
        else if (!base_class_entry->designatedInterface()) {
            if (primaries > 0) {
                ReportHandler::warning(QString("class '%1' has multiple primary base classes"
                                               " '%2' and '%3'")
                                       .arg(meta_class->name())
                                       .arg(base_classes.at(primary))
                                       .arg(base_class_entry->name()));
                return false;
            }
            primaries++;
            primary = i;
        }
    }

    if (primary >= 0) {
        AbstractMetaClass *base_class = m_meta_classes.findClass(base_classes.at(primary));
        if (!base_class) {
            ReportHandler::warning(QString("unknown baseclass for '%1': '%2'")
                                   .arg(meta_class->name())
                                   .arg(base_classes.at(primary)));
            return false;
        }
        meta_class->setBaseClass(base_class);

        if (meta_class->typeEntry()->designatedInterface() != 0 && meta_class->isQObject()) {
            ReportHandler::warning(QString("QObject extended by interface type '%1'. This is not supported and the generated Java code will not compile.")
                                   .arg(meta_class->name()));
        } else if (meta_class->typeEntry()->designatedInterface() != 0 && base_class != 0 && !base_class->isInterface()) {
            ReportHandler::warning(QString("object type '%1' extended by interface type '%2'. The resulting API will be less expressive than the original.")
                                   .arg(base_class->name())
                                   .arg(meta_class->name()));
        }

    }

    for (int i=0; i<base_classes.size(); ++i) {
        if (types->isClassRejected(base_classes.at(i)))
            continue;

        if (i != primary) {
            AbstractMetaClass *base_class = m_meta_classes.findClass(base_classes.at(i));
            if (base_class == 0) {
                ReportHandler::warning(QString("class not found for setup inheritance '%1'").arg(base_classes.at(i)));
                return false;
            }

            setupInheritance(base_class);

            QString interface_name = InterfaceTypeEntry::interfaceName(base_class->name());
            AbstractMetaClass *iface = m_meta_classes.findClass(interface_name);
            if (!iface) {
                ReportHandler::warning(QString("unknown interface for '%1': '%2'")
                                       .arg(meta_class->name())
                                       .arg(interface_name));
                return false;
            }
            meta_class->addInterface(iface);

            AbstractMetaClassList interfaces = iface->interfaces();
            foreach (AbstractMetaClass *iface, interfaces)
                meta_class->addInterface(iface);
        }
    }

    return true;
}

void AbstractMetaBuilder::registerBaseScopeFlagsAliases(ClassModelItem class_item,
                                                           const QString &full_class_name)
{
    // Qt 6: enums moved to base scopes keep derived-qualified typesystem
    // entries. Register the flags type under the base scope qualifier as well
    // (e.g. QIODeviceBase::OpenMode alongside QIODevice::OpenMode) so
    // arguments written against either scope resolve to the same entry.
    QSet<QString> seen;
    QStringList stack = class_item->baseClasses();
    QSet<QString> visited;
    while (!stack.isEmpty()) {
        QString baseName = stack.takeFirst();
        if (visited.contains(baseName))
            continue;
        visited.insert(baseName);
        ClassModelItem baseItem = m_dom->findClass(baseName);
        if (!baseItem)
            continue;
        foreach (QString b, baseItem->baseClasses())
            stack.append(b);
        foreach (EnumModelItem enum_item, baseItem->enums()) {
            if (seen.contains(enum_item->name()))
                continue;
            seen.insert(enum_item->name());
            QString entryName = full_class_name + QLatin1String("::") + enum_item->name();
            TypeEntry *te = TypeDatabase::instance()->findType(entryName);
            if (!te || !te->isEnum())
                continue;
            FlagsTypeEntry *fte = static_cast<EnumTypeEntry *>(te)->flags();
            if (!fte)
                continue;
            QStringList qn = enum_item->qualifiedName();
            QString baseScope = QStringList(qn.mid(0, qn.size() - 1)).join("::");
            QString baseFlagsName = baseScope + QLatin1String("::") + fte->flagsName();
            if (TypeDatabase::instance()->findFlagsType(baseFlagsName) == 0) {
                FlagsTypeEntry *alias = new FlagsTypeEntry(*fte);
                alias->setOriginalName(baseFlagsName);
                TypeDatabase::instance()->addFlagsType(alias);
            }
        }
    }
}

void AbstractMetaBuilder::traverseEnums(ScopeModelItem scope_item, AbstractMetaClass *meta_class, const QStringList &enumsDeclarations)
{
    EnumList enums = scope_item->enums();
    foreach (EnumModelItem enum_item, enums) {
        AbstractMetaEnum *meta_enum = traverseEnum(enum_item, meta_class, QSet<QString>(enumsDeclarations.begin(), enumsDeclarations.end()));
        if (meta_enum) {
            meta_enum->setOriginalAttributes(meta_enum->attributes());
            meta_class->addEnum(meta_enum);
            meta_enum->setEnclosingClass(meta_class);
        }
    }

    // Qt 6: several enums moved to new base classes (e.g. QIODevice::OpenModeFlag
    // now lives in QIODeviceBase) while the typesystem keeps the derived qualified
    // name (which C++ still resolves through inheritance). Look up base scopes
    // for enums that have an entry qualified under this class but no declaration
    // in this scope, so the script class keeps its historical placement.
    //
    // The walk below only covers the ancestors of the class being traversed.
    // When a multi-module build loads another module with generate="no" (e.g.
    // network loading core), the alias entries exist but the holder classes
    // never traverse, so their aliases never register. Repair that lazily in
    // translateType (step 6b): if a QFlags lookup fails, retry it under every
    // known enum entry whose unqualified enum name matches.
    //
    // Qt 6 also spells some enums as using-aliases for another scope's enum
    // (e.g. QHostAddress::NetworkLayerProtocol for
    // QAbstractSocket::NetworkLayerProtocol). The alias scope is not an
    // ancestor, so the same entry lookup applies here: when the derived
    // qualifier has an enum entry, traverse the base enum's declaration under
    // this class.
    if (meta_class && meta_class->typeEntry() && meta_class->typeEntry()->isComplex()) {
        QString className = meta_class->typeEntry()->qualifiedCppName();
        QSet<QString> seen;
        foreach (AbstractMetaEnum *e, meta_class->enums())
            seen.insert(e->name());
        // Seed the walk with real bases plus the alias-holder itself, so its
        // own type aliases resolve to their targets below.
        QStringList stack = meta_class->baseClassNames();
        stack.append(className);
        QSet<QString> visited;
        while (!stack.isEmpty()) {
            QString baseName = stack.takeFirst();
            if (visited.contains(baseName))
                continue;
            visited.insert(baseName);
            ClassModelItem baseItem = m_dom->findClass(baseName);
            if (!baseItem)
                continue;
            foreach (QString b, baseItem->baseClasses())
                stack.append(b);
            foreach (EnumModelItem enum_item, baseItem->enums()) {
                QString entryName = className + QLatin1String("::") + enum_item->name();
                TypeEntry *te = TypeDatabase::instance()->findType(entryName);
                if (!te || !te->isEnum())
                    continue;
                if (seen.contains(enum_item->name()))
                    continue;
                seen.insert(enum_item->name());
                AbstractMetaEnum *meta_enum = traverseEnum(enum_item, meta_class,
                    QSet<QString>(enumsDeclarations.begin(), enumsDeclarations.end()));
                if (meta_enum) {
                    meta_enum->setOriginalAttributes(meta_enum->attributes());
                    meta_class->addEnum(meta_enum);
                    meta_enum->setEnclosingClass(meta_class);
                }
            }
            // Qt 6 using-alias enums: resolve alias targets declared in this
            // scope (e.g. QHostAddress::NetworkLayerProtocol =
            // QAbstractSocket::NetworkLayerProtocol) and traverse the target
            // declaration when the alias qualifier has an enum entry.
            foreach (TypeAliasModelItem alias_item, baseItem->typeAliases()) {
                QString entryName = className + QLatin1String("::") + alias_item->name();
                TypeEntry *te = TypeDatabase::instance()->findType(entryName);
                if (!te || !te->isEnum())
                    continue;
                // The alias target must be a real enum declaration somewhere.
                QString target = alias_item->type().qualifiedName().join(QLatin1String("::"));
                int sep = target.lastIndexOf(QLatin1String("::"));
                if (sep <= 0)
                    continue;
                QString targetScope = target.left(sep);
                QString targetName = target.mid(sep + 2);
                ClassModelItem targetItem = m_dom->findClass(targetScope);
                if (!targetItem)
                    continue;
                foreach (EnumModelItem enum_item, targetItem->enums()) {
                    if (enum_item->name() != targetName)
                        continue;
                    if (seen.contains(alias_item->name()))
                        continue;
                    seen.insert(alias_item->name());
                    AbstractMetaEnum *meta_enum = traverseEnum(enum_item, meta_class,
                        QSet<QString>(enumsDeclarations.begin(), enumsDeclarations.end()));
                    if (meta_enum) {
                        meta_enum->setOriginalAttributes(meta_enum->attributes());
                        meta_class->addEnum(meta_enum);
                        meta_enum->setEnclosingClass(meta_class);
                    }
                }
            }
        }
    }
}

AbstractMetaFunction *AbstractMetaBuilder::traverseFunction(FunctionModelItem function_item)
{
    QString function_name = function_item->name();
    QString class_name = m_current_class->typeEntry()->qualifiedCppName();

    if (TypeDatabase::instance()->isFunctionRejected(class_name, function_name)) {
        m_rejected_functions.insert(class_name + "::" + function_name, GenerationDisabled);
        return 0;
    }

    // Qt 6: Q_GADGET declares a static-assert helper (qt_check_for_QGADGET_macro)
    // in every gadget class. It has no definition to link against and is never
    // script API; drop it universally rather than per-class in typesystems.
    if (function_name == QLatin1String("qt_check_for_QGADGET_macro")) {
        m_rejected_functions.insert(class_name + "::" + function_name, GenerationDisabled);
        return 0;
    }


    Q_ASSERT(function_item->functionType() == CodeModel::Normal
             || function_item->functionType() == CodeModel::Signal
             || function_item->functionType() == CodeModel::Slot);

    if (function_item->isFriend())
        return 0;

    // Qt 6: hidden friends (e.g. constexpr friend inline QVector2D
    // operator+(QVector2D, QVector2D)) take the enclosing class as their
    // first argument but bind as free functions, not members. The legacy
    // binder loses the friend flag on such inline definitions, so detect
    // the shape here: a non-static function whose FIRST argument has the
    // same type as the enclosing class is really a hidden friend -- mark it
    // Friendly so the emitter calls it as a free function (see
    // writeFunctionCallAndReturn), matching Qt 5 behavior where these were
    // skipped or bound correctly.
    // NOTE: member binary operators (QPoint::operator+(QPoint)) share the
    // shape; they are genuine members and must NOT match. Hidden friends
    // declare TWO explicit parameters (no implicit this), members declare
    // ONE. Only treat 2+-argument non-static operators as friends.


    QString cast_type;

    if (function_name.startsWith("operator")) {
        const QString original_operator_name = function_name;
        function_name = rename_operator(function_name.mid(8));
        if (function_name.isEmpty()) {
            m_rejected_functions.insert(class_name + "::" + original_operator_name,
                                        GenerationDisabled);
            return 0;
        }
        if (function_name.contains("_cast_"))
            cast_type = function_name.mid(14).trimmed();
    }

    AbstractMetaFunction *meta_function = createMetaFunction();
    meta_function->setConstant(function_item->isConstant());
    meta_function->setException(function_item->exception());

    ReportHandler::debugMedium(QString(" - %2()").arg(function_name));

    meta_function->setName(function_name);
    meta_function->setOriginalName(function_item->name());

    if (function_item->isAbstract())
        *meta_function += AbstractMetaAttributes::Abstract;

    if (!meta_function->isAbstract())
        *meta_function += AbstractMetaAttributes::Native;

    if (!function_item->isVirtual() && !meta_function->isAbstract())
        *meta_function += AbstractMetaAttributes::Final;

    // Qt 6: a virtual function declared 'final' in C++ cannot be overridden by
    // the generated shell either (the shell derives from the C++ class, and
    // MSVC rejects such an override with C3248). Marking it Final removes it
    // from the shell's virtual list while keeping it callable from script
    // (e.g. QXmlStreamEntityResolver::resolveEntity, final since Qt 6.12).
    if (function_item->isFinal())
        *meta_function += AbstractMetaAttributes::Final;

    if (function_item->isInvokable())
        *meta_function += AbstractMetaAttributes::Invokable;

    if (function_item->isStatic()) {
        *meta_function += AbstractMetaAttributes::Static;
        *meta_function += AbstractMetaAttributes::Final;
    }

    // Access rights
    if (function_item->accessPolicy() == CodeModel::Public)
        *meta_function += AbstractMetaAttributes::Public;
    else if (function_item->accessPolicy() == CodeModel::Private)
        *meta_function += AbstractMetaAttributes::Private;
    else
        *meta_function += AbstractMetaAttributes::Protected;


    QString stripped_class_name = class_name;
    int cc_pos = stripped_class_name.lastIndexOf("::");
    if (cc_pos > 0)
        stripped_class_name = stripped_class_name.mid(cc_pos + 2);

    TypeInfo function_type = function_item->type();
    if (function_name.startsWith('~')) {
        meta_function->setFunctionType(AbstractMetaFunction::DestructorFunction);
        meta_function->setInvalid(true);
    } else if (strip_template_args(function_name) == stripped_class_name) {
        meta_function->setFunctionType(AbstractMetaFunction::ConstructorFunction);
        meta_function->setName(m_current_class->name());
    } else {
        bool ok;
        AbstractMetaType *type = 0;

        if (!cast_type.isEmpty()) {
            TypeInfo info;
            info.setQualifiedName(QStringList(cast_type));
            type = translateType(info, &ok);
        } else {
            type = translateType(function_type, &ok);
            // Qt 6: the return/argument spelling may be a same-named
            // using-alias recorded for this class (e.g. QHostAddress::
            // NetworkLayerProtocol for QAbstractSocket::
            // NetworkLayerProtocol). Retry against the alias target entry.
            if ((!type || !ok) && m_current_class) {
                QStringList rqn = function_type.qualifiedName();
                QString shortName = rqn.isEmpty() ? QString() : rqn.last();
                QString aliasKey = m_current_class->qualifiedCppName()
                                   + QLatin1String("::") + shortName;
                if (!shortName.isEmpty()
                    && m_aliased_meta_classes.contains(aliasKey)) {
                    const TypeEntry *targetEntry =
                        m_aliased_meta_classes.value(aliasKey)->typeEntry();
                    // Rebuild the query with the target's qualification.
                    TypeInfo targetInfo = function_type;
                    QStringList tqn = targetEntry->qualifiedCppName().split(
                        QLatin1String("::"));
                    if (!tqn.isEmpty()) {
                        targetInfo.setQualifiedName(tqn);
                        type = translateType(targetInfo, &ok);
                    }
                }
            }
        }

        if (!ok) {
            ReportHandler::warning(QString("skipping function '%1::%2', unmatched return type '%3'")
                                   .arg(class_name)
                                   .arg(function_item->name())
                                   .arg(function_item->type().toString()));
            m_rejected_functions[class_name + "::" + function_name] =
                UnmatchedReturnType;
            meta_function->setInvalid(true);
            return meta_function;
        }
        meta_function->setType(type);

        if (function_item->functionType() == CodeModel::Signal)
            meta_function->setFunctionType(AbstractMetaFunction::SignalFunction);
        else if (function_item->functionType() == CodeModel::Slot)
            meta_function->setFunctionType(AbstractMetaFunction::SlotFunction);
    }

    ArgumentList arguments = function_item->arguments();
    AbstractMetaArgumentList meta_arguments;

    int first_default_argument = 0;
    for (int i=0; i<arguments.size(); ++i) {
        ArgumentModelItem arg = arguments.at(i);

        bool ok;
        AbstractMetaType *meta_type = translateType(arg->type(), &ok);
        if (!meta_type || !ok) {
            ReportHandler::warning(QString("skipping function '%1::%2', "
                                           "unmatched parameter type '%3'")
                                   .arg(class_name)
                                   .arg(function_item->name())
                                   .arg(arg->type().toString()));
            m_rejected_functions[class_name + "::" + function_name] =
                UnmatchedArgumentType;
            meta_function->setInvalid(true);
            return meta_function;
        }
        AbstractMetaArgument *meta_argument = createMetaArgument();
        meta_argument->setType(meta_type);
        meta_argument->setName(arg->name());
        meta_argument->setArgumentIndex(i);
        meta_arguments << meta_argument;
    }

    meta_function->setArguments(meta_arguments);

    // Find the correct default values
    for (int i=0; i<arguments.size(); ++i) {
        ArgumentModelItem arg = arguments.at(i);
        AbstractMetaArgument *meta_arg = meta_arguments.at(i);
        // Qt 6: a replace-default-expression can ADD a default where C++
        // declares none (e.g. QInputEvent's device defaulting to the primary
        // keyboard); enter processing when a replacement exists even without
        // a DOM default.
        bool hasReplaced = !meta_function->replacedDefaultExpression(m_current_class, i + 1).isEmpty();
        if (arg->defaultValue() || hasReplaced) {
            QString expr = arg->defaultValueExpression();
            if (!expr.isEmpty())
                meta_arg->setOriginalDefaultValueExpression(expr);

            expr = translateDefaultValue(arg, meta_arg->type(), meta_function, m_current_class, i);
            if (expr.isEmpty()) {
                first_default_argument = i;
            } else {
                meta_arg->setDefaultValueExpression(expr);
            }

            if (meta_arg->type()->isEnum() || meta_arg->type()->isFlags()) {
                m_enum_default_arguments
                    << QPair<AbstractMetaArgument *, AbstractMetaFunction *>(meta_arg, meta_function);
            }

        }
    }

    // If we where not able to translate the default argument make it
    // reset all default arguments before this one too.
    for (int i=0; i<first_default_argument; ++i)
        meta_arguments[i]->setDefaultValueExpression(QString());

    if (ReportHandler::debugLevel() == ReportHandler::FullDebug)
        foreach(AbstractMetaArgument *arg, meta_arguments)
            ReportHandler::debugFull("   - " + arg->toString());

    // Qt 6: hidden friends (e.g. constexpr friend inline QVector2D
    // operator+(QVector2D, QVector2D)) bind as free functions, not members,
    // but the legacy binder loses the friend flag on such inline definitions
    // (they surface as members, which cannot compile). Detect the shapes and
    // convert to free calls (drop the self argument, mark Friendly; see
    // writeFunctionCallAndReturn):
    // - class-first binary (vec OP other): drop arg 0;
    // - unary plus/minus (single class argument): drop it (0 args remain);
    // - reversed scalar multiply (primitive, class): drop arg 1 (scalar
    //   multiplication commutes, so self-first emission is equivalent).
    // Genuine members (operator(), operator[], member binary ops with a
    // single explicit argument) never match: binary member form has exactly
    // one explicit argument whose... (see below). Stream operators keep
    // their dedicated handling and are excluded.
    if (!function_item->isStatic() && !meta_function->isStatic()
        && (function_name == QLatin1String("operator_add")
            || function_name == QLatin1String("operator_subtract")
            || function_name == QLatin1String("operator_multiply")
            || function_name == QLatin1String("operator_divide")
            || function_name == QLatin1String("operator_equal")
            || function_name == QLatin1String("operator_not_equal"))) {
        // Type entries are stored undecorated (no const/&), so direct
        // qualified-name comparison detects the self argument.
        QString selfName = m_current_class ? m_current_class->typeEntry()->qualifiedCppName() : QString();
        auto argClassName = [](AbstractMetaArgument *metaArg) -> QString {
            AbstractMetaType *t = metaArg ? metaArg->type() : 0;
            return (t && t->typeEntry()) ? t->typeEntry()->qualifiedCppName() : QString();
        };
        int dropIndex = -1;
        if (meta_arguments.size() >= 2 && !selfName.isEmpty()
            && argClassName(meta_arguments.at(0)) == selfName) {
            // Class-first binary form (member binary ops have a single
            // explicit argument and never reach this branch).
            dropIndex = 0;
        } else if (meta_arguments.size() == 1 && !selfName.isEmpty()
                   && argClassName(meta_arguments.at(0)) == selfName
                   && (function_name == QLatin1String("operator_add")
                       || function_name == QLatin1String("operator_subtract"))
                   && !meta_function->isConstant()) {
            // Unary plus/minus friend (Qt 6 vectors etc.). Free functions
            // cannot be const-qualified, so a const single-argument operator
            // is a genuine member binary (e.g. QRegion::operator+ const) and
            // stays untouched. The build verifies each conversion compiles.
            dropIndex = 0;
        } else if (meta_arguments.size() >= 2
                   && function_name == QLatin1String("operator_multiply")) {
            // Reversed scalar form, e.g. operator*(float, QVector2D).
            AbstractMetaType *secondType = meta_arguments.at(1)->type();
            QString secondName = (secondType && secondType->typeEntry())
                ? secondType->typeEntry()->qualifiedCppName() : QString();
            AbstractMetaType *firstType = meta_arguments.at(0)->type();
            if (!selfName.isEmpty() && secondName == selfName && firstType
                && firstType->typeEntry() && firstType->typeEntry()->isPrimitive())
                dropIndex = 1;
        }
        if (dropIndex >= 0) {
            delete meta_arguments.takeAt(dropIndex);
            for (int ai = 0; ai < meta_arguments.size(); ++ai)
                meta_arguments[ai]->setArgumentIndex(ai);
            meta_function->setArguments(meta_arguments);
            *meta_function += AbstractMetaAttributes::Friendly;
        }
    }

    return meta_function;
}


AbstractMetaType *AbstractMetaBuilder::translateType(const TypeInfo &_typei, bool *ok, bool resolveType, bool resolveScope)
{
    Q_ASSERT(ok);
    *ok = true;

    // 1. Test the type info without resolving typedefs in case this is present in the
    //    type system
    TypeInfo typei;
    if (resolveType) {
        bool isok;
        AbstractMetaType *t = translateType(_typei, &isok, false, resolveScope);
        if (t != 0 && isok)
            return t;
    }

    if (!resolveType)
        typei = _typei;
    else {
        // Go through all parts of the current scope (including global namespace)
        // to resolve typedefs. The parser does not properly resolve typedefs in
        // the global scope when they are referenced from inside a namespace.
        // This is a work around to fix this bug since fixing it in resolveType
        // seemed non-trivial
        int i = m_scopes.size() - 1;
        while (i >= 0) {
            typei = TypeInfo::resolveType(_typei, m_scopes.at(i--)->toItem());
            if (typei.qualifiedName().join("::") != _typei.qualifiedName().join("::"))
                break;
        }

    }

    if (typei.isFunctionPointer()) {
        *ok = false;
        return 0;
    }

    TypeParser::Info typeInfo = TypeParser::parse(typei.toString());
    if (typeInfo.is_busted) {
        *ok = false;
        return 0;
    }

    // 2. Handle pointers specified as arrays with unspecified size
    bool array_of_unspecified_size = false;
    if (typeInfo.arrays.size() > 0) {
        array_of_unspecified_size = true;
        for (int i=0; i<typeInfo.arrays.size(); ++i)
            array_of_unspecified_size = array_of_unspecified_size && typeInfo.arrays.at(i).isEmpty();

        if (!array_of_unspecified_size) {
            TypeInfo newInfo;
            //newInfo.setArguments(typei.arguments());
            newInfo.setIndirections(typei.indirections());
            newInfo.setConstant(typei.isConstant());
            newInfo.setFunctionPointer(typei.isFunctionPointer());
            newInfo.setQualifiedName(typei.qualifiedName());
            newInfo.setReference(typei.isReference());
            newInfo.setVolatile(typei.isVolatile());

            AbstractMetaType *elementType = translateType(newInfo, ok);
            if (!(*ok))
                return 0;

            for (int i=typeInfo.arrays.size()-1; i>=0; --i) {
                QString s = typeInfo.arrays.at(i);
                bool isok;

                int elems = s.toInt(&isok);
                if (!isok)
                    return 0;

                AbstractMetaType *arrayType = createMetaType();
                arrayType->setArrayElementCount(elems);
                arrayType->setArrayElementType(elementType);
                arrayType->setTypeEntry(new ArrayTypeEntry(elementType->typeEntry()));
                decideUsagePattern(arrayType);

                elementType = arrayType;
            }

            return elementType;
        }  else {
            typeInfo.indirections += typeInfo.arrays.size();
        }
    }

    QStringList qualifier_list = typeInfo.qualified_name;
    if (qualifier_list.isEmpty()) {
        ReportHandler::warning(QString("horribly broken type '%1'").arg(_typei.toString()));
        *ok = false;
        return 0;
    }

    QString qualified_name = qualifier_list.join("::");
    QString name = qualifier_list.takeLast();

    // 3. Special case 'void' type
    if (name == "void" && typeInfo.indirections == 0) {
        return 0;
    }

    // 4. Special case QFlags (include instantiation in name)
    if (qualified_name == "QFlags")
        qualified_name = typeInfo.toString();

    // 5. Try to find the type
    const TypeEntry *type = TypeDatabase::instance()->findType(qualified_name);

    // 5b. Qt 6: C++11 using-alias to an already-traversed class (e.g.
    // QHostAddress::NetworkLayerProtocol -> QAbstractSocket). Aliases are
    // recorded during class traversal; reuse the target's type entry.
    if (!type && m_current_class) {
        QStringList contexts;
        contexts.append(m_current_class->qualifiedCppName());
        foreach (const QString &ctx, contexts) {
            QString aliasKey = ctx + QLatin1String("::") + qualified_name;
            if (m_aliased_meta_classes.contains(aliasKey)) {
                type = m_aliased_meta_classes.value(aliasKey)->typeEntry();
                break;
            }
            // Also try the progressively-stripped name: the alias may have
            // been recorded under a longer qualifier chain.
            int sep = qualified_name.lastIndexOf(QLatin1String("::"));
            QString shortName = (sep >= 0) ? qualified_name.mid(sep + 2) : qualified_name;
            if (shortName != qualified_name) {
                aliasKey = ctx + QLatin1String("::") + shortName;
                if (m_aliased_meta_classes.contains(aliasKey)) {
                    type = m_aliased_meta_classes.value(aliasKey)->typeEntry();
                    break;
                }
            }
        }
    }

    // 6. No? Try looking it up as a flags type
    if (!type)
        type = TypeDatabase::instance()->findFlagsType(qualified_name);

    // (6b moved after step 9: scoped resolution must precede global
    // fuzzy matching. See below.)

    // 7. No? Try looking it up as a container type
    if (!type)
        type = TypeDatabase::instance()->findContainerType(name);

    // 8. No? Check if the current class is a template and this type is one
    //    of the parameters.
    if (type == 0 && m_current_class != 0) {
        QList<TypeEntry *> template_args = m_current_class->templateArguments();
        foreach (TypeEntry *te, template_args) {
            if (te->name() == qualified_name)
                type = te;
        }
    }

    // 9. Try finding the type by prefixing it with the current
    //    context and all baseclasses of the current context
    if (!type && !TypeDatabase::instance()->isClassRejected(qualified_name) && m_current_class != 0 && resolveScope) {
        QStringList contexts;
        contexts.append(m_current_class->qualifiedCppName());
        contexts.append(currentScope()->qualifiedName().join("::"));


        TypeInfo info = typei;
        bool subclasses_done = false;
        while (!contexts.isEmpty() && type == 0) {
            //type = TypeDatabase::instance()->findType(contexts.at(0) + "::" + qualified_name);

            bool isok;
            info.setQualifiedName(QStringList() << contexts.at(0) << qualified_name);
            AbstractMetaType *t = translateType(info, &isok, true, false);
            if (t != 0 && isok)
                return t;

            ClassModelItem item = m_dom->findClass(contexts.at(0));
            if (item)
                contexts += item->baseClasses();
            contexts.pop_front();

            // 10. Last resort: Special cased prefix of Qt namespace since the meta object implicitly inherits this, so
            //     enum types from there may be addressed without any scope resolution in properties.
            if (contexts.size() == 0 && !subclasses_done) {
                contexts << "Qt";
                subclasses_done = true;
            }
        }

    }

    // 6b. Qt 6: moved-enum flags aliases (see note at step 6 for why this
    // runs LAST, after scoped step 9). When a build loads the holder's
    // module with generate="no", the per-class alias pass never runs for the
    // holder, so a base-scope spelling (e.g. QIODeviceBase::OpenMode from a
    // network class) finds nothing above. Retry under every known enum entry
    // whose unqualified enum name matches, and register the alias so later
    // lookups hit step 6 directly.
    //
    // The requested qualifier may name the flags typedef directly
    // (QIODeviceBase::OpenMode), the enum itself (QIODeviceBase::
    // OpenModeFlag, used when a signature was written against the enum but
    // the entry models the flags twin), or a QFlags<...> instantiation.
    //
    // Qt 6: TWO passes, enum-name first. A flags-typedef-only match can
    // hijack an unrelated scope: the enum-name pass finds the true owner
    // when one exists.
    if (!type) {
        int sep = qualified_name.lastIndexOf(QLatin1String("::"));
        QString shortEnum = (sep >= 0) ? qualified_name.mid(sep + 2) : qualified_name;
        SingleTypeEntryHash allEntries = TypeDatabase::instance()->entries();
        for (int pass = 0; pass < 2 && !type; ++pass) {
        foreach (TypeEntry *candidate, allEntries.values()) {
            if (candidate == 0 || !candidate->isEnum())
                continue;
            QString candName = candidate->qualifiedCppName();
            int csep = candName.lastIndexOf(QLatin1String("::"));
            QString candShort = (csep >= 0) ? candName.mid(csep + 2) : candName;
            const EnumTypeEntry *enumEntry = static_cast<const EnumTypeEntry *>(candidate);
            FlagsTypeEntry *fte = enumEntry->flags();
            QString flagsShort;
            if (fte) {
                QString flagsName = fte->originalName();
                int fsep = flagsName.lastIndexOf(QLatin1String("::"));
                flagsShort = (fsep >= 0) ? flagsName.mid(fsep + 2) : flagsName;
            }
            if (pass == 0 && shortEnum != candShort)
                continue;
            if (pass == 1 && (shortEnum == candShort || shortEnum != flagsShort))
                continue;
            if (!fte)
                continue;
            if (TypeDatabase::instance()->findFlagsType(qualified_name) == 0) {
                FlagsTypeEntry *alias = new FlagsTypeEntry(*fte);
                alias->setOriginalName(qualified_name);
                TypeDatabase::instance()->addFlagsType(alias);
            }
            type = TypeDatabase::instance()->findFlagsType(qualified_name);
            break;
        }
        }
    }

    if (!type) {
        *ok = false;
        return 0;
    }

    // Used to for diagnostics later...
    m_used_types << type;

    // These are only implicit and should not appear in code...
    Q_ASSERT(!type->isInterface());

    AbstractMetaType *meta_type = createMetaType();
    meta_type->setTypeEntry(type);
    meta_type->setIndirections(typeInfo.indirections);
    meta_type->setReference(typeInfo.is_reference);
    meta_type->setConstant(typeInfo.is_constant);
    meta_type->setOriginalTypeDescription(_typei.toString());
    decideUsagePattern(meta_type);

    if (meta_type->typeEntry()->isContainer()) {
        ContainerTypeEntry::Type container_type = static_cast<const ContainerTypeEntry *>(type)->type();

        if (container_type == ContainerTypeEntry::StringListContainer) {
            TypeInfo info;
            info.setQualifiedName(QStringList() << "QString");
            AbstractMetaType *targ_type = translateType(info, ok);

            Q_ASSERT(*ok);
            Q_ASSERT(targ_type);

            meta_type->addInstantiation(targ_type);
            meta_type->setInstantiationInCpp(false);

        } else {
            foreach (const TypeParser::Info &ta, typeInfo.template_instantiations) {
                TypeInfo info;
                info.setConstant(ta.is_constant);
                info.setReference(ta.is_reference);
                info.setIndirections(ta.indirections);

                info.setFunctionPointer(false);
                info.setQualifiedName(ta.instantiationName().split("::"));

                AbstractMetaType *targ_type = translateType(info, ok);
                if (!(*ok)) {
                    delete meta_type;
                    return 0;
                }

                meta_type->addInstantiation(targ_type);
            }
        }

        if (container_type == ContainerTypeEntry::ListContainer
            || container_type == ContainerTypeEntry::VectorContainer
            || container_type == ContainerTypeEntry::StringListContainer) {
            Q_ASSERT(meta_type->instantiations().size() == 1);
        }
    }

    return meta_type;
}

void AbstractMetaBuilder::decideUsagePattern(AbstractMetaType *meta_type)
{
    const TypeEntry *type = meta_type->typeEntry();

    if (type->isPrimitive() && (meta_type->actualIndirections() == 0
                                || (meta_type->isConstant() && meta_type->isReference() && meta_type->indirections() == 0))) {
        meta_type->setTypeUsagePattern(AbstractMetaType::PrimitivePattern);

    } else if (type->isVoid()) {
        meta_type->setTypeUsagePattern(AbstractMetaType::NativePointerPattern);

    } else if (type->isString()
               && meta_type->indirections() == 0
               && (meta_type->isConstant() == meta_type->isReference()
                   || meta_type->isConstant())) {
        meta_type->setTypeUsagePattern(AbstractMetaType::StringPattern);

    } else if (type->isChar()
        && meta_type->indirections() == 0
        && meta_type->isConstant() == meta_type->isReference()) {
        meta_type->setTypeUsagePattern(AbstractMetaType::CharPattern);

    } else if (type->isJObjectWrapper()
        && meta_type->indirections() == 0
        && meta_type->isConstant() == meta_type->isReference()) {
        meta_type->setTypeUsagePattern(AbstractMetaType::JObjectWrapperPattern);

    } else if (type->isVariant()
        && meta_type->indirections() == 0
        && meta_type->isConstant() == meta_type->isReference()) {
        meta_type->setTypeUsagePattern(AbstractMetaType::VariantPattern);

    } else if (type->isEnum() && meta_type->actualIndirections() == 0) {
        meta_type->setTypeUsagePattern(AbstractMetaType::EnumPattern);

    } else if (type->isObject()
                && meta_type->indirections() == 0
                && meta_type->isReference()) {
        if (((ComplexTypeEntry *) type)->isQObject())
            meta_type->setTypeUsagePattern(AbstractMetaType::QObjectPattern);
        else
            meta_type->setTypeUsagePattern(AbstractMetaType::ObjectPattern);

    } else if (type->isObject()
               && meta_type->indirections() == 1) {
        if (((ComplexTypeEntry *) type)->isQObject())
            meta_type->setTypeUsagePattern(AbstractMetaType::QObjectPattern);
        else
            meta_type->setTypeUsagePattern(AbstractMetaType::ObjectPattern);

        // const-references to pointers can be passed as pointers
        if (meta_type->isReference() && meta_type->isConstant()) {
            meta_type->setReference(false);
            meta_type->setConstant(false);
        }

    } else if (type->isContainer() && meta_type->indirections() == 0) {
        meta_type->setTypeUsagePattern(AbstractMetaType::ContainerPattern);

    } else if (type->isTemplateArgument()) {

    } else if (type->isFlags()
               && meta_type->indirections() == 0
               && (meta_type->isConstant() == meta_type->isReference())) {
        meta_type->setTypeUsagePattern(AbstractMetaType::FlagsPattern);

    } else if (type->isArray()) {
        meta_type->setTypeUsagePattern(AbstractMetaType::ArrayPattern);

    } else if (type->isThread()) {
        Q_ASSERT(meta_type->indirections() == 1);
        meta_type->setTypeUsagePattern(AbstractMetaType::ThreadPattern);

    } else if (type->isValue()
               && meta_type->indirections() == 0
               && (meta_type->isConstant() == meta_type->isReference()
                   || !meta_type->isReference())) {
        meta_type->setTypeUsagePattern(AbstractMetaType::ValuePattern);

    } else {
        meta_type->setTypeUsagePattern(AbstractMetaType::NativePointerPattern);
        ReportHandler::debugFull(QString("native pointer pattern for '%1'")
                                 .arg(meta_type->cppSignature()));
    }
}

QString AbstractMetaBuilder::translateDefaultValue(ArgumentModelItem item, AbstractMetaType *type,
                                               AbstractMetaFunction *fnc, AbstractMetaClass *implementing_class,
                                               int argument_index)
{
    QString function_name = fnc->name();
    QString class_name = implementing_class->name();

    QString replaced_expression = fnc->replacedDefaultExpression(implementing_class, argument_index + 1);
    if (fnc->removedDefaultExpression(implementing_class, argument_index +1))
        return "";
    if (!replaced_expression.isEmpty())
        return replaced_expression;

    QString expr = item->defaultValueExpression();
    // Qt 6: braced defaults (= {} / = {...}) recorded by the parser as a
    // marker spanning the braces. An empty brace pair value-constructs the
    // parameter type (e.g. QList<T>()); anything else keeps the source
    // spelling for the shell, which accepts braced construction.
    if (expr == "{}") {
        if (type != 0 && type->isContainer()
            && type->instantiations().size() == 1
            && type->instantiations().at(0)->typeEntry() != 0) {
            QString inner = type->instantiations().at(0)->typeEntry()->qualifiedCppName();
            if (!inner.isEmpty())
                return QString("%1<%2>()").arg(type->typeEntry()->name(), inner);
        }
        if (type != 0 && type->typeEntry() != 0
            && !type->typeEntry()->qualifiedCppName().isEmpty()
            && type->indirections() == 0 && !type->isReference()
            && (type->typeEntry()->isValue() || type->typeEntry()->isContainer())) {
            return QString("%1()").arg(type->typeEntry()->qualifiedCppName());
        }
        return expr;
    }
    if (type != 0 && type->isPrimitive()) {
        if (type->name() == "boolean") {
            if (expr == "false" || expr=="true") {
                return expr;
            } else {
                bool ok = false;
                int number = expr.toInt(&ok);
                if (ok && number)
                    return "true";
                else
                    return "false";
            }
        } else if (expr == "ULONG_MAX") {
            return "Long.MAX_VALUE";
        } else if (expr == "QVariant::Invalid") {
            return QString::number(QVariant::Invalid);
        } else {
            // This can be an enum or flag so I need to delay the
            // translation untill all namespaces are completly
            // processed. This is done in figureOutEnumValues()
            return expr;
        }
    } else if (type != 0 && (type->isFlags() || type->isEnum())) {
        // Same as with enum explanation above...
        return expr;

    } else {

        // constructor or functioncall can be a bit tricky...
        if (expr == "QVariant()" || expr == "QModelIndex()") {
            return "null";
        } else if (expr == "QString()") {
            return "null";
        } else if (expr.endsWith(")") && expr.contains("::")) {
            TypeEntry *typeEntry = TypeDatabase::instance()->findType(expr.left(expr.indexOf("::")));
            if (typeEntry)
                return typeEntry->qualifiedTargetLangName() + "." + expr.right(expr.length() - expr.indexOf("::") - 2);
        } else if (expr.endsWith(")") && type != 0 && type->isValue()) {
            int pos = expr.indexOf("(");

            TypeEntry *typeEntry = TypeDatabase::instance()->findType(expr.left(pos));
            if (typeEntry)
                return "new " + typeEntry->qualifiedTargetLangName() + expr.right(expr.length() - pos);
            else
                return expr;
        } else if (expr == "0" || expr == "nullptr") {
            // "nullptr" is the Qt 5 spelling of a null default argument
            return "null";
        } else if (type != 0 && (type->isObject() || type->isValue() || expr.contains("::"))) { // like Qt::black passed to a QColor
            TypeEntry *typeEntry = TypeDatabase::instance()->findType(expr.left(expr.indexOf("::")));

            expr = expr.right(expr.length() - expr.indexOf("::") - 2);
            if (typeEntry) {
                return "new " + type->typeEntry()->qualifiedTargetLangName() +
                       "(" + typeEntry->qualifiedTargetLangName() + "." + expr + ")";
            }
        }
    }

    QString warn = QString("unsupported default value '%3' of argument in function '%1', class '%2'")
        .arg(function_name).arg(class_name).arg(item->defaultValueExpression());
    ReportHandler::warning(warn);

    return QString();
}


bool AbstractMetaBuilder::isQObject(const QString &qualified_name)
{
    if (qualified_name == "QObject")
        return true;

    ClassModelItem class_item = m_dom->findClass(qualified_name);

    if (!class_item) {
      QStringList names = qualified_name.split(QLatin1String("::"));
      NamespaceModelItem ns = model_dynamic_cast<NamespaceModelItem>(m_dom);
      for (int i=0; i<names.size() - 1 && ns; ++i)
          ns = ns->namespaceMap().value(names.at(i));
      if (ns && names.size() >= 2)
          class_item = ns->findClass(names.at(names.size() - 1));
    }

    bool isqobject = class_item && class_item->extendsClass("QObject");

    if (class_item && !isqobject) {
        QStringList baseClasses = class_item->baseClasses();
        for (int i=0; i<baseClasses.count(); ++i) {

            isqobject = isQObject(baseClasses.at(i));
            if (isqobject)
                break;
        }
    }

    return isqobject;
}


bool AbstractMetaBuilder::isEnum(const QStringList &qualified_name)
{
    CodeModelItem item = m_dom->model()->findItem(qualified_name, m_dom->toItem());
    return item && item->kind() == _EnumModelItem::__node_kind;
}

AbstractMetaType *AbstractMetaBuilder::inheritTemplateType(const QList<AbstractMetaType *> &template_types,
                                                   AbstractMetaType *meta_type, bool *ok)
{
    if (ok != 0)
        *ok = true;
    if (!meta_type || (!meta_type->typeEntry()->isTemplateArgument() && !meta_type->hasInstantiations()))
        return meta_type ? meta_type->copy() : 0;

    AbstractMetaType *returned = meta_type->copy();
    returned->setOriginalTemplateType(meta_type->copy());

    if (returned->typeEntry()->isTemplateArgument()) {
        const TemplateArgumentEntry *tae = static_cast<const TemplateArgumentEntry *>(returned->typeEntry());

        // If the template is intantiated with void we special case this as rejecting the functions that use this
        // parameter from the instantiation.
        if (template_types.size() <= tae->ordinal() || template_types.at(tae->ordinal())->typeEntry()->name() == "void") {
            if (ok != 0)
                *ok = false;
            return 0;
        }

        AbstractMetaType *t = returned->copy();
        t->setTypeEntry(template_types.at(tae->ordinal())->typeEntry());
        t->setIndirections(template_types.at(tae->ordinal())->indirections() + t->indirections()
                           ? 1
                           : 0);
        decideUsagePattern(t);

        delete returned;
        returned = inheritTemplateType(template_types, t, ok);
        if (ok != 0 && !(*ok))
            return 0;
    }

    if (returned->hasInstantiations()) {
        QList<AbstractMetaType *> instantiations = returned->instantiations();
        for (int i=0; i<instantiations.count(); ++i) {
            instantiations[i] = inheritTemplateType(template_types, instantiations.at(i), ok);
            if (ok != 0 && !(*ok))
                return 0;
        }
        returned->setInstantiations(instantiations);
    }

    return returned;
}

bool AbstractMetaBuilder::inheritTemplate(AbstractMetaClass *subclass,
                                      const AbstractMetaClass *template_class,
                                      const TypeParser::Info &info)
{
    QList<TypeParser::Info> targs = info.template_instantiations;

    QList<AbstractMetaType *> template_types;
    foreach (const TypeParser::Info &i, targs) {
        TypeEntry *t = TypeDatabase::instance()->findType(i.qualified_name.join("::"));

        if (t != 0) {
            AbstractMetaType *temporary_type = createMetaType();
            temporary_type->setTypeEntry(t);
            temporary_type->setConstant(i.is_constant);
            temporary_type->setReference(i.is_reference);
            temporary_type->setIndirections(i.indirections);
            template_types << temporary_type;
        }
    }

    AbstractMetaFunctionList funcs = subclass->functions();
    foreach (const AbstractMetaFunction *function, template_class->functions()) {

        if (function->isModifiedRemoved(TypeSystem::All))
            continue;

        AbstractMetaFunction *f = function->copy();
        f->setArguments(AbstractMetaArgumentList());

        bool ok = true;
        AbstractMetaType *ftype = function->type();
        f->setType(inheritTemplateType(template_types, ftype, &ok));
        if (!ok) {
            delete f;
            continue;
        }

        foreach (AbstractMetaArgument *argument, function->arguments()) {
            AbstractMetaType *atype = argument->type();

            AbstractMetaArgument *arg = argument->copy();
            AbstractMetaType *inherited = inheritTemplateType(template_types, atype, &ok);
            if (!ok)
                break;
            // Qt 6: a container argument written against the class's own
            // template parameter (QList<T> & in QList::swap) can come through
            // without its instantiation. That leaves the generated argument
            // conversion uninstantiated and uncompilable, so fill it in from
            // the single instantiation of the template.
            if (inherited && inherited->typeEntry()->isContainer()
                && inherited->instantiations().isEmpty()
                && template_types.size() == 1) {
                QList<AbstractMetaType *> instantiations;
                instantiations << template_types.at(0)->copy();
                inherited->setInstantiations(instantiations);
            }
            arg->setType(inherited);
            f->addArgument(arg);
        }

        if (!ok) {
            delete f;
            continue ;
        }

        // There is no base class in java to inherit from here, so the
        // template instantiation is the class that implements the function..
        f->setImplementingClass(subclass);

        // We also set it as the declaring class, since the superclass is
        // supposed to disappear. This allows us to make certain function modifications
        // on the inherited functions.
        f->setDeclaringClass(subclass);


        if (f->isConstructor() && subclass->isTypeAlias()) {
            f->setName(subclass->name());
        } else if (f->isConstructor()) {
            delete f;
            continue;
        }

        // if the instantiation has a function named the same as an existing
        // function we have shadowing so we need to skip it.
        bool found = false;
        for (int i=0; i<funcs.size(); ++i) {
            if (funcs.at(i)->name() == f->name()) {
                found = true;
                continue;
            }
        }
        if (found) {
            delete f;
            continue;
        }

        ComplexTypeEntry *te = subclass->typeEntry();
        FunctionModificationList mods = function->modifications(template_class);
        for (int i=0; i<mods.size(); ++i) {
            FunctionModification mod = mods.at(i);
            mod.signature = f->minimalSignature();

            // If we ever need it... Below is the code to do
            // substitution of the template instantation type inside
            // injected code..
#if 0
            if (mod.modifiers & Modification::CodeInjection) {
                for (int j=0; j<template_types.size(); ++j) {
                    CodeSnip &snip = mod.snips.last();
                    QString code = snip.code();
                    code.replace(QString::fromLatin1("$$QT_TEMPLATE_%1$$").arg(j),
                                 template_types.at(j)->typeEntry()->qualifiedCppName());
                    snip.codeList.clear();
                    snip.addCode(code);
                }
            }
#endif
            te->addFunctionModification(mod);
        }

        subclass->addFunction(f);
    }

    // Clean up
    foreach (AbstractMetaType *type, template_types) {
        delete type;
    }


    {
        subclass->setTemplateBaseClass(template_class);

        subclass->setInterfaces(template_class->interfaces());
        subclass->setBaseClass(template_class->baseClass());
    }

    return true;
}

void AbstractMetaBuilder::parseQ_Property(AbstractMetaClass *meta_class, const QStringList &declarations)
{
    for (int i=0; i<declarations.size(); ++i) {
        QString p = declarations.at(i);

        QStringList l = p.split(QLatin1String(" "));


        QStringList qualifiedScopeName = currentScope()->qualifiedName();
        bool ok = false;
        AbstractMetaType *type = 0;
        QString scope;
        for (int j=qualifiedScopeName.size(); j>=0; --j) {
            scope = j > 0 ? QStringList(qualifiedScopeName.mid(0, j)).join("::") + "::" : QString();
            TypeInfo info;
            info.setQualifiedName((scope + l.at(0)).split("::"));

            type = translateType(info, &ok);
            if (type != 0 && ok) {
                break;
            }
        }

        if (type == 0 || !ok) {
            ReportHandler::warning(QString("Unable to decide type of property: '%1' in class '%2'")
                                   .arg(l.at(0)).arg(meta_class->name()));
            continue;
        }

        QString typeName = scope + l.at(0);

        QPropertySpec *spec = new QPropertySpec(type->typeEntry());
        spec->setName(l.at(1));
        spec->setIndex(i);

        for (int pos=2; pos+1<l.size(); pos+=2) {
            if (l.at(pos) == QLatin1String("READ"))
                spec->setRead(l.at(pos+1));
            else if (l.at(pos) == QLatin1String("WRITE"))
                spec->setWrite(l.at(pos+1));
            else if (l.at(pos) == QLatin1String("DESIGNABLE"))
                spec->setDesignable(l.at(pos+1));
            else if (l.at(pos) == QLatin1String("RESET"))
                spec->setReset(l.at(pos+1));
        }

        meta_class->addPropertySpec(spec);
        delete type;
    }
}

static void hide_functions(const AbstractMetaFunctionList &l) {
    foreach (AbstractMetaFunction *f, l) {
        FunctionModification mod;
        mod.signature = f->minimalSignature();
        mod.modifiers = FunctionModification::Private;
        ((ComplexTypeEntry *) f->implementingClass()->typeEntry())->addFunctionModification(mod);
    }
}

static void remove_function(AbstractMetaFunction *f) {
    FunctionModification mod;
    mod.removal = TypeSystem::All;
    mod.signature = f->minimalSignature();
    ((ComplexTypeEntry *) f->implementingClass()->typeEntry())->addFunctionModification(mod);
}

static AbstractMetaFunctionList filter_functions(const AbstractMetaFunctionList &lst, QSet<QString> *signatures)
{
    AbstractMetaFunctionList functions;
    foreach (AbstractMetaFunction *f, lst) {
        QString signature = f->minimalSignature();
        int start = signature.indexOf(QLatin1Char('(')) + 1;
        int end = signature.lastIndexOf(QLatin1Char(')'));
        signature = signature.mid(start, end - start);
        if (signatures->contains(signature)) {
            remove_function(f);
            continue;
        }
        (*signatures) << signature;
        functions << f;
    }
    return functions;
}

void AbstractMetaBuilder::setupEquals(AbstractMetaClass *cls)
{
    AbstractMetaFunctionList equals;
    AbstractMetaFunctionList nequals;

    QString op_equals = QLatin1String("operator_equal");
    QString op_nequals = QLatin1String("operator_not_equal");

    AbstractMetaFunctionList functions = cls->queryFunctions(AbstractMetaClass::ClassImplements
                                                         | AbstractMetaClass::NotRemovedFromTargetLang);
    foreach (AbstractMetaFunction *f, functions) {
        if (f->name() == op_equals)
            equals << f;
        else if (f->name() == op_nequals)
            nequals << f;
    }

    if (equals.size() || nequals.size()) {
        if (!cls->hasHashFunction()) {
            ReportHandler::warning(QString::fromLatin1("Class '%1' has equals operators but no qHash() function")
                                   .arg(cls->name()));
        }

        hide_functions(equals);
        hide_functions(nequals);

        // We only need == if we have both == and !=, and one == for
        // each signature type, like QDateTime::==(QDate) and (QTime)
        // if such a thing exists...
        QSet<QString> func_signatures;
        cls->setEqualsFunctions(filter_functions(equals, &func_signatures));
        cls->setNotEqualsFunctions(filter_functions(nequals, &func_signatures));
    }
}

void AbstractMetaBuilder::setupComparable(AbstractMetaClass *cls)
{
    AbstractMetaFunctionList greater;
    AbstractMetaFunctionList greaterEquals;
    AbstractMetaFunctionList less;
    AbstractMetaFunctionList lessEquals;

    QString op_greater = QLatin1String("operator_greater");
    QString op_greater_eq = QLatin1String("operator_greater_or_equal");
    QString op_less = QLatin1String("operator_less");
    QString op_less_eq = QLatin1String("operator_less_or_equal");

    AbstractMetaFunctionList functions = cls->queryFunctions(AbstractMetaClass::ClassImplements
                                                         | AbstractMetaClass::NotRemovedFromTargetLang);
    foreach (AbstractMetaFunction *f, functions) {
        if (f->name() == op_greater)
            greater << f;
        else if (f->name() == op_greater_eq)
            greaterEquals << f;
        else if (f->name() == op_less)
            less << f;
        else if (f->name() == op_less_eq)
            lessEquals << f;
    }

    bool hasEquals = cls->equalsFunctions().size() || cls->notEqualsFunctions().size();

    // Conditions for comparable is:
    //     >, ==, <             - The basic case
    //     >, ==                - Less than becomes else case
    //     <, ==                - Greater than becomes else case
    //     >=, <=               - if (<= && >=) -> equal
    bool mightBeComparable = greater.size() || greaterEquals.size() || less.size() || lessEquals.size()
                        || greaterEquals.size() == 1 || lessEquals.size() == 1;

    if (mightBeComparable) {
        QSet<QString> signatures;

        // We only hide the original functions if we are able to make a compareTo() method
        bool wasComparable = false;

        // The three upper cases, prefer the <, == approach
        if (hasEquals && (greater.size() || less.size())) {
            cls->setLessThanFunctions(filter_functions(less, &signatures));
            cls->setGreaterThanFunctions(filter_functions(greater, &signatures));
            filter_functions(greaterEquals, &signatures);
            filter_functions(lessEquals, &signatures);
            wasComparable = true;
        } else if (hasEquals && (greaterEquals.size() || lessEquals.size())) {
            cls->setLessThanEqFunctions(filter_functions(lessEquals, &signatures));
            cls->setGreaterThanEqFunctions(filter_functions(greaterEquals, &signatures));
            wasComparable = true;
        } else if (greaterEquals.size() == 1 || lessEquals.size() == 1) {
            cls->setGreaterThanEqFunctions(greaterEquals);
            cls->setLessThanEqFunctions(lessEquals);
            filter_functions(less, &signatures);
            filter_functions(greater, &signatures);
            wasComparable = true;
        }

        if (wasComparable) {
            hide_functions(greater);
            hide_functions(greaterEquals);
            hide_functions(less);
            hide_functions(lessEquals);
        }
    }

}

void AbstractMetaBuilder::setupClonable(AbstractMetaClass *cls)
{
    QString op_assign = QLatin1String("operator_assign");

    AbstractMetaFunctionList functions = cls->queryFunctions(AbstractMetaClass::ClassImplements);
    foreach (AbstractMetaFunction *f, functions) {
        if ((f->name() == op_assign || f->isConstructor()) && f->isPublic()) {
            AbstractMetaArgumentList arguments = f->arguments();
            if (arguments.size() == 1) {
                if (cls->typeEntry()->qualifiedCppName() == arguments.at(0)->type()->typeEntry()->qualifiedCppName()) {
                    if (cls->typeEntry()->isValue()) {
                        cls->setHasCloneOperator(true);
                        return;
                    }
                }
            }
        }
    }
}

static void write_reject_log_file(const QString &name,
                                  const QMap<QString, AbstractMetaBuilder::RejectReason> &rejects)
{
    QFile f(name);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ReportHandler::warning(QString("failed to write log file: '%1'")
                               .arg(f.fileName()));
        return;
    }

    QTextStream s(&f);


    for (int reason=0; reason<AbstractMetaBuilder::NoReason; ++reason) {
        s << QString(72, '*') << endl;
        switch (reason) {
        case AbstractMetaBuilder::NotInTypeSystem:
            s << "Not in type system";
            break;
        case AbstractMetaBuilder::GenerationDisabled:
            s << "Generation disabled by type system";
            break;
        case AbstractMetaBuilder::RedefinedToNotClass:
            s << "Type redefined to not be a class";
            break;

        case AbstractMetaBuilder::UnmatchedReturnType:
            s << "Unmatched return type";
            break;

        case AbstractMetaBuilder::UnmatchedArgumentType:
            s << "Unmatched argument type";
            break;

        default:
            s << "unknown reason";
            break;
        }

        s << endl;

        for (QMap<QString, AbstractMetaBuilder::RejectReason>::const_iterator it = rejects.constBegin();
             it != rejects.constEnd(); ++it) {
            if (it.value() != reason)
                continue;
            s << " - " << it.key() << endl;
        }

        s << QString(72, '*') << endl << endl;
    }

}


void AbstractMetaBuilder::dumpLog()
{
    write_reject_log_file("mjb_rejected_classes.log", m_rejected_classes);
    write_reject_log_file("mjb_rejected_enums.log", m_rejected_enums);
    write_reject_log_file("mjb_rejected_functions.log", m_rejected_functions);
    write_reject_log_file("mjb_rejected_fields.log", m_rejected_fields);
}

// Deterministic class ordering. The result of this function decides the order
// in which the generated files are written and the order in which the classes
// are registered by the module initialiser. It used to be "sorted" with
// std::sort on AbstractMetaClass*, which compares the pointers themselves - i.e.
// the process address layout, which changes from one run to the next. Sort by
// name so a regen is reproducible.
static bool metaClassLess(const AbstractMetaClass *a, const AbstractMetaClass *b)
{
    return a->name() < b->name();
}

AbstractMetaClassList AbstractMetaBuilder::classesTopologicalSorted() const
{
    AbstractMetaClassList res;

    AbstractMetaClassList classes = m_meta_classes;
    std::sort(classes.begin(), classes.end(), metaClassLess);

    QSet<AbstractMetaClass*> noDependency;
    QHash<AbstractMetaClass*, QSet<AbstractMetaClass* >* > hash;
    foreach (AbstractMetaClass *cls, classes) {
        QSet<AbstractMetaClass* > *depends = new QSet<AbstractMetaClass* >();

        if (cls->baseClass())
            depends->insert(cls->baseClass());

        foreach (AbstractMetaClass *interface, cls->interfaces()) {
            AbstractMetaClass *impl = interface->primaryInterfaceImplementor();
            if (impl == cls)
                continue;
            // An interface without an implementor (or one that could not be
            // resolved) used to be stored as a null pointer. It can never match
            // an emitted class, so that dependency stayed unsatisfied forever
            // and the class was dropped as if its graph were cyclic.
            if (!impl)
                continue;
            depends->insert(impl);
        }

        if (depends->empty()) {
            noDependency.insert(cls);
        } else {
            hash.insert(cls, depends);
        }
    }

    // Emit one dependency wave at a time: the whole wave is appended before its
    // dependents are released, so a class is always generated after the classes
    // it derives from.
    while (!noDependency.isEmpty()) {
        AbstractMetaClassList wave;
        foreach (AbstractMetaClass *cls, noDependency)
            wave.append(cls);
        std::sort(wave.begin(), wave.end(), metaClassLess);
        noDependency.clear();

        foreach (AbstractMetaClass *cls, wave) {
            if (!cls->isInterface())
                res.append(cls);
        }

        // Collect the entries whose dependencies are now all emitted, and only
        // then touch 'hash'. A QHashIterator is invalidated the moment its hash
        // is modified: the previous code called hash.remove() and deleted the
        // dependency set from inside the iteration, which is undefined
        // behaviour. On some runs the scan stopped early, leaving classes in
        // 'hash' that were then never appended to 'res' - they vanished from the
        // generated bindings, and which ones vanished depended on the QHash
        // iteration order (i.e. on the address layout).
        AbstractMetaClassList released;
        QHashIterator<AbstractMetaClass *, QSet<AbstractMetaClass *> *> i(hash);
        while (i.hasNext()) {
            i.next();
            foreach (AbstractMetaClass *cls, wave)
                i.value()->remove(cls);
            if (i.value()->isEmpty())
                released.append(i.key());
        }

        foreach (AbstractMetaClass *cls, released) {
            delete hash.take(cls);
            noDependency.insert(cls);
        }
    }

    // A class still sitting in 'hash' takes part in a dependency cycle: a base
    // class whose own base is the derived class, or a base class that could not
    // be resolved. Dropping it silently is what made a whole class - with every
    // function, enum and field it owns - disappear from the bindings, while the
    // build stayed green because generated_cpp still held the previous file.
    // Keep the class, append it in a deterministic order, and say so on stderr:
    // this build does not print qWarning output, so a warning alone is invisible
    // in the regen log.
    if (!hash.isEmpty()) {
        AbstractMetaClassList cyclic;
        foreach (AbstractMetaClass *cls, hash.keys())
            cyclic.append(cls);
        std::sort(cyclic.begin(), cyclic.end(), metaClassLess);
        fprintf(stderr, "*** dependency graph is cyclic: %d class(es) kept and "
                        "appended in name order\n", int(cyclic.size()));
        foreach (AbstractMetaClass *cls, cyclic) {
            fprintf(stderr, "***   cyclic: %s\n", qPrintable(cls->name()));
            if (!cls->isInterface())
                res.append(cls);
            delete hash.take(cls);
        }
    }

    return res;
}
