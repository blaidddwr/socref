#include "TestBlockCppExceptionItem.h"
#include <QtTest>
#include "BlockCppException.h"
#include "FactoryLanguage.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {
using ExceptionBlock = ::Block::Cpp::Exception;
using namespace ::Block::Cpp;


void ExceptionItem::initTestCase(
)
{
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<ExceptionBlock>(ExceptionIndex);
    QCOMPARE(_block->name(),"exception");
}


void ExceptionItem::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/exception.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void ExceptionItem::cleanupTestCase(
)
{
    QVERIFY(_block);
    delete _block;
}
}
}
}
