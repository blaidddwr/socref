#include "LanguageCppQt.h"
#include "BlockCppQt.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppNamespace.h"
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
    appendBlock(new Model::Meta::Block(meta,ClassIndex,"class","Class",{}));
    appendBlock(new Model::Meta::Block(meta,EnumerationIndex,"enumeration","Enumeration",{}));
    appendBlock(
        new Model::Meta::Block(
            meta
            ,NamespaceIndex
            ,"namespace"
            ,"Namespace"
            ,{NamespaceIndex,ClassIndex}
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
    case NamespaceIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == NamespaceIndex);
        return new Namespace(meta,parent);
    }
    case ClassIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == ClassIndex);
        return new Class(meta,parent);
    }
    case EnumerationIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == EnumerationIndex);
        return new Enumeration(meta,parent);
    }
    default:
        Q_ASSERT(false);
        return nullptr;
    }
}


int CppQt::rootIndex(
) const
{
    return NamespaceIndex;
}
}
