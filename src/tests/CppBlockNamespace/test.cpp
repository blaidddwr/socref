#include <QtTest>
#include "CppBlock.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;

class TestCppBlockNamespace: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Namespace* _block;
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

void TestCppBlockNamespace::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Namespace*>(_language->createBlock(NamespaceIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"namespace");
    QCOMPARE(_block->description(),"Detailed description.");
}

void TestCppBlockNamespace::descriptionProperty()
{
    static const QString testDescription  = "Testing\n\n 1 2\n 3";
    QSignalSpy spy(_block,&Namespace::descriptionChanged);
    _block->setDescription(testDescription);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testDescription);
    QCOMPARE(_block->description(),testDescription);
}

void TestCppBlockNamespace::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/namespace.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockNamespace::displayTextProperty()
{
    static const QString testName  = "TestingDisplay123";
    QSignalSpy spy(_block,&Namespace::displayTextChanged);
    _block->setName(testName);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testName);
    QCOMPARE(_block->displayText(),testName);
}

void TestCppBlockNamespace::loadFromMap()
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = qobject_cast<Namespace*>(_language->createBlock(NamespaceIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->name(),testName);
    QCOMPARE(block->description(),testDescription);
    delete block;
}

void TestCppBlockNamespace::nameProperty()
{
    static const QString testName  = "Testing123";
    QSignalSpy spy(_block,&Namespace::nameChanged);
    _block->setName(testName);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testName);
    QCOMPARE(_block->name(),testName);
}

void TestCppBlockNamespace::saveToMap()
{
    static const QString testName = "Testing123";
    static const QString testDescription = "Testing\n\n 1\\ 2\n 3";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
    };
    auto block = qobject_cast<Namespace*>(_language->createBlock(NamespaceIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}

void TestCppBlockNamespace::setState()
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

void TestCppBlockNamespace::state()
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

void TestCppBlockNamespace::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockNamespace)
#include "test.moc"
