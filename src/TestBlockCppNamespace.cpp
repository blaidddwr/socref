#include "TestBlockCppNamespace.h"
#include <QtTest>
#include "BlockCppNamespace.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {
using NamespaceBlock = ::Block::Cpp::Namespace;
using namespace ::Block::Cpp;


void Namespace::initTestCase(
)
{
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<NamespaceBlock>(NamespaceIndex);
    QCOMPARE(_block->name(),"namespace");
    QCOMPARE(_block->description(),"Detailed description.");
}


void Namespace::descriptionProperty(
)
{
    static const QString testDescription  = "Testing\n\n 1 2\n 3";
    QSignalSpy spy(_block,&NamespaceBlock::descriptionChanged);
    _block->setDescription(testDescription);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testDescription);
    QCOMPARE(_block->description(),testDescription);
}


void Namespace::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/namespace.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void Namespace::displayTextProperty(
)
{
    static const QString testName  = "TestingDisplay123";
    QSignalSpy spy(_block,&::Block::Abstract::displayTextChanged);
    _block->setName(testName);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testName);
    QCOMPARE(_block->displayText(),testName);
}


void Namespace::loadFromMap(
)
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = create<NamespaceBlock>(NamespaceIndex);
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->name(),testName);
    QCOMPARE(block->description(),testDescription);
    delete block;
}


void Namespace::nameProperty(
)
{
    static const QString testName  = "Testing123";
    QSignalSpy spy(_block,&NamespaceBlock::nameChanged);
    _block->setName(testName);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testName);
    QCOMPARE(_block->name(),testName);
}


void Namespace::saveToMap(
)
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = create<NamespaceBlock>(NamespaceIndex);
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}


void Namespace::scopeProperty(
)
{
    static const QStringList testScope {"Test0","Test1"};
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create<NamespaceBlock>(NamespaceIndex);
    auto child1 = create<NamespaceBlock>(NamespaceIndex);
    child0->setName(testScope.at(0));
    child1->setName(testScope.at(1));
    _block->append(child0);
    child0->append(child1);
    QCOMPARE(child1->scope(),testScope.join("::"));
    QCOMPARE(child0->scope(),testScope.at(0));
    QCOMPARE(_block->scope(),"ROOT");
}


void Namespace::setState(
)
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    _block->setState(testData);
    QCOMPARE(_block->name(),testName);
    QCOMPARE(_block->description(),testDescription);
}


void Namespace::state(
)
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    auto data = _block->state();
    QCOMPARE(data,testData);
}


void Namespace::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
