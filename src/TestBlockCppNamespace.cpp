#include "TestBlockCppNamespace.h"
#include <QSignalSpy>
#include <QTest>
#include "BlockCppNamespace.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
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


void Namespace::displayText(
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
    Q_ASSERT(ret);
    return ret;
}
}
}
}
