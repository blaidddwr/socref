#include <QtTest>
#include "Exception.h"
#include "FactoryLanguage.h"
#include "ModelProject.h"
#include "StreamProjectDir.h"
#include "StreamProjectXml.h"
#include "../TestLanguage.h"
#include "../utility.h"

class TestModelProject: public QObject
{
    Q_OBJECT
    Model::Project* _project;
    TestLanguage* _language;
    int _languageIndex;
    int _blockIndex;
private slots:
    void initTestCase();
    void beginAbortSet();
    void beginFinishSet();
    void blockIndex();
    void canMove();
    void canRedo();
    void canUndo();
    void columnCount();
    void constBlock();
    void copyPaste();
    void cutPaste();
    void data();
    void directoryPath();
    void headerData();
    void index();
    void insertRemove();
    void language();
    void modified();
    void move();
    void name();
    void parent();
    void relativeCodePath();
    void rowCount();
    void undoRedo();
    void toDirFromDir();
    void toXmlFromXml();
    void cleanupTestCase();
private:
    bool clearProject();
    bool saveProject(const QString& path);
    bool setBlockProperties(
        int row
        ,const QString& property1
        ,const QString& property2
        ,const QModelIndex& parent = QModelIndex()
        );
};

void TestModelProject::initTestCase()
{
    Q_INIT_RESOURCE(core);
    _language = new TestLanguage(
        new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,QIcon(":/application.svg"))
        );
    Factory::Language::instance()->appendLanguage(_language);
    _languageIndex = Factory::Language::instance()->indexFromName(LANGUAGE_NAME);
    _project = new Model::Project(_languageIndex,this);
    _blockIndex = _language->indexFromName(BLOCK_NAME);
    QVERIFY(_project);
}

void TestModelProject::beginAbortSet()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(!_project->abortSet());
    auto block = qobject_cast<TestBlock*>(_project->beginSet(_project->index(0,0,QModelIndex())));
    QVERIFY(block);
    QVERIFY(_project->abortSet());
}

void TestModelProject::beginFinishSet()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(!_project->finishSet());
    auto block = qobject_cast<TestBlock*>(_project->beginSet(_project->index(0,0,QModelIndex())));
    QVERIFY(block);
    block->property1 = "test";
    QVERIFY(_project->finishSet());
    QCOMPARE(block->property1,"test");
}

void TestModelProject::blockIndex()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->blockIndex(QModelIndex()),0);
    QCOMPARE(_project->blockIndex(_project->index(0,0,QModelIndex())),0);
}

void TestModelProject::canMove()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->canMove(QModelIndex(),0,-1),false);
    QCOMPARE(_project->canMove(QModelIndex(),0,1),true);
    QCOMPARE(_project->canMove(QModelIndex(),1,2),false);
    QCOMPARE(_project->canMove(QModelIndex(),1,0),true);
}

void TestModelProject::canRedo()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->canRedo(),false);
    QVERIFY(_project->undo());
    QCOMPARE(_project->canRedo(),true);
}

void TestModelProject::canUndo()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    int guard = 0;
    while(_project->canUndo())
    {
        _project->undo();
        QVERIFY(++guard < 10000);
    }
    QVERIFY(_project->redo());
    QCOMPARE(_project->canUndo(),true);
}

void TestModelProject::columnCount()
{
    QCOMPARE(_project->columnCount(QModelIndex()),1);
}

void TestModelProject::constBlock()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    auto parentIndex = _project->index(0,0,QModelIndex());
    QVERIFY(parentIndex.isValid());
    auto parentBlock = _project->constBlock(parentIndex);
    QVERIFY(parentBlock);
    QVERIFY(_project->insert(_blockIndex,0,parentIndex));
    QVERIFY(parentBlock->get(0) == _project->constBlock(_project->index(0,0,parentIndex)));
}

void TestModelProject::copyPaste()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(setBlockProperties(0,"test1of1","test2of1"));
    QVERIFY(setBlockProperties(1,"test1of2","test2of2"));
    QCOMPARE(
        _project->copy({_project->index(1,0,QModelIndex()),_project->index(0,0,QModelIndex())})
        ,2
        );
    auto out = new Model::Project(_languageIndex,this);
    QCOMPARE(out->paste(QModelIndex(),0),2);
    auto cb = qobject_cast<const TestBlock*>(out->constBlock(out->index(0,0,QModelIndex())));
    QVERIFY(cb);
    QCOMPARE(cb->property1,"test1of1");
    QCOMPARE(cb->property2,"test2of1");
    cb = qobject_cast<const TestBlock*>(out->constBlock(out->index(1,0,QModelIndex())));
    QVERIFY(cb);
    QCOMPARE(cb->property1,"test1of2");
    QCOMPARE(cb->property2,"test2of2");
    delete out;
}

