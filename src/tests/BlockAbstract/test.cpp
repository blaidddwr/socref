#include <QtTest>
#include "../TestLanguage.h"
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockRead.h"
#include "ExceptionBlockWrite.h"
#include "ExceptionSystemFile.h"
#include "Global.h"
#include "ModelMeta.h"
#include "StreamBlockDir.h"
#include "StreamBlockXml.h"

class TestAbstractBlock: public QObject
{
    Q_OBJECT
    Block::Abstract* _block {nullptr};
    Language::Abstract* _language {nullptr};
    Model::Meta::Block* _blockMeta {nullptr};
    Model::Meta::Language* _languageMeta {nullptr};
private slots:
    void initTestCase();
    void append();
    void descendants();
    void get();
    void indexOf();
    void insert();
    void metaProperty();
    void move();
    void size();
    void take();
    void toDirFromDir();
    void toXmlFromXml();
    void cleanupTestCase();
};

void TestAbstractBlock::initTestCase()
{
    _languageMeta = new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,QIcon(),this);
    _language = new TestLanguage(_languageMeta,this);
    _blockMeta = new Model::Meta::Block(
        _languageMeta
        ,BLOCK_INDEX
        ,BLOCK_NAME
        ,BLOCK_LABEL
        ,QIcon()
        ,{BLOCK_INDEX}
        ,this
    );
    _block = qobject_cast<TestBlock*>(_language->createBlock(BLOCK_INDEX,this));
    QVERIFY(_block);
}

void TestAbstractBlock::append()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child = new TestBlock(_blockMeta,this);
    QVERIFY(child);
    _block->append(child);
    QCOMPARE(qobject_cast<TestBlock*>(child->parent()),_block);
    QCOMPARE(_block->size(),1);
    QCOMPARE(_block->get(0),child);
    QCOMPARE(child->addCount,1);
    QCOMPARE(child->lastAddIndex,0);
    _block->take(0);
    QCOMPARE(child->removeCount,1);
    QCOMPARE(child->lastRemoveIndex,0);
    QCOMPARE(_block->size(),0);
    _block->append(child);
    QCOMPARE(child->addCount,2);
    QCOMPARE(child->lastAddIndex,0);
    QCOMPARE(_block->size(),1);
    delete child;
    QCOMPARE(_block->size(),0);
}

void TestAbstractBlock::descendants()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->append(child0);
    child0->append(child1);
    child1->append(child2);
    auto descendants = _block->descendants();
    QCOMPARE(descendants.size(),3);
    QVERIFY(descendants.contains(child0));
    QVERIFY(descendants.contains(child1));
    QVERIFY(descendants.contains(child2));
}

void TestAbstractBlock::get()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    QCOMPARE(_block->size(),3);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    QCOMPARE(_block->get(2),child2);
    delete child1;
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child2);
    delete child0;
    QCOMPARE(_block->get(0),child2);
    delete child2;
}

void TestAbstractBlock::indexOf()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    QCOMPARE(_block->size(),3);
    QCOMPARE(_block->indexOf(child0),0);
    QCOMPARE(_block->indexOf(child1),1);
    QCOMPARE(_block->indexOf(child2),2);
    delete child1;
    QCOMPARE(_block->indexOf(child0),0);
    QCOMPARE(_block->indexOf(child2),1);
    delete child0;
    QCOMPARE(_block->indexOf(child2),0);
    delete child2;
}

void TestAbstractBlock::insert()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->insert(0,child1);
    QCOMPARE(_block->get(0),child1);
    QCOMPARE(qobject_cast<TestBlock*>(child1->parent()),_block);
    QCOMPARE(child1->addCount,1);
    QCOMPARE(child1->lastAddIndex,0);
    _block->insert(0,child0);
    QCOMPARE(qobject_cast<TestBlock*>(child0->parent()),_block);
    QCOMPARE(child0->addCount,1);
    QCOMPARE(child0->lastAddIndex,0);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    _block->insert(2,child2);
    QCOMPARE(qobject_cast<TestBlock*>(child2->parent()),_block);
    QCOMPARE(child2->addCount,1);
    QCOMPARE(child2->lastAddIndex,2);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    QCOMPARE(_block->get(2),child2);
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
}

void TestAbstractBlock::metaProperty()
{
    QCOMPARE(_block->meta(),_language->blockMeta(BLOCK_INDEX));
}

void TestAbstractBlock::move()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    _block->move(0,1);
    QCOMPARE(child0->moveCount,1);
    QCOMPARE(child0->lastFromMoveIndex,0);
    QCOMPARE(child0->lastToMoveIndex,1);
    QCOMPARE(_block->get(0),child1);
    QCOMPARE(_block->get(1),child0);
    QCOMPARE(_block->get(2),child2);
    _block->move(2,0);
    QCOMPARE(child2->moveCount,1);
    QCOMPARE(child2->lastFromMoveIndex,2);
    QCOMPARE(child2->lastToMoveIndex,0);
    QCOMPARE(_block->get(0),child2);
    QCOMPARE(_block->get(1),child1);
    QCOMPARE(_block->get(2),child0);
    _block->move(2,1);
    QCOMPARE(child0->moveCount,2);
    QCOMPARE(child0->lastFromMoveIndex,2);
    QCOMPARE(child0->lastToMoveIndex,1);
    QCOMPARE(_block->get(0),child2);
    QCOMPARE(_block->get(1),child0);
    QCOMPARE(_block->get(2),child1);
}

