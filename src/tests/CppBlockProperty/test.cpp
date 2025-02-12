#include <QtTest>
#include "CppBlock.h"
#include "CppBlockFunction.h"
#include "CppBlockProperty.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;

class TestCppBlockProperty: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Property* _block;
private slots:
    void initTestCase();
    void displayIconProperty();
    void isAbstract();
    void isVirtual();
    void cleanupTestCase();
};

void TestCppBlockProperty::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Property*>(_language->createBlock(PropertyIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"property");
}

void TestCppBlockProperty::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/property.svg");
    static const QIcon testIconAbstract(":/cpp/abstract_property.svg");
    static const QIcon testIconVirtual(":/cpp/virtual_property.svg");
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    QSignalSpy spy(_block,&Property::displayIconChanged);
    auto verify = [&spy,this](const QIcon& icon)
    {
        QCOMPARE(spy.count(),1);
        auto arguments = spy.takeLast();
        QCOMPARE(arguments.size(),1);
        QVERIFY(areIconsEqual(arguments.at(0).value<QIcon>(),icon));
        QVERIFY(areIconsEqual(_block->displayIcon(),icon));
        spy.clear();
    };
    auto function = qobject_cast<Function*>(_language->createBlock(FunctionIndex,this));
    QVERIFY(function);
    _block->append(function);
    QCOMPARE(spy.count(),1);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    spy.clear();
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconAbstract);
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconVirtual);
    delete function;
    verify(testIcon);
}

void TestCppBlockProperty::isAbstract()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(!_block->isAbstract());
    auto function = qobject_cast<Function*>(_language->createBlock(FunctionIndex,this));
    QVERIFY(function);
    _block->append(function);
    QVERIFY(!_block->isAbstract());
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
        );
    QVERIFY(_block->isAbstract());
    delete function;
    QVERIFY(!_block->isAbstract());
}

void TestCppBlockProperty::isVirtual()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(!_block->isVirtual());
    auto function = qobject_cast<Function*>(_language->createBlock(FunctionIndex,this));
    QVERIFY(function);
    _block->append(function);
    QVERIFY(!_block->isVirtual());
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
        );
    QVERIFY(_block->isVirtual());
    delete function;
    QVERIFY(!_block->isVirtual());
}

void TestCppBlockProperty::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockProperty)
#include "test.moc"
