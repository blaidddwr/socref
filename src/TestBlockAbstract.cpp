#include "TestBlockAbstract.h"
#include <QtTest>
#include "BlockTestNode.h"
#include "Global.h"
#include "LanguageTest.h"
#include "ModelMetaLanguage.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
using NodeBlock = ::Block::Test::Node;
using namespace ::Block::Test;


void Abstract::initTestCase(
)
{
    _meta = new Model::Meta::Language("test","Test",this);
    initLanguage(new Language::Test(_meta,this));
    _block = create<NodeBlock>(NodeIndex);
    QVERIFY(createTestDir());
}


void Abstract::append(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child = create<NodeBlock>(NodeIndex);
    QVERIFY(child);
    _block->append(child);
    QCOMPARE(qobject_cast<NodeBlock*>(child->parent()),_block);
    QCOMPARE(_block->size(),1);
    QCOMPARE(_block->get(0),child);
    QCOMPARE(child->addCount(),1);
    _block->take(0);
    QCOMPARE(child->removeCount(),1);
    QCOMPARE(_block->size(),0);
    _block->append(child);
    QCOMPARE(child->addCount(),2);
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
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
    _block->append(child0);
    child0->append(child1);
    child1->append(child2);
    auto descendants = _block->descendants();
    QCOMPARE(descendants.size(),3);
    QVERIFY(descendants.contains(child0));
    QVERIFY(descendants.contains(child1));
    QVERIFY(descendants.contains(child2));
}


void Abstract::displayIconProperty(
)
{
    QImage img(10,10,QImage::Format_ARGB32);
    img.fill(Qt::red);
    QIcon testIcon(QPixmap::fromImage(img));
    QVERIFY(_block->displayIcon().isNull());
    QSignalSpy spy(_block,&::Block::Abstract::displayIconChanged);
    _block->setIcon(testIcon);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QVERIFY(areIconsEqual(arguments.at(0).value<QIcon>(),testIcon));
    QVERIFY(areIconsEqual(_block->displayIcon(),testIcon));
}


void Abstract::displayTextProperty(
)
{
    static const QString testName = "Testing123";
    QSignalSpy spy(_block,&::Block::Abstract::displayTextChanged);
    _block->setName(testName);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testName);
    QCOMPARE(_block->displayText(),testName);
}


void Abstract::fromDir(
)
{
    auto root = qobject_cast<Node*>(
        ::Block::Abstract::fromDir(language(),Socref_1_0,testProjDir(),this)
    );
    QVERIFY(root);
    QCOMPARE(root->name(),"");
    QCOMPARE(root->size(),1);
    auto child = qobject_cast<Node*>(root->get(0));
    QVERIFY(child);
    QCOMPARE(child->name(),"Node\n0");
    QCOMPARE(child->size(),1);
    child = qobject_cast<Node*>(child->get(0));
    QVERIFY(child);
    QCOMPARE(child->name(),"Node\n1");
    QCOMPARE(child->size(),0);
    delete root;
}


void Abstract::fromXml(
)
{
    QFile file(testXml());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
        xml.readNext();
        if (
            xml.isStartElement()
            && xml.name().toString() == "node"
        )
        {
            auto root = qobject_cast<Node*>(
                ::Block::Abstract::fromXml(language(),Socref_1_0,xml,this)
            );
            QVERIFY(root);
            QCOMPARE(root->name(),"");
            QCOMPARE(root->size(),1);
            auto child = qobject_cast<Node*>(root->get(0));
            QVERIFY(child);
            QCOMPARE(child->name(),"Node0");
            QCOMPARE(child->size(),1);
            child = qobject_cast<Node*>(child->get(0));
            QVERIFY(child);
            QCOMPARE(child->name(),"Node1");
            QCOMPARE(child->size(),0);
            delete root;
            return;
        }
    }
    QVERIFY(false);
}


void Abstract::fromXmlLegacy(
)
{
    QFile file(testXmlLegacy());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
        xml.readNext();
        if (
            xml.isStartElement()
            && xml.name().toString().toLower() == "node"
        )
        {
            auto root = qobject_cast<Node*>(
                ::Block::Abstract::fromXml(language(),Socref_Legacy,xml,this)
            );
            QVERIFY(root);
            QCOMPARE(root->name(),"");
            QCOMPARE(root->size(),1);
            auto child = qobject_cast<Node*>(root->get(0));
            QVERIFY(child);
            QCOMPARE(child->name(),"Node0");
            QCOMPARE(child->size(),1);
            child = qobject_cast<Node*>(child->get(0));
            QVERIFY(child);
            QCOMPARE(child->name(),"Node1");
            QCOMPARE(child->size(),0);
            delete root;
            return;
        }
    }
    QVERIFY(false);
}