void TestModelProject::cutPaste()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(setBlockProperties(0,"test1of1","test2of1"));
    QVERIFY(setBlockProperties(1,"test1of2","test2of2"));
    QCOMPARE(
        _project->cut({_project->index(1,0,QModelIndex()),_project->index(0,0,QModelIndex())})
        ,2
        );
    QCOMPARE(_project->rowCount(QModelIndex()),0);
    auto out = new Model::Project(_languageIndex,this);
    QCOMPARE(out->paste(QModelIndex(),0),2);
    auto cb = qobject_cast<const TestBlock*>(out->constBlock(out->index(0,0,QModelIndex())));
    QVERIFY(cb);
    QCOMPARE(cb->property1,"test1of1");
    QCOMPARE(cb->property2,"test2of1");
    cb = qobject_cast<const TestBlock*>(out->constBlock(out->index(1,0,QModelIndex())));
    QVERIFY(cb);
    QCOMPARE(cb->property1,"test1of2");
    QCOMPARE(cb->property2,"test2of2");
    delete out;
}

void TestModelProject::data()
{
    static const QIcon testIcon(":/application.svg");
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    auto index = _project->index(0,0,QModelIndex());
    QVERIFY(index.isValid());
    QCOMPARE(_project->data(index,Qt::DisplayRole),"");
    QSignalSpy spy(_project,&Model::Project::dataChanged);
    QVERIFY(setBlockProperties(0,"test",""));
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),3);
    QCOMPARE(arguments.at(0),index);
    QCOMPARE(arguments.at(1),index);
    QCOMPARE(arguments.at(2).value<QList<int>>(),QList<int>({Qt::DisplayRole}));
    QCOMPARE(_project->data(index,Qt::DisplayRole),"test");
    auto data = _project->data(index,Qt::DecorationRole);
    QVERIFY(data.canConvert<QIcon>());
    QVERIFY(areIconsEqual(data.value<QIcon>(),testIcon));
}

void TestModelProject::directoryPath()
{
    auto path = QDir::temp().absoluteFilePath("socref.project.test");
    QVERIFY(clearProject());
    QVERIFY(saveProject(path));
    QCOMPARE(_project->directoryPath(),path);
}

void TestModelProject::headerData()
{
    static const QIcon testIcon(":/application.svg");
    QCOMPARE(_project->headerData(0,Qt::Horizontal,Qt::DisplayRole),LANGUAGE_LABEL);
    auto data = _project->headerData(0,Qt::Horizontal,Qt::DecorationRole);
    QVERIFY(data.canConvert<QIcon>());
    QVERIFY(areIconsEqual(data.value<QIcon>(),testIcon));
}

void TestModelProject::index()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->index(0,0,QModelIndex()).isValid());
}

void TestModelProject::insertRemove()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->rowCount(QModelIndex()),3);
    QCOMPARE(_project->remove({_project->index(1,0,QModelIndex())}),1);
    QCOMPARE(_project->rowCount(QModelIndex()),2);
    QCOMPARE(
        _project->remove({_project->index(0,0,QModelIndex()),_project->index(1,0,QModelIndex())})
        ,2
        );
    QCOMPARE(_project->rowCount(QModelIndex()),0);
}

void TestModelProject::language()
{
    QCOMPARE(_project->language(),_language);
}

void TestModelProject::modified()
{
    auto path = QDir::temp().absoluteFilePath("socref.project.test");
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->modified(),true);
    QSignalSpy spy(_project,&Model::Project::modifiedChanged);
    QVERIFY(saveProject(path));
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),false);
    QCOMPARE(_project->modified(),false);
}

void TestModelProject::move()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(setBlockProperties(0,"test1",""));
    QVERIFY(setBlockProperties(1,"test2",""));
    QVERIFY(_project->move(QModelIndex(),0,1));
    QCOMPARE(_project->data(_project->index(0,0,QModelIndex()),Qt::DisplayRole).toString(),"test2");
    QCOMPARE(_project->data(_project->index(1,0,QModelIndex()),Qt::DisplayRole).toString(),"test1");
}

void TestModelProject::name()
{
    _project->setName("project1");
    QCOMPARE(_project->name(),"project1");
    QSignalSpy spy(_project,&Model::Project::nameChanged);
    _project->setName("project2");
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),"project2");
    QCOMPARE(_project->name(),"project2");
}

void TestModelProject::parent()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    auto index = _project->index(0,0,QModelIndex());
    QVERIFY(index.isValid());
    QVERIFY(_project->insert(_blockIndex,0,index));
    QCOMPARE(_project->parent(_project->index(0,0,index)),index);
    QCOMPARE(_project->parent(index),QModelIndex());
}

void TestModelProject::relativeCodePath()
{
    QDir dirPath(QDir::temp().absoluteFilePath("socref.project.test"));
    saveProject(dirPath.path());
    _project->setRelativeCodePath("path1");
    QCOMPARE(_project->relativeCodePath(),"path1");
    QSignalSpy spy(_project,&Model::Project::relativeCodePathChanged);
    _project->setRelativeCodePath("path2");
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),"path2");
    QCOMPARE(_project->relativeCodePath(),"path2");
    QCOMPARE(_project->absoluteCodePath(),dirPath.absoluteFilePath("path2"));
}

