#include <QtTest>
#include "BlockAbstract.h"
#include "BlockCpp.h"
#include "LanguageCpp.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
#include "../utility.h"
using namespace Block::Cpp;

class TestCppLanguage: public QObject
{
    Q_OBJECT
    Model::Meta::Language* _meta;
    Language::Cpp* _language;
private slots:
    void initTestCase();
    void createClass();
    void createEnumeration();
    void createException();
    void createFunction();
    void createNamespace();
    void createProperty();
    void createUnion();
    void createVariable();
    void rootIndex();
    void cleanupTestCase();
};

void TestCppLanguage::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    _meta = new Model::Meta::Language("cpp","C++",QIcon(),this);
    _language = new ::Language::Cpp(_meta,this);
    QCOMPARE(_language->meta(),_meta);
}

void TestCppLanguage::createClass()
{
    static const QSet<int> allowList {EnumerationIndex,FunctionIndex,PropertyIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/class.svg");
    auto block = _language->create(ClassIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ClassIndex);
    QCOMPARE(meta->name(),"class");
    QCOMPARE(meta->label(),"Class");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createEnumeration()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/enumeration.svg");
    auto block = _language->create(EnumerationIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationIndex);
    QCOMPARE(meta->name(),"enumeration");
    QCOMPARE(meta->label(),"Enumeration");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createException()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/exception.svg");
    auto block = _language->create(ExceptionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ExceptionIndex);
    QCOMPARE(meta->name(),"exception");
    QCOMPARE(meta->label(),"Exception");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createFunction()
{
    static const QSet<int> allowList {ExceptionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/public_function.svg");
    auto block = _language->create(FunctionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),FunctionIndex);
    QCOMPARE(meta->name(),"function");
    QCOMPARE(meta->label(),"Function");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createNamespace()
{
    static const QSet<int> allowList {
        ClassIndex
        ,EnumerationIndex
        ,FunctionIndex
        ,NamespaceIndex
        ,UnionIndex
    };
    static const QIcon testIcon(":/cpp/namespace.svg");
    auto block = _language->create(NamespaceIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),NamespaceIndex);
    QCOMPARE(meta->name(),"namespace");
    QCOMPARE(meta->label(),"Namespace");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createProperty()
{
    static const QSet<int> allowList {FunctionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/property.svg");
    auto block = _language->create(PropertyIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),PropertyIndex);
    QCOMPARE(meta->name(),"property");
    QCOMPARE(meta->label(),"Property");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createUnion()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/union.svg");
    auto block = _language->create(UnionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),UnionIndex);
    QCOMPARE(meta->name(),"union");
    QCOMPARE(meta->label(),"Union");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::createVariable()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/variable.svg");
    auto block = _language->create(VariableIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),VariableIndex);
    QCOMPARE(meta->name(),"variable");
    QCOMPARE(meta->label(),"Variable");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
    QCOMPARE(meta->language(),_meta);
}

void TestCppLanguage::rootIndex()
{
    QCOMPARE(_language->rootIndex(),Block::Cpp::NamespaceIndex);
}

void TestCppLanguage::cleanupTestCase()
{
    delete _language;
    delete _meta;
}

QTEST_MAIN(TestCppLanguage)
#include "test.moc"