void Abstract::get(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
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
}


void Abstract::indexOf(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
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
}


void Abstract::insert(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
    _block->insert(0,child1);
    QCOMPARE(_block->get(0),child1);
    QCOMPARE(qobject_cast<Node*>(child1->parent()),_block);
    QCOMPARE(child1->addCount(),1);
    _block->insert(0,child0);
    QCOMPARE(qobject_cast<Node*>(child0->parent()),_block);
    QCOMPARE(child0->addCount(),1);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    _block->insert(2,child2);
    QCOMPARE(qobject_cast<Node*>(child2->parent()),_block);
    QCOMPARE(child2->addCount(),1);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child1);
    QCOMPARE(_block->get(2),child2);
}


void Abstract::metaProperty(
)
{
    QCOMPARE(_block->meta(),language()->blockMeta(NodeIndex));
}


void Abstract::move(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    _block->move(0,1);
    QCOMPARE(_block->get(0),child1);
    QCOMPARE(_block->get(1),child0);
    QCOMPARE(_block->get(2),child2);
    _block->move(2,0);
    QCOMPARE(_block->get(0),child2);
    QCOMPARE(_block->get(1),child1);
    QCOMPARE(_block->get(2),child0);
    _block->move(2,1);
    QCOMPARE(_block->get(0),child2);
    QCOMPARE(_block->get(1),child0);
    QCOMPARE(_block->get(2),child1);
}


void Abstract::scopeProperty(
)
{
    static const QString testName = "Testing123";
    auto child = create<NodeBlock>(NodeIndex);
    QVERIFY(child);
    child->setName(testName);
    _block->append(child);
    QCOMPARE(child->scope(),testName);
}


void Abstract::size(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->append(create<::Block::Abstract>(NodeIndex));
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
    auto child0 = create<NodeBlock>(NodeIndex);
    auto child1 = create<NodeBlock>(NodeIndex);
    auto child2 = create<NodeBlock>(NodeIndex);
    _block->append(child0);
    _block->append(child1);
    _block->append(child2);
    auto orphan = qobject_cast<Node*>(_block->take(1));
    QCOMPARE(orphan,child1);
    QCOMPARE(orphan->removeCount(),1);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),2);
    QCOMPARE(_block->get(0),child0);
    QCOMPARE(_block->get(1),child2);
    orphan = qobject_cast<Node*>(_block->take(0));
    QCOMPARE(orphan,child0);
    QCOMPARE(orphan->removeCount(),1);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),1);
    QCOMPARE(_block->get(0),child2);
    orphan = qobject_cast<Node*>(_block->take(0));
    QCOMPARE(orphan,child2);
    QCOMPARE(orphan->removeCount(),1);
    QCOMPARE(orphan->parent(),nullptr);
    QCOMPARE(_block->size(),0);
}


void Abstract::toDir(
)
{
    QDir dir(testDir());
    QVERIFY(dir.exists());
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->setName("");
    auto node0 = create<NodeBlock>(NodeIndex);
    auto node1 = create<NodeBlock>(NodeIndex);
    node0->setName("Node\n0");
    node1->setName("Node\n1");
    _block->append(node0);
    node0->append(node1);
    _block->toDir(dir.absoluteFilePath("outputDir"));
    QVERIFY(
        areDirsEqual(
            QDir(testProjDir())
            ,QDir(dir.absoluteFilePath("outputDir"))
            ,"*.srb"
        )
    );
}


void Abstract::toXml(
)
{
    QDir dir(testDir());
    QVERIFY(dir.exists());
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->setName("");
    auto node0 = create<NodeBlock>(NodeIndex);
    auto node1 = create<NodeBlock>(NodeIndex);
    node0->setName("Node0");
    node1->setName("Node1");
    _block->append(node0);
    node0->append(node1);
    QFile file(dir.absoluteFilePath("output.xml"));
    QVERIFY(file.open(QIODevice::WriteOnly|QIODevice::Truncate));
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    writeStartProjectXml(xml);
    _block->toXml(xml);
    writeEndProjectXml(xml);
    xml.writeEndDocument();
    QVERIFY(file.flush());
    QVERIFY(areXmlFilesEqual(testXml(),dir.absoluteFilePath("output.xml")));
}


void Abstract::cleanupTestCase(
)
{
    cleanup();
    delete _block;
    delete _meta;
}
}
}
