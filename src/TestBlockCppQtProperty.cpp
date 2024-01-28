#include "TestBlockCppQtProperty.h"
#include <QtTest>
#include "BlockCppQtProperty.h"
#include "FactoryLanguage.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace CppQt {
using PropertyBlock = ::Block::CppQt::Property;
using namespace ::Block::CppQt;


void Property::initTestCase(
)
{
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cppqt");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<PropertyBlock>(PropertyIndex);
    QCOMPARE(_block->name(),"property");
}


void Property::notifySignal(
)
{
    static const QString testSignal = "testing123";
    QVERIFY(_block);
    _block->setNotifySignal("");
    QSignalSpy spy(_block,&PropertyBlock::notifySignalChanged);
    _block->setNotifySignal(testSignal);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testSignal);
    QCOMPARE(_block->notifySignal(),testSignal);
}


void Property::readMethod(
)
{
    static const QString testMethod = "testing123";
    QVERIFY(_block);
    _block->setReadMethod("");
    QSignalSpy spy(_block,&PropertyBlock::readMethodChanged);
    _block->setReadMethod(testMethod);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testMethod);
    QCOMPARE(_block->readMethod(),testMethod);
}


void Property::writeMethod(
)
{
    static const QString testMethod = "testing123";
    QVERIFY(_block);
    _block->setWriteMethod("");
    QSignalSpy spy(_block,&PropertyBlock::writeMethodChanged);
    _block->setWriteMethod(testMethod);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testMethod);
    QCOMPARE(_block->writeMethod(),testMethod);
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
