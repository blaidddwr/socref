#include "LanguageCppQt.h"
#include <QtGui>
#include "BlockCppQt.h"
#include "BlockCppClass.h"
#include "BlockCppEnumeration.h"
#include "BlockCppEnumerationValue.h"
#include "BlockCppException.h"
#include "BlockCppNamespace.h"
#include "BlockCppProperty.h"
#include "BlockCppQtFunction.h"
#include "BlockCppUnion.h"
#include "BlockCppVariable.h"
#include "ModelMetaBlock.h"
#include "gassert.h"
namespace Language {
using namespace Block::CppQt;
using Class = Block::Cpp::Class;
using Enumeration = Block::Cpp::Enumeration;
using EnumerationValue = Block::Cpp::EnumerationValue;
using Exception = Block::Cpp::Exception;
using Namespace = Block::Cpp::Namespace;
using Property = Block::Cpp::Property;
using Union = Block::Cpp::Union;
using Variable = Block::Cpp::Variable;


CppQt::CppQt(
    Model::Meta::Language* meta
    ,QObject* parent
):
    Abstract(meta,parent)
{
    appendBlocks(
        {
            new Model::Meta::Block(
                meta
                ,ClassIndex
                ,"class"
                ,"Class"
                ,QIcon(":/cpp/class.svg")
                ,{EnumerationIndex,FunctionIndex,PropertyIndex,VariableIndex}
            )
            ,new Model::Meta::Block(
                meta
                ,EnumerationIndex
                ,"enumeration"
                ,"Enumeration"
                ,QIcon(":/cpp/enumeration.svg")
                ,{EnumerationValueIndex}
            )
            ,new Model::Meta::Block(
                 meta
                 ,EnumerationValueIndex
                 ,"enumerationvalue"
                 ,"Enumeration Value"
                 ,QIcon(":/cpp/enumeration_value.svg")
                 ,{}
             )
            ,new Model::Meta::Block(
                meta
                ,ExceptionIndex
                ,"exception"
                ,"Exception"
                ,QIcon(":/cpp/exception.svg")
                ,{}
            )
            ,new Model::Meta::Block(
                meta
                ,FunctionIndex
                ,"function"
                ,"Function"
                ,QIcon(":/cpp/public_function.svg")
                ,{ExceptionIndex,VariableIndex}
            )
            ,new Model::Meta::Block(
                meta
                ,NamespaceIndex
                ,"namespace"
                ,"Namespace"
                ,QIcon(":/cpp/namespace.svg")
                ,{ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex,UnionIndex}
            )
            ,new Model::Meta::Block(
                meta
                ,PropertyIndex
                ,"property"
                ,"Property"
                ,QIcon(":/cpp/property.svg")
                ,{FunctionIndex,VariableIndex}
            )
            ,new Model::Meta::Block(
                meta
                ,UnionIndex
                ,"union"
                ,"Union"
                ,QIcon(":/cpp/union.svg")
                ,{}
            )
            ,new Model::Meta::Block(
                meta
                ,VariableIndex
                ,"variable"
                ,"Variable"
                ,QIcon(":/cpp/variable.svg")
                ,{}
            )
        }
    );
}


Block::Abstract* CppQt::createBlock(
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
    case EnumerationValueIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == EnumerationValueIndex);
        return new EnumerationValue(meta,parent);
    }
    case ExceptionIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == ExceptionIndex);
        return new Exception(meta,parent);
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


Controller::Parse::Abstract* CppQt::createParser(
    int index
    ,QObject* parent
) const
{
    Q_UNUSED(index);
    Q_UNUSED(parent);
    return nullptr;
}


Block::Abstract* CppQt::createRootBlock(
    QObject* parent
) const
{
    auto ret = new Namespace(blockMeta(NamespaceIndex),parent);
    ret->setName(QString());
    ret->setDescription(QString());
    return ret;
}


int CppQt::rootIndex(
) const
{
    return NamespaceIndex;
}


Controller::Route::Abstract* CppQt::router(
) const
{
    return nullptr;
}
}
