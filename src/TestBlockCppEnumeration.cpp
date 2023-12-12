#include "TestBlockCppEnumeration.h"
#include <QSignalSpy>
#include <QTest>
#include "BlockCppEnumeration.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {


void Enumeration::initTestCase(
)
{
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _blockIndex = language()->indexFromName("enumeration");
    QVERIFY(_blockIndex >= 0);
    _block = create<::Block::Cpp::Enumeration>(_blockIndex);
    QVERIFY(_block);
}


void Enumeration::classProperty(
)
{
    QCOMPARE(_block->isClass(),false);
    QSignalSpy spy(_block,&::Block::Cpp::Enumeration::classChanged);
    _block->setClass(true);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),true);
    QCOMPARE(_block->isClass(),true);
}


void Enumeration::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/enumeration.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void Enumeration::loadFromMap(
)
{
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    auto block = create<::Block::Cpp::Enumeration>(_blockIndex);
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->isClass(),true);
    delete block;
}


void Enumeration::saveToMap(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"class",true}
    };
    auto block = create<::Block::Cpp::Enumeration>(_blockIndex);
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setClass(true);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}


void Enumeration::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
