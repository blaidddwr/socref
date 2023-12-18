#include "TestBlockCppClass.h"
#include <QtTest>
#include "BlockCppClass.h"
#include "BlockCppFunction.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {
using ClassBlock = ::Block::Cpp::Class;
using namespace ::Block::Cpp;


void Class::initTestCase(
)
{
    static const QIcon testIcon(":/cpp/class.svg");
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<ClassBlock>(ClassIndex);
    QVERIFY(_block->parents().isEmpty());
    QVERIFY(_block->templates().isEmpty());
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void Class::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/class.svg");
    static const QIcon testIconAbstract(":/cpp/abstract_class.svg");
    static const QIcon testIconVirtual(":/cpp/virtual_class.svg");
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
    QSignalSpy spy(_block,&ClassBlock::displayIconChanged);
    auto verify = [&spy,this](const QIcon& icon)
    {
        QCOMPARE(spy.count(),1);
        auto arguments = spy.takeLast();
        QCOMPARE(arguments.size(),1);
        QVERIFY(areIconsEqual(arguments.at(0).value<QIcon>(),icon));
        QVERIFY(areIconsEqual(_block->displayIcon(),icon));
        spy.clear();
    };
    auto function = create<Function>(FunctionIndex);
    _block->append(function);
    QCOMPARE(spy.count(),0);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
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
}


void Class::loadFromMap(
)
{
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B"};
    static const QMap<QString,QVariant> testData {
        {"name","name"}
        ,{"description","description"}
        ,{"parents",testParents.join(';')}
        ,{"templates",testTemplates.join(';')}
    };
    auto block = create<ClassBlock>(ClassIndex);
    QVERIFY(block);
    block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(block->parents(),testParents);
    QCOMPARE(block->templates(),testTemplates);
    delete block;
}


void Class::loadFromMapLegacy(
)
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
    auto block = create<ClassBlock>(ClassIndex);
    QVERIFY(block);
    block->loadFromMap(testData,Socref_Legacy);
    QCOMPARE(block->parents(),testParents);
    QCOMPARE(block->templates(),testTemplates);
    delete block;
}


void Class::parentsProperty(
)
{
    static const QStringList testParents  = {"parent1","parent2"};
    QSignalSpy spy(_block,&ClassBlock::parentsChanged);
    _block->setParents(testParents);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testParents);
    QCOMPARE(_block->parents(),testParents);
}


void Class::saveToMap(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QStringList testParents {"parent1","parent2"};
    static const QStringList testTemplates {"class A","class B>"};
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"parents",testParents.join(';')}
        ,{"template",testTemplates.join(';')}
    };
    auto block = create<ClassBlock>(ClassIndex);
    QVERIFY(block);
    block->setName(testName);
    block->setDescription(testDescription);
    block->setParents(testParents);
    block->setTemplates(testTemplates);
    auto data = block->saveToMap();
    QCOMPARE(data,testData);
    delete block;
}


void Class::templatesProperty(
)
{
    static const QStringList testTemplates {"class A","class B>"};
    QSignalSpy spy(_block,&ClassBlock::templatesChanged);
    _block->setTemplates(testTemplates);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testTemplates);
    QCOMPARE(_block->templates(),testTemplates);
}


void Class::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
