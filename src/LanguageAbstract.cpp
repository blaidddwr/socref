#include "LanguageAbstract.h"
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
    Q_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


Model::Meta::Block* Abstract::blockMeta(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _blocks.size());
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
    Q_ASSERT(_meta);
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
    Q_ASSERT(meta);
    auto name = meta->name();
    Q_ASSERT(!_lookup.contains(name));
    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(name.at(0) != '_');
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
