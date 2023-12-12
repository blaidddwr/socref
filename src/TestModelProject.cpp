#include "TestModelProject.h"
#include <QtTest>
#include "FactoryLanguage.h"
#include "ModelProject.h"
namespace Test {
namespace Model {


void Project::initTestCase(
)
{
    QVERIFY(createTestDir());
}


void Project::importLegacy(
)
{
    auto factory = Factory::Language::instance();
    int index = factory->indexFromName("test");
    auto project = ::Model::Project::import(testXmlLegacy(),this);
    QCOMPARE(project->name(),"Testing123");
    QCOMPARE(project->language(),factory->get(index));
    QCOMPARE(project->relativeParsePath(),"../testing");
}


void Project::loadConstruct(
)
{
    auto factory = Factory::Language::instance();
    int index = factory->indexFromName("test");
    ::Model::Project project(testProjDir());
    QCOMPARE(project.name(),"Testing123");
    QCOMPARE(project.language(),factory->get(index));
    QCOMPARE(project.relativeParsePath(),"../testing");
}


void Project::cleanupTestCase(
)
{
    cleanup();
}
}
}
