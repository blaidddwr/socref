#include <QtTest>
#include "BlockCpp.h"
#include "BlockCppEnumeration.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "../utility.h"
using EnumerationBlock = Block::Cpp::Enumeration;
using namespace Block::Cpp;

class TestEnumerationCppBlock: public QObject
{
    Q_OBJECT
    Block::Cpp::Enumeration* _block;
    Language::Abstract* _language {nullptr};
private slots:
    void initTestCase();
    void classProperty();
    void displayIconProperty();
    void loadFromMap();
    void saveToMap();
    void setState();
    void state();
    void cleanupTestCase();
};

void TestEnumerationCppBlock::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    _language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<EnumerationBlock*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"enumeration");
    QCOMPARE(_block->isClass(),false);
}

void TestEnumerationCppBlock::classProperty()
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

void TestEnumerationCppBlock::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/enumeration.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestEnumerationCppBlock::loadFromMap()
{
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    auto block = qobject_cast<EnumerationBlock*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->isClass(),true);
    delete block;
}

void TestEnumerationCppBlock::saveToMap()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"class",true}
    };
    auto block = qobject_cast<EnumerationBlock*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setClass(true);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}

void TestEnumerationCppBlock::setState()
{
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    _block->setState(testData);
    QCOMPARE(_block->isClass(),true);
}

void TestEnumerationCppBlock::state()
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

void TestEnumerationCppBlock::cleanupTestCase()
{
    delete _block;
}

QTEST_MAIN(TestEnumerationCppBlock)
#include "test.moc"
