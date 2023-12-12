#include "TestBase.h"
#include <QtGui>
#include "Exception.h"
#include "LanguageAbstract.h"
#define TEST_DIR "socref.unit_test.d"
#define TEST_PROJ_DIR "testDir"
#define TEST_XML "test.xml"
#define TEST_XML_LEGACY "test.legacy.xml"
namespace Test {


bool Base::areDirsEqual(
    const QDir& dir0
    ,const QDir& dir1
    ,const QString& filter
)
{
    for (const auto& info: dir0.entryInfoList({filter},QDir::Files))
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


bool Base::areFilesEqual(
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


bool Base::areIconsEqual(
    const QIcon& icon0
    ,const QIcon& icon1
)
{
    QByteArray data0;
    QByteArray data1;
    QDataStream in0(&data0,QIODevice::WriteOnly);
    QDataStream in1(&data1,QIODevice::WriteOnly);
    in0 << icon0;
    in1 << icon1;
    return data0 == data1;
}


bool Base::areXmlFilesEqual(
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


void Base::cleanup(
)
{
    auto dir = QDir::temp();
    if (dir.exists(TEST_DIR))
    {
        QProcess::execute("rm",{"-fr",dir.absoluteFilePath(TEST_DIR)});
    }
}


bool Base::createTestDir(
)
{
    auto dir = QDir::temp();
    if (dir.exists(TEST_DIR))
    {
        QProcess::execute("rm",{"-fr",dir.absoluteFilePath(TEST_DIR)});
    }
    if (!dir.mkdir(TEST_DIR))
    {
        return false;
    }
    if (!createTestXml())
    {
        return false;
    }
    if (!createTestXmlLegacy())
    {
        return false;
    }
    if (!createTestProjectDir())
    {
        return false;
    }
    return true;
}


void Base::initLanguage(
    Language::Abstract* language
)
{
    G_ASSERT(language);
    G_ASSERT(!_language);
    _language = language;
    _language->setParent(this);
}


Language::Abstract* Base::language(
) const
{
    G_ASSERT(_language);
    return _language;
}


QString Base::testDir(
) const
{
    return QDir::temp().absoluteFilePath(TEST_DIR);
}


QString Base::testProjDir(
) const
{
    return QDir(testDir()).absoluteFilePath(TEST_PROJ_DIR);
}


QString Base::testXml(
) const
{
    return QDir(testDir()).absoluteFilePath(TEST_XML);
}


QString Base::testXmlLegacy(
) const
{
    return QDir(testDir()).absoluteFilePath(TEST_XML_LEGACY);
}


void Base::writeEndProjectXml(
    QXmlStreamWriter& xml
)
{
    xml.writeEndElement();
}


void Base::writeStartProjectXml(
    QXmlStreamWriter& xml
)
{
    xml.writeStartElement("socref");
    xml.writeTextElement("name","Testing123");
    xml.writeTextElement("language","testing");
    xml.writeTextElement("relativeParsePath","../testing");
}


bool Base::createBlockFile(
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


bool Base::createTestProjectConfig(
    const QString& path
)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return false;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("socref");
    xml.writeTextElement("name","Testing123");
    xml.writeTextElement("language","test");
    xml.writeTextElement("relativeParsePath","../testing");
    xml.writeEndElement();
    xml.writeEndDocument();
    return (file.error() == QFileDevice::NoError);
}


bool Base::createTestProjectDir(
)
{
    auto dir = QDir::temp();
    if (!dir.cd(TEST_DIR))
    {
        return false;
    }
    if (!dir.mkdir(TEST_PROJ_DIR))
    {
        return false;
    }
    if (!dir.cd(TEST_PROJ_DIR))
    {
        return false;
    }
    if (!createTestProjectConfig(dir.absoluteFilePath("project.xml")))
    {
        return false;
    }
    if (!createBlockFile(dir.absoluteFilePath("ROOT.srb"),"",{"Node0"}))
    {
        return false;
    }
    if (!createBlockFile(dir.absoluteFilePath("Node0.srb"),"Node\\n0",{"Node0:Node1"}))
    {
        return false;
    }
    if (!createBlockFile(dir.absoluteFilePath("Node0:Node1.srb"),"Node\\n1",{}))
    {
        return false;
    }
    return true;
}


bool Base::createTestXml(
)
{
    auto dir = QDir::temp();
    if (!dir.cd(TEST_DIR))
    {
        return false;
    }
    QFile file(dir.absoluteFilePath(TEST_XML));
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return false;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    writeStartProjectXml(xml);
    xml.writeStartElement("node");
    xml.writeTextElement("_name","");
    xml.writeStartElement("node");
    xml.writeTextElement("_name","Node0");
    xml.writeStartElement("node");
    xml.writeTextElement("_name","Node1");
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    writeEndProjectXml(xml);
    xml.writeEndDocument();
    return (file.error() == QFileDevice::NoError);
}


bool Base::createTestXmlLegacy(
)
{
    auto dir = QDir::temp();
    if (!dir.cd(TEST_DIR))
    {
        return false;
    }
    QFile file(dir.absoluteFilePath(TEST_XML_LEGACY));
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return false;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("srp_project");
    xml.writeTextElement("name","Testing123");
    xml.writeTextElement("language","test");
    xml.writeTextElement("parse_path","../testing");
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
    return (file.error() == QFileDevice::NoError);
}
}
