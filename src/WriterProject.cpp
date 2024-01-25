#include "WriterProject.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "Exception.h"
#include "ExceptionProjectWrite.h"
#include "ExceptionSystemFile.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaLanguage.h"
#include "ModelProject.h"
#define CONFIG_FILE "project.xml"
namespace Writer {


Project::Project(
    const QString& path
    ,QObject* parent
):
    QObject(parent)
    ,_path(path)
{
}


Writer::Project& Project::operator<<(
    const Model::Project& project
)
{
    G_ASSERT(_open);
    writeConfig(project);
    project._root->toDir(_path);
    return *this;
}


void Project::open(
)
{
    using FileError = Exception::System::File;
    QDir dir(_path);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            throw FileError(tr("Failed creating directory %1.").arg(_path));
        }
    }
    if (!dir.isReadable())
    {
        throw FileError(tr("The directory at %1 is not readable.").arg(_path));
    }
    _open = true;
}


void Project::writeConfig(
    const Model::Project& project
)
{
    using FileError = Exception::System::File;
    using WriteError = Exception::Project::Write;
    QDir dir(_path);
    auto path = dir.absoluteFilePath(CONFIG_FILE);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(tr("Failed opening %1: %2").arg(path,file.errorString()));
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("socref");
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
