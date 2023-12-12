#include "TestBlockAbstract.h"
#include <QSignalSpy>
#include <QtTest>
#include "BlockTestNode.h"
#include "Exception.h"
#include "Global.h"
#include "LanguageTest.h"
#include "ModelMetaLanguage.h"
#include "Test.h"
#define TEST_DIR "socref.unit_test.block.d"
#define TEST_PROJ_DIR "testDir"
#define TEST_XML "test.xml"
#define TEST_XML_LEGACY "test.legacy.xml"
namespace Test {
namespace Block {


void Abstract::initTestCase(
)
{
    _meta = new Model::Meta::Language("test","Test",this);
    _language = new Language::Test(_meta,this);
    _block = create();
    QVERIFY(_block);
    auto dir = QDir::temp();
    if (dir.exists(TEST_DIR))
    {
        QProcess::execute("rm",{"-fr",dir.absoluteFilePath(TEST_DIR)});
    }
    QVERIFY(dir.mkdir(TEST_DIR));
    createTestXml();
    createTestXmlLegacy();
    createTestProjectDir();
}


void Abstract::append(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child = create();
    _block->append(child);
    QCOMPARE(qobject_cast<::Block::Test::Node*>(child->parent()),_block);
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
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
    using namespace ::Block::Test;
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    auto root = qobject_cast<Node*>(
        ::Block::Abstract::fromDir(_language,Socref_1_0,dir.absoluteFilePath(TEST_PROJ_DIR),this)
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
    using namespace ::Block::Test;
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    QFile file(dir.absoluteFilePath(TEST_XML));
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
                ::Block::Abstract::fromXml(_language,Socref_1_0,xml,this)
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
    using namespace ::Block::Test;
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    QFile file(dir.absoluteFilePath(TEST_XML_LEGACY));
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
                ::Block::Abstract::fromXml(_language,Socref_Legacy,xml,this)
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
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
    using namespace ::Block::Test;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
    QCOMPARE(_block->meta(),_language->blockMeta(::Block::Test::NodeIndex));
}


void Abstract::move(
)
{
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
}


void Abstract::scopeProperty(
)
{
    static const QString testName = "Testing123";
    auto child = create();
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
    _block->append(create());
    QCOMPARE(_block->size(),1);
    delete _block->take(0);
    QCOMPARE(_block->size(),0);
}


void Abstract::take(
)
{
    using namespace ::Block::Test;
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    auto child0 = create();
    auto child1 = create();
    auto child2 = create();
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
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->setName("");
    auto node0 = create();
    auto node1 = create();
    node0->setName("Node\n0");
    node1->setName("Node\n1");
    _block->append(node0);
    node0->append(node1);
    _block->toDir(dir.absoluteFilePath("outputDir"));
    QVERIFY(
        areDirsEqual(
            QDir(dir.absoluteFilePath(TEST_PROJ_DIR))
            ,QDir(dir.absoluteFilePath("outputDir"))
        )
    );
}


void Abstract::toXml(
)
{
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    while (_block->size() > 0)
    {
        delete _block->take(0);
    }
    _block->setName("");
    auto node0 = create();
    auto node1 = create();
    node0->setName("Node0");
    node1->setName("Node1");
    _block->append(node0);
    node0->append(node1);
    QFile file(dir.absoluteFilePath("output.xml"));
    QVERIFY(file.open(QIODevice::WriteOnly|QIODevice::Truncate));
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    _block->toXml(xml);
    xml.writeEndDocument();
    QVERIFY(file.flush());
    QVERIFY(
        areXmlFilesEqual(dir.absoluteFilePath(TEST_XML),dir.absoluteFilePath("output.xml"))
    );
}


void Abstract::cleanupTestCase(
)
{
    QProcess::execute("rm",{"-fr",QDir::temp().absoluteFilePath(TEST_DIR)});
    delete _block;
    delete _language;
    delete _meta;
}


bool Abstract::areDirsEqual(
    const QDir& dir0
    ,const QDir& dir1
)
{
    for (const auto& info: dir0.entryInfoList(QDir::Files))
    {
        if (!dir1.exists(info.fileName()))
        {
            return false;
        }
        if (!areFilesEqual(info.absoluteFilePath(),dir1.absoluteFilePath(info.fileName())))
        {
            return false;
        }
    }
    return true;
}


bool Abstract::areFilesEqual(
    const QString& path0
    ,const QString& path1
)
{
    QFile file0(path0);
    QFile file1(path1);
    if (!file0.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!file1.open(QIODevice::ReadOnly))
    {
        return false;
    }
    return file0.readAll() == file1.readAll();
}


bool Abstract::areXmlFilesEqual(
    const QString& path0
    ,const QString& path1
)
{
    QFile file0(path0);
    QFile file1(path1);
    if (!file0.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!file1.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QXmlStreamReader xml0(&file0);
    QXmlStreamReader xml1(&file1);
    while (!xml0.atEnd())
    {
        if (xml0.tokenType() != xml1.tokenType())
        {
            return false;
        }
        if (xml0.name() != xml1.name())
        {
            return false;
        }
        if (xml0.attributes() != xml1.attributes())
        {
            return false;
        }
        if (xml0.text() != xml1.text())
        {
            return false;
        }
        xml0.readNext();
        xml1.readNext();
    }
    return true;
}


::Block::Test::Node* Abstract::create(
)
{
    ASSERT(_language);
    auto ret = qobject_cast<::Block::Test::Node*>(_language->create(::Block::Test::NodeIndex,this));
    ASSERT(ret);
    return ret;
}


bool Abstract::createBlockFile(
    const QString& path
    ,const QString& name
    ,const QStringList links
)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return false;
    }
    QTextStream out(&file);
    out << "node\n";
    out << ":name\n" << name << "\n";
    out << "+CHILDREN+\n";
    for (const auto& link: links)
    {
        out << link << "\n";
    }
    return (file.error() == QFileDevice::NoError);
}


void Abstract::createTestProjectDir(
)
{
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    QVERIFY(dir.mkdir(TEST_PROJ_DIR));
    QVERIFY(dir.cd(TEST_PROJ_DIR));
    QVERIFY(createBlockFile(dir.absoluteFilePath("ROOT.srb"),"",{"Node0"}));
    QVERIFY(createBlockFile(dir.absoluteFilePath("Node0.srb"),"Node\\n0",{"Node0:Node1"}));
    QVERIFY(createBlockFile(dir.absoluteFilePath("Node0:Node1.srb"),"Node\\n1",{}));
}


void Abstract::createTestXml(
)
{
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    QFile file(dir.absoluteFilePath(TEST_XML));
    QVERIFY(file.open(QIODevice::WriteOnly|QIODevice::Truncate));
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("node");
    xml.writeTextElement("_name","");
    xml.writeStartElement("node");
    xml.writeTextElement("_name","Node0");
    xml.writeStartElement("node");
    xml.writeTextElement("_name","Node1");
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndDocument();
    QVERIFY(file.error() == QFileDevice::NoError);
}


void Abstract::createTestXmlLegacy(
)
{
    auto dir = QDir::temp();
    QVERIFY(dir.cd(TEST_DIR));
    QFile file(dir.absoluteFilePath(TEST_XML_LEGACY));
    QVERIFY(file.open(QIODevice::WriteOnly|QIODevice::Truncate));
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("NoDe");
    xml.writeTextElement("__p_name","");
    xml.writeStartElement("noDE");
    xml.writeTextElement("__p_name","Node0");
    xml.writeStartElement("nOdE");
    xml.writeTextElement("__p_name","Node1");
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndDocument();
    QVERIFY(file.error() == QFileDevice::NoError);
}
}
}
