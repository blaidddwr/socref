#include "TestBlockCppProperty.h"
#include <QtTest>
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
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
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
