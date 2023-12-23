#include "TestLanguageCppQt.h"
#include <QtTest>
#include "BlockAbstract.h"
#include "BlockCppQt.h"
#include "LanguageCppQt.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
namespace Test {
namespace Language {
using namespace Block::CppQt;


void CppQt::initTestCase(
)
{
    _meta = new Model::Meta::Language("cppqt","C++/Qt",this);
    _language = new ::Language::CppQt(_meta,this);
    QCOMPARE(_language->meta(),_meta);
}


void CppQt::createClass(
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


void CppQt::createEnumeration(
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


void CppQt::createFunction(
)
{
    static const QSet<int> allowList {};
    auto block = _language->create(FunctionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),FunctionIndex);
    QCOMPARE(meta->name(),"function");
    QCOMPARE(meta->label(),"Function");
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}


void CppQt::createNamespace(
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


void CppQt::rootIndex(
)
{
    QCOMPARE(_language->rootIndex(),NamespaceIndex);
}


void CppQt::cleanupTestCase(
)
{
    delete _language;
    delete _meta;
}
}
}
