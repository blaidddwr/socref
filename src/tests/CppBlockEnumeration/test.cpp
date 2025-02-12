#include <QtTest>
#include "CppBlock.h"
#include "CppBlockEnumeration.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;

class TestCppBlockEnumeration: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Enumeration* _block;
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

void TestCppBlockEnumeration::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Enumeration*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"enumeration");
    QCOMPARE(_block->isClass(),false);
}

void TestCppBlockEnumeration::classProperty()
{
    QCOMPARE(_block->isClass(),false);
    QSignalSpy spy(_block,&Enumeration::classChanged);
    _block->setClass(true);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),true);
    QCOMPARE(_block->isClass(),true);
}

void TestCppBlockEnumeration::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/enumeration.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockEnumeration::loadFromMap()
{
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    auto block = qobject_cast<Enumeration*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->isClass(),true);
    delete block;
}

void TestCppBlockEnumeration::saveToMap()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"class",true}
    };
    auto block = qobject_cast<Enumeration*>(_language->createBlock(EnumerationIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setClass(true);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}

void TestCppBlockEnumeration::setState()
{
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"class",true}
    };
    _block->setState(testData);
    QCOMPARE(_block->isClass(),true);
}

void TestCppBlockEnumeration::state()
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

void TestCppBlockEnumeration::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockEnumeration)
#include "test.moc"
