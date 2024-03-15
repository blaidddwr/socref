#include "StreamProjectDir.h"
#include <QtCore>
#include "ExceptionProjectLogical.h"
#include "ExceptionProjectRead.h"
#include "ExceptionProjectWrite.h"
#include "ExceptionSystemFile.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaLanguage.h"
#include "ModelProject.h"
#include "StreamBlockDir.h"
#include "gassert.h"
#define CONFIG_FILE "project.xml"
namespace Stream {
using QApp = QCoreApplication;


ProjectDir::ProjectDir(
    const QString& path
):
    _dir(path)
{
}


ProjectDir& ProjectDir::operator<<(
    Model::Project& project
)
{
    using FileError = Exception::System::File;
    if (!_dir.exists())
    {
        if (!_dir.mkpath("."))
        {
            throw FileError(QApp::tr("Failed creating directory %1.").arg(_dir.path()));
        }
    }
    if (!_dir.isReadable())
    {
        throw FileError(QApp::tr("The directory at %1 is not readable.").arg(_dir.path()));
    }
    write(project);
    BlockDir(_dir.absolutePath()) << *project._root;
    project.setDirectoryPath(_dir.absolutePath());
    project.setModified(false);
    return *this;
}


Model::Project* ProjectDir::load(
    QObject* parent
) const
{
    using FileError = Exception::System::File;
    std::unique_ptr<Model::Project> ret(new Model::Project);
    if (!_dir.exists())
    {
        throw FileError(QApp::tr("No such directory at path %1.").arg(_dir.path()));
    }
    if (!_dir.isReadable())
    {
        throw FileError(QApp::tr("The directory at %1 is not readable.").arg(_dir.path()));
    }
    auto version = read(*ret);
    ret->_root = BlockDir(_dir.absolutePath(),ret->_language,version).load(ret.get());
    ret->connectAll();
    ret->_directoryPath = QFileInfo(_dir.path()).absoluteFilePath();
    ret->_modified = false;
    ret->setParent(parent);
    return ret.release();
}


QStringList ProjectDir::orphanFiles(
    const Model::Project& project
) const
{
    using LogicalError = Exception::Project::Logical;
    if (project._directoryPath.isNull())
    {
        throw LogicalError(
            QApp::tr("Cannot generate deprecated files from new project without directory path.")
        );
    }
    return BlockDir(project._directoryPath).orphanFiles(*project._root);
}


void ProjectDir::removeOrphanFiles(
    const QStringList& paths
    ,const Model::Project& project
    ,bool git
) const
{
    using LogicalError = Exception::Project::Logical;
    if (project._directoryPath.isNull())
    {
        throw LogicalError(
            QApp::tr("Cannot remove deprecated files from new project without directory path.")
        );
    }
    BlockDir(project._directoryPath).removeOrphanFiles(paths,*project._root,git);
}


int ProjectDir::read(
    Model::Project& project
) const
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Project::Read;
    auto path = _dir.absoluteFilePath(CONFIG_FILE);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    int version = -1;
    QXmlStreamReader xml(&file);
    try
    {
        while(!xml.atEnd())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                auto name = xml.name().toString();
                if (name == "SocratesReference")
                {
                    auto attributes = xml.attributes();
                    if (!attributes.hasAttribute("version"))
                    {
                        throw ReadError(QApp::tr("Project element missing version attribute."));
                    }
                    bool ok;
                    version = attributes.value("version").toInt(&ok);
                    if (!ok)
                    {
                        throw ReadError(
                            QApp::tr("Invalid version %1.").arg(attributes.value("version"))
                        );
                    }
                    switch (version)
                    {
                    case Socref_1_0:
                        break;
                    default:
                        throw ReadError(QApp::tr("Invalid version %1.").arg(version));
                    }
                }
                else if (name == "language")
                {
                    auto langName = xml.readElementText();
                    auto factory = Factory::Language::instance();
                    auto i = factory->indexFromName(langName);
                    if (i == -1)
                    {
                        throw ReadError(QApp::tr("Unknown language %1.").arg(langName));
                    }
                    project._language = factory->get(i);
                    G_ASSERT(project._language);
                    QApp::connect(
                        project._language
                        ,&QObject::destroyed
                        ,&project
                        ,&Model::Project::onLanguageDestroyed
                    );
                }
                else if (name == "name")
                {
                    project._name = xml.readElementText();
                }
                else if (name == "relativeCodePath")
                {
                    project._relativeCodePath = xml.readElementText();
                }
            }
        }
        if (xml.hasError())
        {
            throw ReadError(xml.errorString());
        }
    }
    catch (ReadError& e)
    {
        throw ReadError(
            QApp::tr("Failed reading %1 on line %2: %3")
                .arg(path)
                .arg(xml.lineNumber())
                .arg(e.message())
        );
    }
    if (version == -1)
    {
        throw ReadError(QApp::tr("Version not set in project config file."));
    }
    if (!project._language)
    {
        throw ReadError(QApp::tr("Language not set in project config file."));
    }
    return version;
}


void ProjectDir::write(
    const Model::Project& project
) const
{
    using FileError = Exception::System::File;
    using WriteError = Exception::Project::Write;
    auto path = _dir.absoluteFilePath(CONFIG_FILE);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("SocratesReference");
    xml.writeAttribute("version",QString::number(Socref_Current));
    xml.writeTextElement("name",project._name);
    xml.writeTextElement("language",project._language->meta()->name());
    xml.writeTextElement("relativeCodePath",project._relativeCodePath);
    xml.writeEndElement();
    xml.writeEndDocument();
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(
            QApp::tr("Failed writing project file %1: %2").arg(path,file.errorString())
        );
    }
}
}
