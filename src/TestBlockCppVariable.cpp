#include "TestBlockCppVariable.h"
#include <QtTest>
#include "BlockCppVariable.h"
#include "FactoryLanguage.h"
#include "Global.h"
namespace Test {
namespace Block {
namespace Cpp {
using VariableBlock = ::Block::Cpp::Variable;
using namespace ::Block::Cpp;


void Variable::initTestCase(
)
{
    QVERIFY(!_block);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<VariableBlock>(VariableIndex);
    QCOMPARE(_block->name(),"variable");
    QCOMPARE(_block->type(),"int");
    QCOMPARE(_block->assignment(),"");
}


void Variable::assignmentProperty(
)
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


void Variable::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/variable.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void Variable::loadFromMap(
)
{
    static const QString testType = "float";
    static const QString testAssignment = "3.14";
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"type",testType}
        ,{"assignment",testAssignment}
    };
    auto block = create<VariableBlock>(VariableIndex);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->type(),testType);
    QCOMPARE(block->assignment(),testAssignment);
    delete block;
}


void Variable::saveToMap(
)
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
    auto block = create<VariableBlock>(VariableIndex);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setType(testType);
    block->setAssignment(testAssignment);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
}


void Variable::setState(
)
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


void Variable::state(
)
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


void Variable::typeProperty(
)
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


void Variable::cleanupTestCase(
)
{
    QVERIFY(_block);
    delete _block;
}
}
}
}
