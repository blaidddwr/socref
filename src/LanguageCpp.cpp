#include "LanguageCpp.h"
#include "BlockCpp.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppFunction.h"
#include "BlockCppNamespace.h"
#include "BlockCppVariable.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Language {
using namespace Block::Cpp;


Cpp::Cpp(
    Model::Meta::Language* meta
    ,QObject* parent
):
    Abstract(meta,parent)
{
    appendBlock(new Model::Meta::Block(meta,ClassIndex,"class","Class",{FunctionIndex}));
    appendBlock(new Model::Meta::Block(meta,EnumerationIndex,"enumeration","Enumeration",{}));
    appendBlock(new Model::Meta::Block(meta,FunctionIndex,"function","Function",{VariableIndex}));
    appendBlock(
        new Model::Meta::Block(
            meta
            ,NamespaceIndex
            ,"namespace"
            ,"Namespace"
            ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex}
        )
    );
    appendBlock(new Model::Meta::Block(meta,VariableIndex,"variable","Variable",{}));
}


Block::Abstract* Cpp::create(
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


int Cpp::rootIndex(
) const
{
    return NamespaceIndex;
}
}
