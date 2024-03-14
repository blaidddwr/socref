#include "TestModelProject.h"
#include <QtTest>
#include "ExceptionBlockRead.h"
#include "ExceptionBlockWrite.h"
#include "ExceptionProjectRead.h"
#include "ExceptionProjectWrite.h"
#include "ExceptionSystemFile.h"
#include "FactoryLanguage.h"
#include "ModelProject.h"
#include "StreamProject.h"
namespace Test {
namespace Model {


void Project::toDirFromDir(
)
{
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test");
    auto out = new ::Model::Project(Factory::Language::instance()->indexFromName("cppqt"),this);
    QCOMPARE(out->modified(),true);
    out->setName("Test Project Name");
    out->setRelativeCodePath("../test/path");
    auto stream = Stream::Project::instance();
    try
    {
        stream->toDir(*out,path);
    }
    catch (Exception::Project::Write& e)
    {
        qDebug() << tr("Write Project Exception: %1").arg(e.message());
    }
    catch (Exception::Block::Write& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QCOMPARE(out->modified(),false);
    QCOMPARE(out->directoryPath(),path);
    ::Model::Project* in = nullptr;
    try
    {
        in = stream->fromDir(path,this);
    }
    catch (Exception::Project::Read& e)
    {
        qDebug() << tr("Read Project Exception: %1").arg(e.message());
    }
    catch (Exception::Block::Read& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
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


void Project::toXmlFromXml(
)
{
    auto dir = QDir::temp();
    auto path = dir.absoluteFilePath("socref.project.test.xml");
    auto out = new ::Model::Project(Factory::Language::instance()->indexFromName("cppqt"),this);
    QCOMPARE(out->modified(),true);
    out->setName("Test Project Name");
    out->setRelativeCodePath("../test/path");
    auto stream = Stream::Project::instance();
    try
    {
        stream->toXml(*out,path);
    }
    catch (Exception::Project::Write& e)
    {
        qDebug() << tr("Write Project Exception: %1").arg(e.message());
    }
    catch (Exception::Block::Write& e)
    {
        qDebug() << tr("Write Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
    {
        qDebug() << tr("File System Exception: %1").arg(e.message());
    }
    QCOMPARE(out->modified(),true);
    QCOMPARE(out->directoryPath(),"");
    ::Model::Project* in = nullptr;
    try
    {
        in = stream->fromXml(path,this);
    }
    catch (Exception::Project::Read& e)
    {
        qDebug() << tr("Read Project Exception: %1").arg(e.message());
    }
    catch (Exception::Block::Read& e)
    {
        qDebug() << tr("Read Block Exception: %1").arg(e.message());
    }
    catch (Exception::System::File& e)
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
}
}
