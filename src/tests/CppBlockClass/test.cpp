#include <QtTest>
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "CppLanguage.h"
#include "../utility.h"
using namespace Cpp::Block;

class TestCppBlockClass: public QObject
{
    Q_OBJECT
    Cpp::Language* _language;
    Class* _block;
private slots:
    void initTestCase();
    void displayIconProperty();
    void displayTextProperty();
    void loadFromMap();
    void loadFromMapLegacy();
    void parentsProperty();
    void saveToMap();
    void setState();
    void state();
    void templatesProperty();
    void cleanupTestCase();
};

void TestCppBlockClass::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    static const QIcon testIcon(":/cpp/class.svg");
    _language = new Cpp::Language;
    _language->setParent(this);
    _block = qobject_cast<Class*>(_language->createBlock(ClassIndex,this));
    QVERIFY(_block);
    QCOMPARE(_block->name(),"class");
    QVERIFY(_block->parents().isEmpty());
    QVERIFY(_block->templates().isEmpty());
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}

void TestCppBlockClass::displayIconProperty()
{
    static const QIcon testIcon(":/cpp/class.svg");
    static const QIcon testIconAbstract(":/cpp/abstract_class.svg");
    static const QIcon testIconVirtual(":/cpp/virtual_class.svg");
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    QSignalSpy spy(_block,&Class::displayIconChanged);
    auto verify = [&spy,this](const QIcon& icon)
    {
        QCOMPARE(spy.count(),1);
        auto arguments = spy.takeLast();
        QCOMPARE(arguments.size(),1);
        QVERIFY(areIconsEqual(arguments.at(0).value<QIcon>(),icon));
        QVERIFY(areIconsEqual(_block->displayIcon(),icon));
        spy.clear();
    };
    auto function = qobject_cast<Function*>(_language->createBlock(FunctionIndex,this));
    QVERIFY(function);
    _block->append(function);
    QCOMPARE(spy.count(),1);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    spy.clear();
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconAbstract);
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconVirtual);
    delete function;
    verify(testIcon);
    function = qobject_cast<Function*>(_language->createBlock(FunctionIndex,this));
    QVERIFY(function);
    auto property = qobject_cast<Property*>(_language->createBlock(PropertyIndex,this));
    QVERIFY(property);
    _block->append(property);
    QCOMPARE(spy.count(),1);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    spy.clear();
    property->append(function);
    QCOMPARE(spy.count(),1);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    spy.clear();
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconAbstract);
    function->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
        );
    verify(testIconVirtual);
    delete property;
    verify(testIcon);
}

void TestCppBlockClass::displayTextProperty()
{
    _block->setName("class123");
    _block->setTemplates({});
    QCOMPARE(_block->displayText(),"class123");
    QSignalSpy spy(_block,&Class::displayTextChanged);
    _block->setTemplates({"class A","class B"});
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),"class123 -> template<class A,class B>");
    QCOMPARE(_block->displayText(),"class123 -> template<class A,class B>");
}

void TestCppBlockClass::loadFromMap()
{
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B"};
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"parents",testParents.join(';')}
        ,{"templates",testTemplates.join(';')}
    };
    auto block = qobject_cast<Class*>(_language->createBlock(ClassIndex));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->parents(),testParents);
    QCOMPARE(block->templates(),testTemplates);
    delete block;
}

void TestCppBlockClass::loadFromMapLegacy()
{
    static const QStringList testParents {"parent1","parent2"};
    static const QString testTemplateString = "template <  class A   ,  class B>";
    static const QStringList testTemplates {"class A","class B"};
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"parents",testParents.join("\n\n")}
        ,{"template",testTemplateString}
    };
    auto block = qobject_cast<Class*>(_language->createBlock(ClassIndex,this));
    QVERIFY(block);
    block->loadFromMap(testData,Socref_Legacy);
    QCOMPARE(block->parents(),testParents);
    QCOMPARE(block->templates(),testTemplates);
    delete block;
}

void TestCppBlockClass::parentsProperty()
{
    static const QStringList testParents  = {"parent1","parent2"};
    QSignalSpy spy(_block,&Class::parentsChanged);
    _block->setParents(testParents);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testParents);
    QCOMPARE(_block->parents(),testParents);
}

void TestCppBlockClass::saveToMap()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B>"};
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"parents",testParents.join(';')}
        ,{"templates",testTemplates.join(';')}
    };
    auto block = qobject_cast<Class*>(_language->createBlock(ClassIndex,this));
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setParents(testParents);
    block->setTemplates(testTemplates);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}

void TestCppBlockClass::setState()
{
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B"};
    static const QHash<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"parents",testParents}
        ,{"templates",testTemplates}
    };
    _block->setState(testData);
    QCOMPARE(_block->parents(),testParents);
    QCOMPARE(_block->templates(),testTemplates);
}

void TestCppBlockClass::state()
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B"};
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"parents",testParents}
        ,{"templates",testTemplates}
    };
    _block->setName(testName);
    _block->setDescription(testDescription);
    _block->setParents(testParents);
    _block->setTemplates(testTemplates);
    auto data = _block->state();
    QCOMPARE(data,testData);
}

void TestCppBlockClass::templatesProperty()
{
    static const QStringList testTemplates {"class A","class B>"};
    QSignalSpy spy(_block,&Class::templatesChanged);
    _block->setTemplates(testTemplates);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testTemplates);
    QCOMPARE(_block->templates(),testTemplates);
}

void TestCppBlockClass::cleanupTestCase()
{
    delete _block;
    delete _language;
}

QTEST_MAIN(TestCppBlockClass)
#include "test.moc"
