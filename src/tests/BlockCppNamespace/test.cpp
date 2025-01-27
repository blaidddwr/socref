#include <QtTest>
#include "BlockCpp.h"
#include "BlockCppNamespace.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "../utility.h"
using NamespaceBlock = Block::Cpp::Namespace;
using namespace Block::Cpp;

class TestNamespaceCppBlock: public QObject
{
    Q_OBJECT
    Block::Cpp::Namespace* _block;
    Language::Abstract* _language {nullptr};
private slots:
    void initTestCase();
    void descriptionProperty();
    void displayIconProperty();
    void displayTextProperty();
    void loadFromMap();
    void nameProperty();
    void saveToMap();
    void setState();
    void state();
    void cleanupTestCase();
};

void TestNamespaceCppBlock::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    _language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<NamespaceBlock*>(_language->create(NamespaceIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"namespace");
    QCOMPARE(_block->description(),"Detailed description.");
}

void TestNamespaceCppBlock::descriptionProperty()
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

void TestNamespaceCppBlock::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/namespace.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestNamespaceCppBlock::displayTextProperty()
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

void TestNamespaceCppBlock::loadFromMap()
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = qobject_cast<NamespaceBlock*>(_language->create(NamespaceIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->name(),testName);
    QCOMPARE(block->description(),testDescription);
    delete block;
}

void TestNamespaceCppBlock::nameProperty()
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

void TestNamespaceCppBlock::saveToMap()
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = qobject_cast<NamespaceBlock*>(_language->create(NamespaceIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}

void TestNamespaceCppBlock::setState()
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

void TestNamespaceCppBlock::state()
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

void TestNamespaceCppBlock::cleanupTestCase()
{
    delete _block;
}

QTEST_MAIN(TestNamespaceCppBlock)
#include "test.moc"
