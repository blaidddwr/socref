#include "LanguageCppQt.h"
#include <QtGui>
#include "BlockCppQt.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppExceptionItem.h"
#include "BlockCppNamespace.h"
#include "BlockCppQtFunction.h"
#include "BlockCppQtProperty.h"
#include "BlockCppUnion.h"
#include "BlockCppVariable.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Language {
using namespace Block::CppQt;
using Class = Block::Cpp::Class;
using Enumeration = Block::Cpp::Enumeration;
using ExceptionItem = Block::Cpp::ExceptionItem;
using Namespace = Block::Cpp::Namespace;
using Union = Block::Cpp::Union;
using Variable = Block::Cpp::Variable;


CppQt::CppQt(
    Model::Meta::Language* meta
    ,QObject* parent
):
    Abstract(meta,parent)
{
    appendBlock(
        new Model::Meta::Block(
            meta
            ,ClassIndex
            ,"class"
            ,"Class"
            ,new QIcon(":/cpp/class.svg")
            ,{FunctionIndex,PropertyIndex,VariableIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,EnumerationIndex
            ,"enumeration"
            ,"Enumeration"
            ,new QIcon(":/cpp/enumeration.svg")
            ,{}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,ExceptionIndex
            ,"exception"
            ,"Exception"
            ,new QIcon(":/cpp/exception.svg")
            ,{}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,FunctionIndex
            ,"function"
            ,"Function"
            ,new QIcon(":/cpp/public_function.svg")
            ,{ExceptionIndex,VariableIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,NamespaceIndex
            ,"namespace"
            ,"Namespace"
            ,new QIcon(":/cpp/namespace.svg")
            ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex,UnionIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,PropertyIndex
            ,"property"
            ,"Property"
            ,new QIcon(":/cpp/property.svg")
            ,{FunctionIndex,VariableIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(meta,UnionIndex,"union","Union",new QIcon(":/cpp/union.svg"),{})
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,VariableIndex
            ,"variable"
            ,"Variable"
            ,new QIcon(":/cpp/variable.svg")
            ,{}
        )
    );
}


Block::Abstract* CppQt::create(
    int index
    ,QObject* parent
) const
{
    switch (index)
    {
    case ClassIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == ClassIndex);
        return new Class(meta,parent);
    }
    case EnumerationIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == EnumerationIndex);
        return new Enumeration(meta,parent);
    }
    case ExceptionIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == ExceptionIndex);
        return new ExceptionItem(meta,parent);
    }
    case FunctionIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == FunctionIndex);
        return new Function(meta,parent);
    }
    case NamespaceIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == NamespaceIndex);
        return new Namespace(meta,parent);
    }
    case PropertyIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == PropertyIndex);
        return new Property(meta,parent);
    }
    case UnionIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == UnionIndex);
        return new Union(meta,parent);
    }
    case VariableIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == VariableIndex);
        return new Variable(meta,parent);
    }
    default:
        G_ASSERT(false);
        return nullptr;
    }
}


int CppQt::rootIndex(
) const
{
    return NamespaceIndex;
}
}
