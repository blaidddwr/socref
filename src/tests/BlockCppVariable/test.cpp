#include <QtTest>
#include "BlockCpp.h"
#include "Language.h"
#include "BlockCppVariable.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "../utility.h"
using VariableBlock = Block::Cpp::Variable;
using namespace Block::Cpp;

class TestVariableCppBlock: public QObject
{
    Q_OBJECT
    Block::Cpp::Variable* _block {nullptr};
    Language::Abstract* _language {nullptr};
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

void TestVariableCppBlock::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    _language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<VariableBlock*>(_language->createBlock(VariableIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"variable");
    QCOMPARE(_block->type(),"int");
    QCOMPARE(_block->assignment(),"");
}

void TestVariableCppBlock::assignmentProperty()
{
    static const QString testAssignment = "0";
    _block->setAssignment("");
    QSignalSpy spy(_block,&VariableBlock::assignmentChanged);
    _block->setAssignment(testAssignment);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testAssignment);
    QCOMPARE(_block->assignment(),testAssignment);
}

void TestVariableCppBlock::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/variable.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestVariableCppBlock::loadFromMap()
{
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    auto block = qobject_cast<VariableBlock*>(_language->createBlock(VariableIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->type(),testType);
    QCOMPARE(block->assignment(),testAssignment);
    delete block;
}

void TestVariableCppBlock::saveToMap()
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
    auto block = qobject_cast<VariableBlock*>(_language->createBlock(VariableIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setType(testType);
    block->setAssignment(testAssignment);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
}

void TestVariableCppBlock::setState()
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

void TestVariableCppBlock::state()
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

void TestVariableCppBlock::typeProperty()
{
    static const QString testType = "char**";
    _block->setAssignment("int");
    QSignalSpy spy(_block,&VariableBlock::typeChanged);
    _block->setType(testType);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testType);
    QCOMPARE(_block->type(),testType);
}

void TestVariableCppBlock::cleanupTestCase()
{
    QVERIFY(_block);
    delete _block;
}

QTEST_MAIN(TestVariableCppBlock)
#include "test.moc"
