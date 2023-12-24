#include "TestLanguageCpp.h"
#include <QtTest>
#include "BlockAbstract.h"
#include "BlockCpp.h"
#include "LanguageCpp.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
namespace Test {
namespace Language {
using namespace Block::Cpp;


void Cpp::initTestCase(
)
{
    _meta = new Model::Meta::Language("cpp","C++",this);
    _language = new ::Language::Cpp(_meta,this);
    QCOMPARE(_language->meta(),_meta);
}


void Cpp::createClass(
)
{
    static const QSet<int> allowList {FunctionIndex};
    auto block = _language->create(ClassIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ClassIndex);
    QCOMPARE(meta->name(),"class");
    QCOMPARE(meta->label(),"Class");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::createEnumeration(
)
{
    static const QSet<int> allowList {};
    auto block = _language->create(EnumerationIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationIndex);
    QCOMPARE(meta->name(),"enumeration");
    QCOMPARE(meta->label(),"Enumeration");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::createException(
)
{
    static const QSet<int> allowList {};
    auto block = _language->create(ExceptionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ExceptionIndex);
    QCOMPARE(meta->name(),"exception");
    QCOMPARE(meta->label(),"Exception");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::createFunction(
)
{
    static const QSet<int> allowList {ExceptionIndex,VariableIndex};
    auto block = _language->create(FunctionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),FunctionIndex);
    QCOMPARE(meta->name(),"function");
    QCOMPARE(meta->label(),"Function");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::createNamespace(
)
{
    static const QSet<int> allowList {ClassIndex,EnumerationIndex,FunctionIndex,NamespaceIndex};
    auto block = _language->create(NamespaceIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),NamespaceIndex);
    QCOMPARE(meta->name(),"namespace");
    QCOMPARE(meta->label(),"Namespace");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::createVariable(
)
{
    static const QSet<int> allowList {};
    auto block = _language->create(VariableIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),VariableIndex);
    QCOMPARE(meta->name(),"variable");
    QCOMPARE(meta->label(),"Variable");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void Cpp::rootIndex(
)
{
    QCOMPARE(_language->rootIndex(),Block::Cpp::NamespaceIndex);
}


void Cpp::cleanupTestCase(
)
{
    delete _language;
    delete _meta;
}
}
}
