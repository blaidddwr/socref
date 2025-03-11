#include <QtTest>
#include "CppBlock.h"
#include "CppBlockException.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp;

class TestCppBlockException: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Exception* _block;
private slots:
    void initTestCase();
    void displayIconProperty();
    void cleanupTestCase();
};

void TestCppBlockException::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Exception*>(_language->createBlock(ExceptionIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"exception");
}

void TestCppBlockException::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/exception.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockException::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockException)
#include "test.moc"
