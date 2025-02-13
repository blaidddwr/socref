#include <QtTest>
#include "CppBlock.h"
#include "CppBlockVariable.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp;

class TestCppBlockVariable: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Variable* _block;
private slots:
    void initTestCase();
    void assignmentProperty();
    void displayIconProperty();
    void loadFromMap();
    void saveToMap();
    void setState();
    void state();
    void typeProperty();
    void cleanupTestCase();
};

void TestCppBlockVariable::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Variable*>(_language->createBlock(VariableIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"variable");
    QCOMPARE(_block->type(),"int");
    QCOMPARE(_block->assignment(),"");
}

void TestCppBlockVariable::assignmentProperty()
{
    static const QString testAssignment = "0";
    _block->setAssignment("");
    QSignalSpy spy(_block,&Variable::assignmentChanged);
    _block->setAssignment(testAssignment);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testAssignment);
    QCOMPARE(_block->assignment(),testAssignment);
}

void TestCppBlockVariable::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/variable.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockVariable::loadFromMap()
{
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    auto block = qobject_cast<Variable*>(_language->createBlock(VariableIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->type(),testType);
    QCOMPARE(block->assignment(),testAssignment);
    delete block;
}

void TestCppBlockVariable::saveToMap()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    auto block = qobject_cast<Variable*>(_language->createBlock(VariableIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setType(testType);
    block->setAssignment(testAssignment);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
}

void TestCppBlockVariable::setState()
{
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    _block->setState(testData);
    QCOMPARE(_block->type(),testType);
    QCOMPARE(_block->assignment(),testAssignment);
}

void TestCppBlockVariable::state()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    _block->setType(testType);
    _block->setAssignment(testAssignment);
    auto data = _block->state();
    QCOMPARE(data,testData);
}

void TestCppBlockVariable::typeProperty()
{
    static const QString testType = "char**";
    _block->setAssignment("int");
    QSignalSpy spy(_block,&Variable::typeChanged);
    _block->setType(testType);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testType);
    QCOMPARE(_block->type(),testType);
}

void TestCppBlockVariable::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockVariable)
#include "test.moc"
