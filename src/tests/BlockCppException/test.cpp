#include <QtTest>
#include "BlockCpp.h"
#include "BlockCppException.h"
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
#include "../utility.h"
using ExceptionBlock = Block::Cpp::Exception;
using namespace Block::Cpp;

class TestExceptionCppBlock: public QObject
{
    Q_OBJECT
    Block::Cpp::Exception* _block {nullptr};
private slots:
    void initTestCase();
    void displayIconProperty();
    void cleanupTestCase();
};

void TestExceptionCppBlock::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    auto language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<ExceptionBlock*>(language->createBlock(ExceptionIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"exception");
}

void TestExceptionCppBlock::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/exception.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestExceptionCppBlock::cleanupTestCase()
{
    QVERIFY(_block);
    delete _block;
}

QTEST_MAIN(TestExceptionCppBlock)
#include "test.moc"
