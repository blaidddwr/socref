#include "TestModelProject.h"
#include <QtTest>
#include "ExceptionBase.h"
#include "FactoryLanguage.h"
#include "ModelProject.h"
#include "StreamProject.h"
namespace Test {
namespace Model {


void Project::initTestCase(
)
{
    QVERIFY(createTestDir());
}


void Project::fromDir(
)
{
    try
    {
        auto factory = Factory::Language::instance();
        int index = factory->indexFromName("test");
        auto project = Stream::Project::fromDir(testProjDir(),this);
        QCOMPARE(project->name(),"Testing123");
        QCOMPARE(project->language(),factory->get(index));
        //QCOMPARE(project->relativeParsePath(),"../testing");
        delete project;
    }
    catch (Exception::Base& e)
    {
        qDebug() << e.message();
        QVERIFY(false);
    }
}


void Project::fromXmlLegacy(
)
{
    try
    {
        auto factory = Factory::Language::instance();
        int index = factory->indexFromName("test");
        auto project = Stream::Project::fromXml(testXmlLegacy(),this);
        QCOMPARE(project->name(),"Testing123");
        QCOMPARE(project->language(),factory->get(index));
        //QCOMPARE(project->relativeParsePath(),"../testing");
        delete project;
    }
    catch (Exception::Base& e)
    {
        qDebug() << e.message();
        QVERIFY(false);
    }
}


void Project::cleanupTestCase(
)
{
    cleanup();
}
}
}
