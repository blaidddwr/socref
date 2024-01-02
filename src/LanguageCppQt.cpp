#include "LanguageCppQt.h"
#include "BlockCppQt.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppExceptionItem.h"
#include "BlockCppNamespace.h"
#include "BlockCppQtFunction.h"
#include "BlockCppQtProperty.h"
#include "BlockCppVariable.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Language {
using namespace Block::CppQt;
using Class = Block::Cpp::Class;
using Enumeration = Block::Cpp::Enumeration;
using ExceptionItem = Block::Cpp::ExceptionItem;
using Namespace = Block::Cpp::Namespace;
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
            ,{FunctionIndex,PropertyIndex,VariableIndex}
        )
    );
    appendBlock(new Model::Meta::Block(meta,EnumerationIndex,"enumeration","Enumeration",{}));
    appendBlock(new Model::Meta::Block(meta,ExceptionIndex,"exception","Exception",{}));
    appendBlock(
        new Model::Meta::Block(
            meta
            ,FunctionIndex
            ,"function"
            ,"Function"
            ,{ExceptionIndex,VariableIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,NamespaceIndex
            ,"namespace"
            ,"Namespace"
            ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex}
        )
    );
    appendBlock(
        new Model::Meta::Block(
            meta
            ,PropertyIndex
            ,"property"
            ,"Property"
            ,{FunctionIndex,VariableIndex}
        )
    );
    appendBlock(new Model::Meta::Block(meta,VariableIndex,"variable","Variable",{}));
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
