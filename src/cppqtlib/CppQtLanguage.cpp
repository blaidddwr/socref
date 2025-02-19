#include "CppQtLanguage.h"
#include <QtGui>
#include "CppBlockClass.h"
#include "CppBlockEnumeration.h"
#include "CppBlockEnumerationValue.h"
#include "CppBlockException.h"
#include "CppBlockNamespace.h"
#include "CppBlockProperty.h"
#include "CppBlockUnion.h"
#include "CppBlockVariable.h"
#include "CppParse.h"
#include "CppParseHeadParser.h"
#include "CppQtBlock.h"
#include "CppQtBlockFunction.h"
#include "ModelMetaBlock.h"
namespace CppQt {
using namespace CppQt::Block;
using namespace Cpp::Parse;
using Class = Cpp::Block::Class;
using Enumeration = Cpp::Block::Enumeration;
using EnumerationValue = Cpp::Block::EnumerationValue;
using Exception = Cpp::Block::Exception;
using Namespace = Cpp::Block::Namespace;
using Property = Cpp::Block::Property;
using Union = Cpp::Block::Union;
using Variable = Cpp::Block::Variable;


Language::Language(
):
    AbstractLanguage(new Model::Meta::Language("cppqt","C++/Qt",QIcon(":/cppqt.svg")))
{
    Class::initializeIcons();
    Function::initializeIcons();
    Property::initializeIcons();
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
        throw std::logic_error("unknown block index");
    }
}


AbstractParser* Language::createParser(
    int index
) const
{
    switch (index)
    {
    case HeadParserIndex:
        return new HeadParser;
    default:
        throw std::logic_error("unknown parser index.");
    }
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
