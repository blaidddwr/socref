#include "TestBlockCppEnumeration.h"
#include <QtTest>
#include "BlockCppEnumeration.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "Test.h"
namespace Test {
namespace Block {
namespace Cpp {
using EnumerationBlock = ::Block::Cpp::Enumeration;
using namespace ::Block::Cpp;


void Enumeration::initTestCase(
)
{
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    _language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<EnumerationBlock*>(_language->create(EnumerationIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"enumeration");
    QCOMPARE(_block->isClass(),false);
}


void Enumeration::classProperty(
)
{
    QCOMPARE(_block->isClass(),false);
    QSignalSpy spy(_block,&EnumerationBlock::classChanged);
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
    auto block = qobject_cast<EnumerationBlock*>(_language->create(EnumerationIndex,this));
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
    auto block = qobject_cast<EnumerationBlock*>(_language->create(EnumerationIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setClass(true);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}


void Enumeration::setState(
)
{
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    _block->setState(testData);
    QCOMPARE(_block->isClass(),true);
}


void Enumeration::state(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"class",true}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    _block->setClass(true);
    auto data = _block->state();
    QCOMPARE(data,testData);
}


void Enumeration::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
