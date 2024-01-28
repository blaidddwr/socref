#include "LanguageAbstract.h"
#include "Exceptions.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
namespace Language {


Abstract::Abstract(
    Model::Meta::Language* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    G_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


Model::Meta::Block* Abstract::blockMeta(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _blocks.size());
    return _blocks.at(index);
}


int Abstract::indexFromName(
    const QString& name
) const
{
    return _lookup.value(name,-1);
}


Model::Meta::Language* Abstract::meta(
) const
{
    G_ASSERT(_meta);
    return _meta;
}


int Abstract::size(
) const
{
    return _blocks.size();
}


void Abstract::appendBlock(
    Model::Meta::Block* meta
)
{
    G_ASSERT(meta);
    auto name = meta->name();
    G_ASSERT(!_lookup.contains(name));
    G_ASSERT(!name.isEmpty());
    G_ASSERT(name.at(0) != '_');
    meta->setParent(this);
    _lookup.insert(name,_blocks.size());
    _blocks.append(meta);
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
