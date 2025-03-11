#include <QtTest>
#include "ControllerCode.h"
#include "FactoryLanguage.h"
#include "ModelProject.h"
#include "../TestLanguage.h"

class TestControllerCode: public QObject
{
    Q_OBJECT
    Controller::Code* _code;
    Model::Project* _project;
private slots:
    void initTestCase();
    void clear();
    void size();
    void cleanupTestCase();
};

void TestControllerCode::initTestCase()
{
    Factory::Language::instance()->appendLanguage(
        new TestLanguage(new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,QIcon()))
        );
    _project = new Model::Project(0,this);
    _project->insert(0,0,QModelIndex());
    _project->insert(0,0,QModelIndex());
    _project->insert(0,0,QModelIndex());
    auto child = qobject_cast<TestBlock*>(_project->beginSet(_project->index(0,0,QModelIndex())));
    QVERIFY(child);
    child->property1 = "child0";
    QVERIFY(_project->finishSet());
    child = qobject_cast<TestBlock*>(_project->beginSet(_project->index(1,0,QModelIndex())));
    QVERIFY(child);
    child->property1 = "child1";
    QVERIFY(_project->finishSet());
    child = qobject_cast<TestBlock*>(_project->beginSet(_project->index(2,0,QModelIndex())));
    QVERIFY(child);
    child->property1 = "child2";
    QVERIFY(_project->finishSet());
    _code = new Controller::Code(_project,this);
}

void TestControllerCode::clear()
{
    auto child = _project->beginSet(_project->index(0,0,QModelIndex()));
    QVERIFY(child);
    child->code().insert("test",{"test"});
    child->touchCode();
    QVERIFY(_project->finishSet());
    _code->clear();
    QVERIFY(child->code().isEmpty());
}

void TestControllerCode::size()
{
    QCOMPARE(_code->size(),4);
}

void TestControllerCode::cleanupTestCase()
{
    delete _code;
    delete _project;
}

QTEST_MAIN(TestControllerCode)
#include "test.moc"