void TestAbstractBlock::size()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->append(new TestBlock(_blockMeta));
    QCOMPARE(_block->size(),1);
    delete _block->take(0);
    QCOMPARE(_block->size(),0);
}

void TestAbstractBlock::take()
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new TestBlock(_blockMeta,this);
    auto child1 = new TestBlock(_blockMeta,this);
    auto child2 = new TestBlock(_blockMeta,this);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    auto orphan = qobject_cast<TestBlock*>(_block->take(1));
    QCOMPARE(orphan,child1);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,1);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),2);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child2);
    orphan = qobject_cast<TestBlock*>(_block->take(0));
    QCOMPARE(orphan,child0);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,0);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),1);
    QCOMPARE(_block->get(0),child2);
    orphan = qobject_cast<TestBlock*>(_block->take(0));
    QCOMPARE(orphan,child2);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,0);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),0);
}

void TestAbstractBlock::toDirFromDir()
{
    using Stream = Stream::BlockDir;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.toDirFromDir.test");
    auto outRoot = qobject_cast<TestBlock*>(_block);
    outRoot->property1 = "Testing Root Property 1";
    outRoot->property2 = "Testing\n Root Property\n 2";
    auto out0 = new TestBlock(_blockMeta,this);
    auto out1 = new TestBlock(_blockMeta,this);
    out0->property1 = "Testing in0 Property 1";
    out0->property2 = "Testing\n in0 Property\n 2";
    out1->property1 = "Testing in1 Property 1";
    out1->property2 = "Testing\n in1 Property\n 2";
    _block->append(out0);
    out0->append(out1);
    try
    {
        Stream(path) << *_block;
    }
    catch (Exception::Block::Write& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    catch (Exception::Block::Logical& e)
    {
        qDebug() << tr("Logical Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    TestBlock* inRoot = nullptr;
    try
    {
        inRoot = qobject_cast<TestBlock*>(Stream(path,_language,Socref_Current).load(this));
    }
    catch (Exception::Block::Read& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QVERIFY(inRoot);
    QCOMPARE(inRoot->property1,outRoot->property1);
    QCOMPARE(inRoot->property2,outRoot->property2);
    QCOMPARE(inRoot->size(),1);
    auto in0 = qobject_cast<TestBlock*>(inRoot->get(0));
    QVERIFY(in0);
    QCOMPARE(in0->property1,out0->property1);
    QCOMPARE(in0->property2,out0->property2);
    auto in1 = qobject_cast<TestBlock*>(in0->get(0));
    QVERIFY(in1);
    QCOMPARE(in1->property1,out1->property1);
    QCOMPARE(in1->property2,out1->property2);
    delete inRoot;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
}

void TestAbstractBlock::toXmlFromXml()
{
    using Stream = Stream::BlockXml;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.toXmlFromXml.test.xml");
    auto outRoot = qobject_cast<TestBlock*>(_block);
    outRoot->property1 = "Testing Root Property 1";
    outRoot->property2 = "Testing\n Root Property\n 2";
    auto out0 = new TestBlock(_blockMeta,this);
    auto out1 = new TestBlock(_blockMeta,this);
    out0->property1 = "Testing in0 Property 1";
    out0->property2 = "Testing\n in0 Property\n 2";
    out1->property1 = "Testing in1 Property 1";
    out1->property2 = "Testing\n in1 Property\n 2";
    _block->append(out0);
    out0->append(out1);
    try
    {
        QFile file(path);
        QVERIFY(file.open(QIODevice::WriteOnly|QIODevice::Truncate));
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        Stream(xml) << *_block;
        xml.writeEndDocument();
    }
    catch (Exception::Block::Write& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    TestBlock* inRoot = nullptr;
    try
    {
        QFile file(path);
        QVERIFY(file.open(QIODevice::ReadOnly));
        QXmlStreamReader xml(&file);
        while (
            !xml.atEnd()
            && !xml.isStartElement()
        )
        {
            xml.readNext();
        }
        inRoot = qobject_cast<TestBlock*>(Stream(xml,_language,Socref_Current).load(this));
    }
    catch (Exception::Block::Read& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QVERIFY(inRoot);
    QCOMPARE(inRoot->property1,outRoot->property1);
    QCOMPARE(inRoot->property2,outRoot->property2);
    QCOMPARE(inRoot->size(),1);
    auto in0 = qobject_cast<TestBlock*>(inRoot->get(0));
    QVERIFY(in0);
    QCOMPARE(in0->property1,out0->property1);
    QCOMPARE(in0->property2,out0->property2);
    auto in1 = qobject_cast<TestBlock*>(in0->get(0));
    QVERIFY(in1);
    QCOMPARE(in1->property1,out1->property1);
    QCOMPARE(in1->property2,out1->property2);
    delete inRoot;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
}

void TestAbstractBlock::cleanupTestCase()
{
    delete _block;
    delete _blockMeta;
    delete _language;
    delete _languageMeta;
}

QTEST_MAIN(TestAbstractBlock)
#include "test.moc"
