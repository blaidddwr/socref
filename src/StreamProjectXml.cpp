#include "StreamProjectXml.h"
#include <QtCore>
#include "ExceptionProjectRead.h"
#include "ExceptionProjectWrite.h"
#include "ExceptionSystemFile.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaLanguage.h"
#include "ModelProject.h"
#include "StreamBlockXml.h"
#include "gassert.h"
namespace Stream {
using QApp = QCoreApplication;


ProjectXml::ProjectXml(
    const QString& path
):
    _path(path)
{
}


ProjectXml& ProjectXml::operator<<(
    const Model::Project& project
)
{
    using FileError = Exception::System::File;
    using WriteError = Exception::Project::Write;
    QFile file(_path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(_path,file.errorString()));
    }
    try
    {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement("SocratesReference");
        xml.writeAttribute("version",QString::number(Socref_Current));
        xml.writeTextElement("name",project._name);
        xml.writeTextElement("language",project._language->meta()->name());
        xml.writeTextElement("relativeCodePath",project._relativeCodePath);
        BlockXml(xml) << *project._root;
        xml.writeEndElement();
        xml.writeEndDocument();
        if (file.error() != QFileDevice::NoError)
        {
            throw WriteError(file.errorString());
        }
    }
    catch (Exception::Base& e)
    {
        throw WriteError(QApp::tr("Failed writing project file %1: %2").arg(_path,e.message()));
    }
    return *this;
}


Model::Project* ProjectXml::load(
    QObject* parent
) const
{
    using FileError = Exception::System::File;
    using ReadError = Exception::Project::Read;
    std::unique_ptr<Model::Project> ret(new Model::Project);
    QFile file(_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileError(QApp::tr("Failed opening %1: %2").arg(_path,file.errorString()));
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
                    auto langName = xml.readElementText().toLower();
                    langName.replace("+","p").replace("/","");
                    auto factory = Factory::Language::instance();
                    auto i = factory->indexFromName(langName);
                    if (i == -1)
                    {
                        throw ReadError(QApp::tr("Unknown language %1.").arg(langName));
                    }
                    ret->_language = factory->get(i);
                    G_ASSERT(ret->_language);
                    QApp::connect(
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
                    (version == Socref_Legacy && name == "parse_path")
                    || name == "relativeCodePath"
                )
                {
                    ret->_relativeCodePath = xml.readElementText();
                }
                else if (!ret->_root)
                {
                    if (version == -1)
                    {
                        throw ReadError(QApp::tr("Version not set before first block element."));
                    }
                    if (!ret->_language)
                    {
                        throw ReadError(QApp::tr("Language not set before first block element."));
                    }
                    ret->_root = BlockXml(xml,ret->_language,version).load(ret.get());
                    ret->connectAll();
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
                .arg(_path)
                .arg(xml.lineNumber())
                .arg(e.message())
        );
    }
    if (!ret->_root)
    {
        throw ReadError(QApp::tr("Missing root block in project XML file."));
    }
    ret->_modified = false;
    ret->setParent(parent);
    return ret.release();
}
}
