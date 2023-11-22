#include "BlockAbstract.h"
#include <QtCore>
#include "LanguageAbstract.h"
#include "Global.h"
#include "ModelMetaBlock.h"
namespace Block {


Abstract::Abstract(
    Model::Meta::Block* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    Q_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


void Abstract::append(
    Block::Abstract* block
)
{
    Q_ASSERT(block);
    Q_ASSERT(meta()->allowList().contains(block->meta()->index()));
    Q_ASSERT(!_children.contains(block));
    Q_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    _children.append(block);
    block->addEvent();
}


Block::Abstract* Abstract::fromXml(
    Language::Abstract* language
    ,int version
    ,QXmlStreamReader& xml
    ,QObject* parent
)
{
    Q_ASSERT(language);
    auto blockName = xml.name().toString();
    if (version == Socref_Legacy)
    {
        blockName = blockName.toLower();
    }
    auto i = language->indexFromName(blockName);
    if (i == -1)
    {
        throw tr("Unknown block %1").arg(blockName);
    }
    QMap<QString,QVariant> map;
    std::unique_ptr<Abstract> block(language->create(i));
    while (!xml.atEnd())
    {
        xml.readNext();
        switch (xml.tokenType())
        {
        case QXmlStreamReader::StartElement:
        {
            auto name = xml.name().toString();
            bool isProp = false;
            if (version == Socref_Legacy)
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
                    throw tr("Duplicate property element %1").arg(name);
                }
                map.insert(name,xml.readElementText());
            }
            else
            {
                block->append(fromXml(language,version,xml));
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == blockName)
            {
                block->loadFromMap(map,Socref_Legacy);
                block->setParent(parent);
                return block.release();
            }
            break;
        default:
            break;
        }
    }
    return nullptr;
}


Block::Abstract* Abstract::get(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _children.size());
    return _children.at(index);
}


int Abstract::indexOf(
    Block::Abstract* block
)
{
    return _children.indexOf(block);
}


void Abstract::insert(
    int index
    ,Block::Abstract* block
)
{
    Q_ASSERT(block);
    Q_ASSERT(meta()->allowList().contains(block->meta()->index()));
    Q_ASSERT(!_children.contains(block));
    Q_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    _children.insert(index,block);
    block->addEvent();
}


Model::Meta::Block* Abstract::meta(
) const
{
    Q_ASSERT(_meta);
    return _meta;
}


int Abstract::size(
) const
{
    return _children.size();
}


Block::Abstract* Abstract::take(
    int index
)
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _children.size());
    _children.at(index)->removeEvent();
    auto ret = _children.takeAt(index);
    ret->setParent(nullptr);
    return ret;
}


void Abstract::toXml(
    QXmlStreamWriter& xml
)
{
    xml.writeStartElement(meta()->name());
    auto map = saveToMap();
    for (auto i = map.begin();i != map.end();i++)
    {
        xml.writeTextElement("_"+i.key(),i.value().toString());
    }
    for (auto child: _children)
    {
        child->toXml(xml);
    }
    xml.writeEndElement();
}


void Abstract::addEvent(
)
{
}


void Abstract::removeEvent(
)
{
}


void Abstract::onMetaDestroyed(
    QObject* object
)
{
    if (_meta == object)
    {
        _meta = nullptr;
    }
}
}
