#include "TestBlockCppExceptionItem.h"
#include <QtTest>
#include "BlockCppException.h"
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
#include "Test.h"
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
    auto language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<ExceptionBlock*>(language->create(ExceptionIndex,this));
    QVERIFY(_block);
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
