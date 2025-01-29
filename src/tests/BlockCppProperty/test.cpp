#include <QtTest>
#include "BlockCpp.h"
#include "BlockCppFunction.h"
#include "BlockCppProperty.h"
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
#include "../utility.h"
using PropertyBlock = Block::Cpp::Property;
using namespace Block::Cpp;

class TestPropertyCppBlock: public QObject
{
    Q_OBJECT
    Block::Cpp::Property* _block {nullptr};
    Language::Abstract* _language {nullptr};
private slots:
    void initTestCase();
    void displayIconProperty();
    void isAbstract();
    void isVirtual();
    void cleanupTestCase();
};

void TestPropertyCppBlock::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    _language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<PropertyBlock*>(_language->createBlock(PropertyIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"property");
}

void TestPropertyCppBlock::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/property.svg");
    static const QIcon testIconAbstract(":/cpp/abstract_property.svg");
    static const QIcon testIconVirtual(":/cpp/virtual_property.svg");
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    QSignalSpy spy(_block,&PropertyBlock::displayIconChanged);
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

void TestPropertyCppBlock::isAbstract()
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

void TestPropertyCppBlock::isVirtual()
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

void TestPropertyCppBlock::cleanupTestCase()
{
    QVERIFY(_block);
    delete _block;
}

QTEST_MAIN(TestPropertyCppBlock)
#include "test.moc"
