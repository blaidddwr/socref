#include "StreamProjectXml.h"
#include <QtCore>
#include "AbstractLanguage.h"
#include "Exception.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "ModelMetaLanguage.h"
#include "ModelProject.h"
#include "StreamBlockXml.h"
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
    using FileSystem = Exception::FileSystem;
    using WriteProject = Exception::WriteProject;
    QFile file(_path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        throw FileSystem(QApp::tr("Failed opening %1: %2").arg(_path,file.errorString()));
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
            throw WriteProject(file.errorString());
        }
    }
    catch (Exception::Base& e)
    {
        throw WriteProject(QApp::tr("Failed writing project file %1: %2").arg(_path,e.message()));
    }
    return *this;
}


Model::Project* ProjectXml::load(
    QObject* parent
) const
{
    using FileSystem = Exception::FileSystem;
    using ReadProject = Exception::ReadProject;
    std::unique_ptr<Model::Project> ret(new Model::Project);
    QFile file(_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileSystem(QApp::tr("Failed opening %1: %2").arg(_path,file.errorString()));
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
                        throw ReadProject(QApp::tr("Project element missing version attribute."));
                    }
                    bool ok;
                    version = attributes.value("version").toInt(&ok);
                    if (!ok)
                    {
                        throw ReadProject(
                            QApp::tr("Invalid version %1.").arg(attributes.value("version"))
                            );
                    }
                    switch (version)
                    {
                    case Socref_1_0:
                        break;
                    default:
                        throw ReadProject(QApp::tr("Invalid version %1.").arg(version));
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
                        throw ReadProject(QApp::tr("Unknown language %1.").arg(langName));
                    }
                    ret->_language = factory->get(i);
                    Q_ASSERT(ret->_language);
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
                        throw ReadProject(QApp::tr("Version not set before first block element."));
                    }
                    if (!ret->_language)
                    {
                        throw ReadProject(QApp::tr("Language not set before first block element."));
                    }
                    ret->_root = BlockXml(xml,ret->_language,version).load(ret.get());
                    ret->connectAll();
                }
            }
        }
        if (xml.hasError())
        {
            throw ReadProject(xml.errorString());
        }
    }
    catch (ReadProject& e)
    {
        throw ReadProject(
            QApp::tr("Failed reading %1 on line %2: %3")
                .arg(_path)
                .arg(xml.lineNumber())
                .arg(e.message())
            );
    }
    if (!ret->_root)
    {
        throw ReadProject(QApp::tr("Missing root block in project XML file."));
    }
    ret->_modified = false;
    ret->setParent(parent);
    return ret.release();
}
}