void TestModelProject::rowCount()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->rowCount(QModelIndex()),1);
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->rowCount(QModelIndex()),2);
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QCOMPARE(_project->rowCount(QModelIndex()),3);
}

void TestModelProject::undoRedo()
{
    QVERIFY(clearProject());
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->insert(_blockIndex,0,QModelIndex()));
    QVERIFY(_project->undo());
    QCOMPARE(_project->rowCount(QModelIndex()),2);
    QVERIFY(_project->undo());
    QCOMPARE(_project->rowCount(QModelIndex()),1);
    QVERIFY(_project->undo());
    QCOMPARE(_project->rowCount(QModelIndex()),0);
    QVERIFY(_project->redo());
    QCOMPARE(_project->rowCount(QModelIndex()),1);
    QVERIFY(_project->redo());
    QCOMPARE(_project->rowCount(QModelIndex()),2);
    QVERIFY(_project->redo());
    QCOMPARE(_project->rowCount(QModelIndex()),3);
}

void TestModelProject::toDirFromDir()
{
    using Stream = Stream::ProjectDir;
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test");
    QVERIFY(clearProject());
    _project->setName("Test Project Name");
    _project->setRelativeCodePath("../test/path");
    _project->insert(_blockIndex,0,QModelIndex());
    _project->insert(_blockIndex,0,QModelIndex());
    _project->insert(_blockIndex,0,QModelIndex());
    QCOMPARE(_project->modified(),true);
    QVERIFY(saveProject(path));
    QCOMPARE(_project->modified(),false);
    QCOMPARE(_project->directoryPath(),path);
    ::Model::Project* in = nullptr;
    try
    {
        in = Stream(path).load(this);
    }
    catch (Exception::ReadProject& e)
    {
        qDebug() << tr("Read Project Exception: %1").arg(e.message());
    }
    catch (Exception::ReadBlock& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::FileSystem& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QVERIFY(in);
    QCOMPARE(in->name(),_project->name());
    QCOMPARE(in->relativeCodePath(),_project->relativeCodePath());
    QCOMPARE(in->directoryPath(),_project->directoryPath());
    QCOMPARE(in->modified(),false);
    QCOMPARE(in->rowCount(QModelIndex()),_project->rowCount(QModelIndex()));
    delete in;
}

void TestModelProject::toXmlFromXml()
{
    using Stream = Stream::ProjectXml;
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test.xml");
    QVERIFY(clearProject());
    _project->setName("Test Project Name");
    _project->setRelativeCodePath("../test/path");
    _project->insert(_blockIndex,0,QModelIndex());
    _project->insert(_blockIndex,0,QModelIndex());
    _project->insert(_blockIndex,0,QModelIndex());
    QCOMPARE(_project->modified(),true);
    try
    {
        Stream(path) << *_project;
    }
    catch (Exception::WriteProject& e)
    {
        qDebug() << tr("Write Project Exception: %1").arg(e.message());
    }
    catch (Exception::WriteBlock& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    catch (Exception::FileSystem& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QCOMPARE(_project->modified(),true);
    QVERIFY(_project->directoryPath() != path);
    Model::Project* in = nullptr;
    try
    {
        in = Stream(path).load(this);
    }
    catch (Exception::ReadProject& e)
    {
        qDebug() << tr("Read Project Exception: %1").arg(e.message());
    }
    catch (Exception::ReadBlock& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::FileSystem& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QVERIFY(in);
    QCOMPARE(in->name(),_project->name());
    QCOMPARE(in->relativeCodePath(),_project->relativeCodePath());
    QCOMPARE(in->directoryPath(),"");
    QCOMPARE(in->modified(),false);
    QCOMPARE(in->rowCount(QModelIndex()),_project->rowCount(QModelIndex()));
    delete in;
}

void TestModelProject::cleanupTestCase()
{
    delete _project;
}

bool TestModelProject::clearProject()
{
    auto size = _project->rowCount(QModelIndex());
    QModelIndexList list;
    for (int i = 0;i < size;i++)
    {
        list.append(_project->index(i,0,QModelIndex()));
    }
    return (
        _project->remove(list) == size
        && _project->rowCount(QModelIndex()) == 0
        );
}

bool TestModelProject::saveProject(const QString& path)
{
    using Stream = Stream::ProjectDir;
    try
    {
        Stream(path) << *_project;
    }
    catch (Exception::WriteProject& e)
    {
        qDebug() << tr("Write Project Exception: %1").arg(e.message());
        return false;
    }
    catch (Exception::WriteBlock& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
        return false;
    }
    catch (Exception::FileSystem& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
        return false;
    }
    return true;
}

bool TestModelProject::setBlockProperties(
    int row
    ,const QString& property1
    ,const QString& property2
    ,const QModelIndex& parent
    )
{
    auto index = _project->index(row,0,parent);
    if (!index.isValid())
    {
        return false;
    }
    auto block = qobject_cast<TestBlock*>(_project->beginSet(index));
    if (!block)
    {
        return false;
    }
    block->property1 = property1;
    block->property2 = property2;
    return _project->finishSet();
}

QTEST_MAIN(TestModelProject)
#include "test.moc"
