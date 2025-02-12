#include <QtTest>
#include "Exception.h"
#include "FactoryLanguage.h"
#include "ModelProject.h"
#include "StreamProjectDir.h"
#include "StreamProjectXml.h"
#include "../TestLanguage.h"

class TestModelProject: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void toDirFromDir();
    void toXmlFromXml();
};

void TestModelProject::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Factory::Language::instance()->appendLanguage(
        new TestLanguage(new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,QIcon()))
        );
}

void TestModelProject::toDirFromDir()
{
    using Stream = Stream::ProjectDir;
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test");
    auto out = new Model::Project(Factory::Language::instance()->indexFromName(LANGUAGE_NAME),this);
    QCOMPARE(out->modified(),true);
    out->setName("Test Project Name");
    out->setRelativeCodePath("../test/path");
    try
    {
        Stream(path) << *out;
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
    QCOMPARE(out->modified(),false);
    QCOMPARE(out->directoryPath(),path);
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
    QCOMPARE(in->name(),out->name());
    QCOMPARE(in->relativeCodePath(),out->relativeCodePath());
    QCOMPARE(in->directoryPath(),out->directoryPath());
    QCOMPARE(in->modified(),false);
    delete out;
    delete in;
}

void TestModelProject::toXmlFromXml()
{
    using Stream = Stream::ProjectXml;
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test.xml");
    auto out = new Model::Project(Factory::Language::instance()->indexFromName(LANGUAGE_NAME),this);
    QCOMPARE(out->modified(),true);
    out->setName("Test Project Name");
    out->setRelativeCodePath("../test/path");
    try
    {
        Stream(path) << *out;
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
    QCOMPARE(out->modified(),true);
    QCOMPARE(out->directoryPath(),"");
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
    QCOMPARE(in->name(),out->name());
    QCOMPARE(in->relativeCodePath(),out->relativeCodePath());
    QCOMPARE(in->directoryPath(),out->directoryPath());
    QCOMPARE(in->modified(),false);
    delete out;
    delete in;
}

QTEST_MAIN(TestModelProject)
#include "test.moc"
