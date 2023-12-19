#include "LanguageCppQt.h"
#include "BlockCppQt.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppNamespace.h"
#include "BlockCppQtFunction.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Language {
using namespace Block::CppQt;
using Class = Block::Cpp::Class;
using Enumeration = Block::Cpp::Enumeration;
using Namespace = Block::Cpp::Namespace;


CppQt::CppQt(
    Model::Meta::Language* meta
    ,QObject* parent
):
    Abstract(meta,parent)
{
    appendBlock(new Model::Meta::Block(meta,ClassIndex,"class","Class",{FunctionIndex}));
    appendBlock(new Model::Meta::Block(meta,EnumerationIndex,"enumeration","Enumeration",{}));
    appendBlock(new Model::Meta::Block(meta,FunctionIndex,"function","Function",{}));
    appendBlock(
        new Model::Meta::Block(
            meta
            ,NamespaceIndex
            ,"namespace"
            ,"Namespace"
            ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex}
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
