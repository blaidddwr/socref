#include "CppLanguage.h"
#include <QtGui>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockEnumeration.h"
#include "CppBlockEnumerationValue.h"
#include "CppBlockException.h"
#include "CppBlockFunction.h"
#include "CppBlockNamespace.h"
#include "CppBlockProperty.h"
#include "CppBlockUnion.h"
#include "CppBlockVariable.h"
#include "ModelMetaBlock.h"
namespace Cpp {
using namespace Block;


Language::Language(
):
    AbstractLanguage(new Model::Meta::Language("cpp","C++",QIcon(":/cpp.svg")))
{
    appendBlocks(
        {
            new Model::Meta::Block(
                meta()
                ,ClassIndex
                ,"class"
                ,"Class"
                ,QIcon(":/cpp/class.svg")
                ,{EnumerationIndex,FunctionIndex,PropertyIndex,VariableIndex}
                )
            ,new Model::Meta::Block(
                meta()
                ,EnumerationIndex
                ,"enumeration"
                ,"Enumeration"
                ,QIcon(":/cpp/enumeration.svg")
                ,{EnumerationValueIndex}
                )
            ,new Model::Meta::Block(
                meta()
                ,EnumerationValueIndex
                ,"enumerationvalue"
                ,"Enumeration Value"
                ,QIcon(":/cpp/enumeration_value.svg")
                ,{}
                )
            ,new Model::Meta::Block(
                meta()
                ,ExceptionIndex
                ,"exception"
                ,"Exception"
                ,QIcon(":/cpp/exception.svg")
                ,{}
                )
            ,new Model::Meta::Block(
                meta()
                ,FunctionIndex
                ,"function"
                ,"Function"
                ,QIcon(":/cpp/public_function.svg")
                ,{ExceptionIndex,VariableIndex}
                )
            ,new Model::Meta::Block(
                meta()
                ,NamespaceIndex
                ,"namespace"
                ,"Namespace"
                ,QIcon(":/cpp/namespace.svg")
                ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex,UnionIndex}
                )
            ,new Model::Meta::Block(
                meta()
                ,PropertyIndex
                ,"property"
                ,"Property"
                ,QIcon(":/cpp/property.svg")
                ,{FunctionIndex,VariableIndex}
                )
            ,new Model::Meta::Block(
                meta()
                ,UnionIndex
                ,"union"
                ,"Union"
                ,QIcon(":/cpp/union.svg")
                ,{}
                )
            ,new Model::Meta::Block(
                meta()
                ,VariableIndex
                ,"variable"
                ,"Variable"
                ,QIcon(":/cpp/variable.svg")
                ,{}
                )
        }
        );
}


AbstractBlock* Language::createBlock(
    int index
    ,QObject* parent
) const
{
    switch (index)
    {
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
    case EnumerationValueIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == EnumerationValueIndex);
        return new EnumerationValue(meta,parent);
    }
    case ExceptionIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == ExceptionIndex);
        return new Exception(meta,parent);
    }
    case FunctionIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == FunctionIndex);
        return new Function(meta,parent);
    }
    case NamespaceIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == NamespaceIndex);
        return new Namespace(meta,parent);
    }
    case PropertyIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == PropertyIndex);
        return new Property(meta,parent);
    }
    case UnionIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == UnionIndex);
        return new Union(meta,parent);
    }
    case VariableIndex:
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == VariableIndex);
        return new Variable(meta,parent);
    }
    default:
        Q_ASSERT(false);
        return nullptr;
    }
}


AbstractParser* Language::createParser(
    int index
    ,QObject* parent
) const
{
    Q_UNUSED(index);
    Q_UNUSED(parent);
    return nullptr;
}


AbstractBlock* Language::createRootBlock(
    QObject* parent
) const
{
    auto ret = new Namespace(blockMeta(NamespaceIndex),parent);
    ret->setName(QString());
    ret->setDescription(QString());
    return ret;
}


int Language::rootIndex(
) const
{
    return NamespaceIndex;
}


AbstractRouter* Language::router(
) const
{
    return nullptr;
}
}
