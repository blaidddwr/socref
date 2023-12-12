#include "TestBlockCppNamespace.h"
#include <QSignalSpy>
#include <QTest>
#include "BlockCppNamespace.h"
#include "Exception.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "Test.h"
namespace Test {
namespace Block {
namespace Cpp {


void Namespace::initTestCase(
)
{
    _block = create();
    QVERIFY(_block);
}


void Namespace::descriptionProperty(
)
{
    static const QString testDescription  = "Testing\n\n 1 2\n 3";
    QSignalSpy spy(_block,&::Block::Cpp::Namespace::descriptionChanged);
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
    static const QString testDescription = "Testing\n\n 1 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = create();
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
    QSignalSpy spy(_block,&::Block::Cpp::Namespace::nameChanged);
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
    static const QString testDescription = "Testing\n\n 1 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = create();
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
    auto child0 = create();
    auto child1 = create();
    child0->setName(testScope.at(0));
    child1->setName(testScope.at(1));
    _block->append(child0);
    child0->append(child1);
    QCOMPARE(child1->scope(),testScope.join("::"));
    QCOMPARE(child0->scope(),testScope.at(0));
    QCOMPARE(_block->scope(),"ROOT");
}


void Namespace::cleanupTestCase(
)
{
    delete _block;
}


::Block::Cpp::Namespace* Namespace::create(
)
{
    using namespace ::Block::Cpp;
    auto factory = Factory::Language::instance();
    auto index = factory->indexFromName("cpp");
    auto ret = qobject_cast<::Block::Cpp::Namespace*>(
        factory->get(index)->create(NamespaceIndex,this)
    );
    G_ASSERT(ret);
    return ret;
}
}
}
}
