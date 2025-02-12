#include <QtTest>
#include "AbstractBlock.h"
#include "CppQtBlock.h"
#include "CppQtLanguage.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
#include "../utility.h"
using namespace CppQt::Block;

class TestCppQtLanguage: public QObject
{
    Q_OBJECT
    CppQt::Language* _language;
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

void TestCppQtLanguage::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    Q_INIT_RESOURCE(cppqt);
    static const QIcon testIcon(":/cppqt.svg");
    _language = new CppQt::Language;
    _language->setParent(this);
    auto meta = _language->meta();
    QCOMPARE(meta->name(),"cppqt");
    QCOMPARE(meta->label(),"C++/Qt");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
}

void TestCppQtLanguage::createClass()
{
    static const QSet<int> allowList {EnumerationIndex,FunctionIndex,PropertyIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/class.svg");
    auto block = _language->createBlock(ClassIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ClassIndex);
    QCOMPARE(meta->name(),"class");
    QCOMPARE(meta->label(),"Class");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createEnumeration()
{
    static const QSet<int> allowList {EnumerationValueIndex};
    static const QIcon testIcon(":/cpp/enumeration.svg");
    auto block = _language->createBlock(EnumerationIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationIndex);
    QCOMPARE(meta->name(),"enumeration");
    QCOMPARE(meta->label(),"Enumeration");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createEnumerationValue()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/enumeration_value.svg");
    auto block = _language->createBlock(EnumerationValueIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),EnumerationValueIndex);
    QCOMPARE(meta->name(),"enumerationvalue");
    QCOMPARE(meta->label(),"Enumeration Value");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createException()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/exception.svg");
    auto block = _language->createBlock(ExceptionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),ExceptionIndex);
    QCOMPARE(meta->name(),"exception");
    QCOMPARE(meta->label(),"Exception");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createFunction()
{
    static const QSet<int> allowList {ExceptionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/public_function.svg");
    auto block = _language->createBlock(FunctionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),FunctionIndex);
    QCOMPARE(meta->name(),"function");
    QCOMPARE(meta->label(),"Function");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createNamespace()
{
    static const QSet<int> allowList {
        ClassIndex
        ,EnumerationIndex
        ,FunctionIndex
        ,NamespaceIndex
        ,UnionIndex
    };
    static const QIcon testIcon(":/cpp/namespace.svg");
    auto block = _language->createBlock(NamespaceIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),NamespaceIndex);
    QCOMPARE(meta->name(),"namespace");
    QCOMPARE(meta->label(),"Namespace");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createProperty()
{
    static const QSet<int> allowList {FunctionIndex,VariableIndex};
    static const QIcon testIcon(":/cpp/property.svg");
    auto block = _language->createBlock(PropertyIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),PropertyIndex);
    QCOMPARE(meta->name(),"property");
    QCOMPARE(meta->label(),"Property");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createUnion()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/union.svg");
    auto block = _language->createBlock(UnionIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),UnionIndex);
    QCOMPARE(meta->name(),"union");
    QCOMPARE(meta->label(),"Union");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::createVariable()
{
    static const QSet<int> allowList {};
    static const QIcon testIcon(":/cpp/variable.svg");
    auto block = _language->createBlock(VariableIndex,this);
    auto meta = block->meta();
    QCOMPARE(meta->index(),VariableIndex);
    QCOMPARE(meta->name(),"variable");
    QCOMPARE(meta->label(),"Variable");
    QVERIFY(areIconsEqual(meta->displayIcon(),testIcon));
    QCOMPARE(meta->allowList(),allowList);
}

void TestCppQtLanguage::rootIndex()
{
    QCOMPARE(_language->rootIndex(),NamespaceIndex);
}

void TestCppQtLanguage::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppQtLanguage)
#include "test.moc"
