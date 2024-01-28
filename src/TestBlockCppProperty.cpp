#include "TestBlockCppProperty.h"
#include <QtTest>
#include "BlockCppFunction.h"
#include "BlockCppProperty.h"
#include "FactoryLanguage.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {
using PropertyBlock = ::Block::Cpp::Property;
using namespace ::Block::Cpp;


void Property::initTestCase(
)
{
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<PropertyBlock>(PropertyIndex);
    QCOMPARE(_block->name(),"property");
}


void Property::displayIconProperty(
)
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
    auto function = create<Function>(FunctionIndex);
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


void Property::isAbstract(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(!_block->isAbstract());
    auto function = create<Function>(FunctionIndex);
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


void Property::isVirtual(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(!_block->isVirtual());
    auto function = create<Function>(FunctionIndex);
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


void Property::cleanupTestCase(
)
{
    QVERIFY(_block);
    delete _block;
}
}
}
}
