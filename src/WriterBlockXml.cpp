#include "WriterBlockXml.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "ExceptionBlockWrite.h"
#include "ModelMetaBlock.h"
namespace Writer {


BlockXml::BlockXml(
    QXmlStreamWriter& xml
    ,QObject* parent
):
    QObject(parent)
    ,_xml(xml)
{
}


Writer::BlockXml& BlockXml::operator<<(
    const ::Block::Abstract& block
)
{
    using WriteError = Exception::Block::Write;
    _xml.writeStartElement(block.meta()->name());
    auto map = block.saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        _xml.writeTextElement("_"+i.key(),i.value().toString());
    }
    for (auto child: block._children)
    {
        *this << *child;
    }
    _xml.writeEndElement();
    if (_xml.hasError())
    {
        throw WriteError(_xml.device()->errorString());
    }
    return *this;
}
}
