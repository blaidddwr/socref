#include <QtTest>
#include "CppBlock.h"
#include "CppBlockEnumerationValue.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp;

class TestCppBlockEnumerationValue: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    EnumerationValue* _block;
private slots:
    void initTestCase();
    void assignmentProperty();
    void displayIconProperty();
    void loadFromMap();
    void saveToMap();
    void setState();
    void state();
    void cleanupTestCase();
};

void TestCppBlockEnumerationValue::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<EnumerationValue*>(_language->createBlock(EnumerationValueIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"value");
    QCOMPARE(_block->assignment(),"");
}

void TestCppBlockEnumerationValue::assignmentProperty()
{
    QCOMPARE(_block->assignment(),"");
    QSignalSpy spy(_block,&EnumerationValue::assignmentChanged);
    _block->setAssignment("test");
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),"test");
    QCOMPARE(_block->assignment(),"test");
}

void TestCppBlockEnumerationValue::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/enumeration_value.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockEnumerationValue::loadFromMap()
{
    _block->setAssignment("");
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"assignment","test"}
    };
    _block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(_block->assignment(),"test");
}

void TestCppBlockEnumerationValue::saveToMap()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testAssignment = "assignment";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"assignment",testAssignment}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    _block->setAssignment(testAssignment);
    auto data = _block->saveToMap();
    QCOMPARE(data,testData);
}

void TestCppBlockEnumerationValue::setState()
{
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"assignment","test"}
    };
    _block->setState(testData);
    QCOMPARE(_block->assignment(),"test");
}

void TestCppBlockEnumerationValue::state()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testAssignment = "assignment";
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"assignment",testAssignment}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    _block->setAssignment(testAssignment);
    auto data = _block->state();
    QCOMPARE(data,testData);
}

void TestCppBlockEnumerationValue::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockEnumerationValue)
#include "test.moc"
