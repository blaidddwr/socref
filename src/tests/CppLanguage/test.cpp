#include <QtTest>
#include "AbstractBlock.h"
#include "CppBlock.h"
#include "CppLanguage.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;

class TestCppLanguage: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
private slots:
    void initTestCase();
    void createClass();
    void createEnumeration();
    void createEnumerationValue();
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
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    static const QIcon testIcon(":/cpp.svg");
    _language = new Cpp::Language;
    _language->setParent(this);
    auto meta = _language->meta();
    QCOMPARE(meta->name(),"cpp");
    QCOMPARE(meta->label(),"C++");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
}

void TestCppLanguage::createClass()
{
    static const QSet<int> allowList {EnumerationIndex,FunctionIndex,PropertyIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/class.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(ClassIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),ClassIndex);
    QCOMPARE(meta->name(),"class");
    QCOMPARE(meta->label(),"Class");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createEnumeration()
{
    static const QSet<int> allowList {EnumerationValueIndex};
    static const QIcon testIcon(":/cpp/enumeration.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(EnumerationIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationIndex);
    QCOMPARE(meta->name(),"enumeration");
    QCOMPARE(meta->label(),"Enumeration");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createEnumerationValue()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/enumeration_value.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(EnumerationValueIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationValueIndex);
    QCOMPARE(meta->name(),"enumerationvalue");
    QCOMPARE(meta->label(),"Enumeration Value");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createException()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/exception.svg");
    std::unique_ptr<AbstractBlock> block (_language->createBlock(ExceptionIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),ExceptionIndex);
    QCOMPARE(meta->name(),"exception");
    QCOMPARE(meta->label(),"Exception");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createFunction()
{
    static const QSet<int> allowList {ExceptionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/public_function.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(FunctionIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),FunctionIndex);
    QCOMPARE(meta->name(),"function");
    QCOMPARE(meta->label(),"Function");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
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
    std::unique_ptr<AbstractBlock> block(_language->createBlock(NamespaceIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),NamespaceIndex);
    QCOMPARE(meta->name(),"namespace");
    QCOMPARE(meta->label(),"Namespace");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createProperty()
{
    static const QSet<int> allowList {FunctionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/property.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(PropertyIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),PropertyIndex);
    QCOMPARE(meta->name(),"property");
    QCOMPARE(meta->label(),"Property");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createUnion()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/union.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(UnionIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),UnionIndex);
    QCOMPARE(meta->name(),"union");
    QCOMPARE(meta->label(),"Union");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::createVariable()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/variable.svg");
    std::unique_ptr<AbstractBlock> block(_language->createBlock(VariableIndex));
    auto meta = block->meta();
    QCOMPARE(meta->index(),VariableIndex);
    QCOMPARE(meta->name(),"variable");
    QCOMPARE(meta->label(),"Variable");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppLanguage::rootIndex()
{
    QCOMPARE(_language->rootIndex(),NamespaceIndex);
}

void TestCppLanguage::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppLanguage)
#include "test.moc"
