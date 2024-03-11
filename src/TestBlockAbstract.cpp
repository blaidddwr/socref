#include "TestBlockAbstract.h"
#include <QtTest>
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockRead.h"
#include "ExceptionBlockWrite.h"
#include "ExceptionSystemFile.h"
#include "Global.h"
#include "StreamBlock.h"
#include "TestDummyLanguage.h"
namespace Test {
namespace Block {
using DummyBlock = Dummy::Block;
using DummyLanguage = Dummy::Language;


void Abstract::initTestCase(
)
{
    _languageMeta = new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,this);
    _language = new DummyLanguage(_languageMeta,this);
    _blockMeta = new Model::Meta::Block(
        _languageMeta
        ,BLOCK_INDEX
        ,BLOCK_NAME
        ,BLOCK_LABEL
        ,QIcon()
        ,{BLOCK_INDEX}
        ,this
    );
    _block = qobject_cast<DummyBlock*>(_language->create(BLOCK_INDEX,this));
    QVERIFY(_block);
}


void Abstract::append(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child = new DummyBlock(_blockMeta,this);
    QVERIFY(child);
    _block->append(child);
    QCOMPARE(qobject_cast<DummyBlock*>(child->parent()),_block);
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


void Abstract::descendants(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
    _block->append(child0);
    child0->append(child1);
    child1->append(child2);
    auto descendants = _block->descendants();
    QCOMPARE(descendants.size(),3);
    QVERIFY(descendants.contains(child0));
    QVERIFY(descendants.contains(child1));
    QVERIFY(descendants.contains(child2));
}


void Abstract::get(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
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


void Abstract::indexOf(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
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


void Abstract::insert(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
    _block->insert(0,child1);
    QCOMPARE(_block->get(0),child1);
    QCOMPARE(qobject_cast<DummyBlock*>(child1->parent()),_block);
    QCOMPARE(child1->addCount,1);
    QCOMPARE(child1->lastAddIndex,0);
    _block->insert(0,child0);
    QCOMPARE(qobject_cast<DummyBlock*>(child0->parent()),_block);
    QCOMPARE(child0->addCount,1);
    QCOMPARE(child0->lastAddIndex,0);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    _block->insert(2,child2);
    QCOMPARE(qobject_cast<DummyBlock*>(child2->parent()),_block);
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


void Abstract::metaProperty(
)
{
    QCOMPARE(_block->meta(),_language->blockMeta(BLOCK_INDEX));
}


void Abstract::move(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
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


void Abstract::size(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->append(new DummyBlock(_blockMeta));
    QCOMPARE(_block->size(),1);
    delete _block->take(0);
    QCOMPARE(_block->size(),0);
}


void Abstract::take(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = new DummyBlock(_blockMeta,this);
    auto child1 = new DummyBlock(_blockMeta,this);
    auto child2 = new DummyBlock(_blockMeta,this);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    auto orphan = qobject_cast<DummyBlock*>(_block->take(1));
    QCOMPARE(orphan,child1);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,1);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),2);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child2);
    orphan = qobject_cast<DummyBlock*>(_block->take(0));
    QCOMPARE(orphan,child0);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,0);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),1);
    QCOMPARE(_block->get(0),child2);
    orphan = qobject_cast<DummyBlock*>(_block->take(0));
    QCOMPARE(orphan,child2);
    QCOMPARE(orphan->removeCount,1);
    QCOMPARE(orphan->lastRemoveIndex,0);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),0);
}


void Abstract::toDirFromDir(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.toDirFromDir.test");
    auto outRoot = qobject_cast<DummyBlock*>(_block);
    outRoot->property1 = "Testing Root Property 1";
    outRoot->property2 = "Testing\n Root Property\n 2";
    auto out0 = new DummyBlock(_blockMeta,this);
    auto out1 = new DummyBlock(_blockMeta,this);
    out0->property1 = "Testing in0 Property 1";
    out0->property2 = "Testing\n in0 Property\n 2";
    out1->property1 = "Testing in1 Property 1";
    out1->property2 = "Testing\n in1 Property\n 2";
    _block->append(out0);
    out0->append(out1);
    try
    {
        Stream::Block::toDir(*_block,path);
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
    DummyBlock* inRoot = nullptr;
    try
    {
        inRoot = qobject_cast<DummyBlock*>(Stream::Block::fromDir(_language,Socref_1_0,path,this));
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
    auto in0 = qobject_cast<DummyBlock*>(inRoot->get(0));
    QVERIFY(in0);
    QCOMPARE(in0->property1,out0->property1);
    QCOMPARE(in0->property2,out0->property2);
    auto in1 = qobject_cast<DummyBlock*>(in0->get(0));
    QVERIFY(in1);
    QCOMPARE(in1->property1,out1->property1);
    QCOMPARE(in1->property2,out1->property2);
    delete inRoot;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
}


void Abstract::toXmlFromXml(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.toXmlFromXml.test.xml");
    auto outRoot = qobject_cast<DummyBlock*>(_block);
    outRoot->property1 = "Testing Root Property 1";
    outRoot->property2 = "Testing\n Root Property\n 2";
    auto out0 = new DummyBlock(_blockMeta,this);
    auto out1 = new DummyBlock(_blockMeta,this);
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
        Stream::Block::toXml(*_block,xml);
        xml.writeEndDocument();
    }
    catch (Exception::Block::Write& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    DummyBlock* inRoot = nullptr;
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
        inRoot = qobject_cast<DummyBlock*>(Stream::Block::fromXml(_language,Socref_1_0,xml,this));
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
    auto in0 = qobject_cast<DummyBlock*>(inRoot->get(0));
    QVERIFY(in0);
    QCOMPARE(in0->property1,out0->property1);
    QCOMPARE(in0->property2,out0->property2);
    auto in1 = qobject_cast<DummyBlock*>(in0->get(0));
    QVERIFY(in1);
    QCOMPARE(in1->property1,out1->property1);
    QCOMPARE(in1->property2,out1->property2);
    delete inRoot;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
}


void Abstract::cleanupTestCase(
)
{
    delete _block;
    delete _blockMeta;
    delete _language;
    delete _languageMeta;
}
}
}
