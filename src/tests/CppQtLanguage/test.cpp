#include <QtTest>
#include "CppBlock.h"
#include "CppQtBlockFunction.h"
#include "CppQtLanguage.h"
#include "ModelMetaLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;
using CppQtFunction = CppQt::Block::Function;

class TestCppQtLanguage: public QObject
{
    Q_OBJECT
    CppQt::Language* _language;
private slots:
    void initTestCase();
    void createFunction();
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

void TestCppQtLanguage::createFunction()
{
    std::unique_ptr<AbstractBlock> block(_language->createBlock(FunctionIndex,this));
    QVERIFY(qobject_cast<CppQtFunction*>(block.get()));
}

void TestCppQtLanguage::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppQtLanguage)
#include "test.moc"
