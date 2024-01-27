#include "StreamProject.h"
#include <QtCore>
#include "Exceptions.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaLanguage.h"
#include "ModelProject.h"
#include "StreamBlock.h"
#define CONFIG_FILE "project.xml"
namespace Stream {
Project* Project::_instance {nullptr};


QStringList Project::deprecatedFiles(
    const Model::Project& project
)
{
    using LogicalError = Exception::Project::Logical;
    if (project._directoryPath.isNull())
    {
        throw LogicalError(
            tr("Cannot generate deprecated files from new project without directory path.")
        );
    }
    return Block::deprecatedFiles(*project._root,project._directoryPath);
}


Model::Project* Project::fromDir(
    const QString& path
    ,QObject* parent
)
{
    using FileError = Exception::System::File;
    std::unique_ptr<Model::Project> ret(new Model::Project);
    QDir dir(path);
    if (!dir.exists())
    {
        throw FileError(tr("No such directory at path %1.").arg(path));
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("The directory at %1 is not readable.").arg(path));
    }
    auto version = read(dir.absoluteFilePath(CONFIG_FILE),*ret);
    ret->_root = Block::fromDir(ret->_language,version,dir.absolutePath(),ret.get());
    ret->_directoryPath = QFileInfo(path).absoluteFilePath();
    ret->setParent(parent);
    return ret.release();
}


Model::Project* Project::fromXml(
    const QString& path
    ,QObject* parent
)
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Project::Read;
    std::unique_ptr<Model::Project> ret(new Model::Project);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    int version = -1;
    QXmlStreamReader xml(&file);
    try
    {
        while (!xml.atEnd())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                auto name = xml.name().toString();
                if (name == "srp_project")
                {
                    version = Socref_Legacy;
                }
                else if (name == "SocratesReference")
                {
                    auto attributes = xml.attributes();
                    if (!attributes.hasAttribute("version"))
                    {
                        throw ReadError(tr("Project element missing version attribute."));
                    }
                    bool ok;
                    version = attributes.value("version").toInt(&ok);
                    if (!ok)
                    {
                        throw ReadError(tr("Invalid version %1.").arg(attributes.value("version")));
                    }
                    switch (version)
                    {
                    case Socref_1_0:
                        break;
                    default:
                        throw ReadError(tr("Invalid version %1.").arg(version));
                    }
                }
                else if (name == "language")
                {
                    auto langName = xml.readElementText().toLower();
                    langName.replace("+","p").replace("/","");
                    auto factory = Factory::Language::instance();
                    auto i = factory->indexFromName(langName);
                    if (i == -1)
                    {
                        throw ReadError(tr("Unknown language %1.").arg(langName));
                    }
                    ret->_language = factory->get(i);
                    G_ASSERT(ret->_language);
                    connect(
                        ret->_language
                        ,&QObject::destroyed
                        ,ret.get()
                        ,&Model::Project::onLanguageDestroyed
                    );
                }
                else if (name == "name")
                {
                    ret->_name = xml.readElementText();
                }
                else if (
                    name == "parse_path"
                    || name == "relativeParsePath"
                )
                {
                    ret->_relativeParsePath = xml.readElementText();
                }
                else if (!ret->_root)
                {
                    if (version == -1)
                    {
                        throw ReadError(tr("Version not set before first block element."));
                    }
                    if (!ret->_language)
                    {
                        throw ReadError(tr("Language not set before first block element."));
                    }
                    ret->_root = Block::fromXml(ret->_language,version,xml,ret.get());
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
            tr("Failed reading %1 on line %2: %3").arg(path).arg(xml.lineNumber()).arg(e.message())
        );
    }
    if (!ret->_root)
    {
        throw ReadError(tr("Missing root block in project XML file."));
    }
    ret->setParent(parent);
    return ret.release();
}


void Project::removeFiles(
    const Model::Project& project
    ,bool git
)
{
    using LogicalError = Exception::Project::Logical;
    if (project._directoryPath.isNull())
    {
        throw LogicalError(
            tr("Cannot remove deprecated files from new project without directory path.")
        );
    }
    Block::removeFiles(*project._root,project._directoryPath,git);
}


void Project::toDir(
    const Model::Project& project
)
{
    using LogicalError = Exception::Project::Logical;
    if (project._directoryPath.isNull())
    {
        throw LogicalError(tr("Cannot save new project without directory path."));
    }
    saveDir(project,project._directoryPath);
}


void Project::toDir(
    Model::Project& project
    ,const QString& path
)
{
    saveDir(project,path);
    project.setDirectoryPath(QFileInfo(path).absolutePath());
}


void Project::toXml(
    const Model::Project& project
    ,const QString& path
)
{
}


int Project::read(
    const QString& path
    ,Model::Project& project
)
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Project::Read;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
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
                        throw ReadError(tr("Project element missing version attribute."));
                    }
                    bool ok;
                    version = attributes.value("version").toInt(&ok);
                    if (!ok)
                    {
                        throw ReadError(tr("Invalid version %1.").arg(attributes.value("version")));
                    }
                    switch (version)
                    {
                    case Socref_1_0:
                        break;
                    default:
                        throw ReadError(tr("Invalid version %1.").arg(version));
                    }
                }
                else if (name == "language")
                {
                    auto langName = xml.readElementText();
                    auto factory = Factory::Language::instance();
                    auto i = factory->indexFromName(langName);
                    if (i == -1)
                    {
                        throw ReadError(tr("Unknown language %1.").arg(langName));
                    }
                    project._language = factory->get(i);
                    G_ASSERT(project._language);
                    connect(
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
                else if (name == "relativeParsePath")
                {
                    project._relativeParsePath = xml.readElementText();
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
            tr("Failed reading %1 on line %2: %3").arg(path).arg(xml.lineNumber()).arg(e.message())
        );
    }
    if (version == -1)
    {
        throw ReadError(tr("Version not set in project config file."));
    }
    if (!project._language)
    {
        throw ReadError(tr("Language not set in project config file."));
    }
    return version;
}


void Project::saveDir(
    const Model::Project& project
    ,const QString& path
)
{
    using FileError = Exception::System::File;
    QDir dir(path);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            throw FileError(tr("Failed creating directory %1.").arg(path));
        }
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("The directory at %1 is not readable.").arg(path));
    }
    write(project,dir.absoluteFilePath(CONFIG_FILE));
    Block::toDir(*project._root,path);
}


void Project::write(
    const Model::Project& project
    ,const QString& path
)
{
    using FileError = Exception::System::File;
    using WriteError = Exception::Project::Write;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("SocratesReference");
    xml.writeAttribute("version",QString::number(Socref_1_0));
    xml.writeTextElement("name",project._name);
    xml.writeTextElement("language",project._language->meta()->name());
    xml.writeTextElement("relativeParsePath",project._relativeParsePath);
    xml.writeEndElement();
    xml.writeEndDocument();
    if (file.error() != QFileDevice::NoError)
    {
        throw WriteError(tr("Failed writing project file %1: %2").arg(path,file.errorString()));
    }
}
}