#include "StreamBlockXml.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "ExceptionBlockRead.h"
#include "ExceptionBlockWrite.h"
#include "Global.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "gassert.h"
namespace Stream {
using QApp = QCoreApplication;


BlockXml::BlockXml(
    QXmlStreamWriter& xml
):
    _xmlWriter(&xml)
{
}


BlockXml::BlockXml(
    QXmlStreamReader& xml
    ,Language::Abstract* language
    ,int version
):
    _language(language)
    ,_xmlReader(&xml)
    ,_version(version)
{
    G_ASSERT(_language);
    G_ASSERT(_version >= Socref_Legacy);
    G_ASSERT(_version <= Socref_Current);
}


BlockXml& BlockXml::operator<<(
    const ::Block::Abstract& block
)
{
    G_ASSERT(_xmlWriter);
    write(block,*_xmlWriter);
    return *this;
}


::Block::Abstract* BlockXml::load(
    QObject* parent
) const
{
    using ReadError = Exception::Block::Read;
    G_ASSERT(_xmlReader);
    G_ASSERT(_language);
    G_ASSERT(_version != -1);
    auto& xml = *_xmlReader;
    auto blockName = xml.name().toString();
    int i = -1;
    if (_version == Socref_Legacy)
    {
        i = _language->indexFromName(blockName.toLower());
    }
    else
    {
        i = _language->indexFromName(blockName);
    }
    if (i == -1)
    {
        throw ReadError(QApp::tr("Unknown block %1.").arg(blockName));
    }
    QMap<QString,QVariant> map;
    std::unique_ptr<::Block::Abstract> block(_language->create(i));
    while (!xml.atEnd())
    {
        xml.readNext();
        switch (xml.tokenType())
        {
        case QXmlStreamReader::StartElement:
        {
            auto name = xml.name().toString();
            bool isProp = false;
            if (_version == Socref_Legacy)
            {
                if (name.first(4) == "__p_")
                {
                    name = name.mid(4);
                    isProp = true;
                }
            }
            else
            {
                if (name.at(0) == '_')
                {
                    name = name.mid(1);
                    isProp = true;
                }
            }
            if (isProp)
            {
                if (map.contains(name))
                {
                    throw ReadError(QApp::tr("Duplicate property element %1.").arg(name));
                }
                map.insert(name,xml.readElementText());
            }
            else
            {
                auto child = load(block.get());
                child->setParent(nullptr);
                block->append(child);
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == blockName)
            {
                block->setParent(parent);
                try
                {
                    block->loadFromMap(map,Socref_Legacy);
                }
                catch (...)
                {
                    block->setParent(nullptr);
                    throw;
                }
                return block.release();
            }
            break;
        default:
            break;
        }
    }
    return nullptr;
}


void BlockXml::write(
    const ::Block::Abstract& block
    ,QXmlStreamWriter& xml
)
{
    using WriteError = Exception::Block::Write;
    G_ASSERT(_xmlWriter);
    xml.writeStartElement(block.meta()->name());
    auto map = block.saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        xml.writeTextElement("_"+i.key(),i.value().toString());
    }
    for (auto child: block._children)
    {
        write(*child,xml);
    }
    xml.writeEndElement();
    if (xml.hasError())
    {
        throw WriteError(xml.device()->errorString());
    }
}
}
