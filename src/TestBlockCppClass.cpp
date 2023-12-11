#include "TestBlockCppClass.h"
#include <QSignalSpy>
#include <QTest>
#include "BlockCppClass.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "Test.h"
namespace Test {
namespace Block {
namespace Cpp {


void Class::initTestCase(
)
{
    _block = create();
    QVERIFY(_block);
}


void Class::displayIconProperty(
)
{
    static const QIcon testIcon(":/cpp/class.svg");
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
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
    auto block = create();
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
    auto block = create();
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
    QSignalSpy spy(_block,&::Block::Cpp::Class::parentsChanged);
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
    auto block = create();
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
    QSignalSpy spy(_block,&::Block::Cpp::Class::templatesChanged);
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


::Block::Cpp::Class* Class::create(
)
{
    using namespace ::Block::Cpp;
    auto factory = Factory::Language::instance();
    auto index = factory->indexFromName("cpp");
    auto ret = qobject_cast<::Block::Cpp::Class*>(
        factory->get(index)->create(ClassIndex,this)
        );
    Q_ASSERT(ret);
    return ret;
}
}
}
}
