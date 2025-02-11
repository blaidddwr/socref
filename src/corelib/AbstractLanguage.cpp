#include "AbstractLanguage.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"


AbstractLanguage::AbstractLanguage(
    Model::Meta::Language* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    Q_ASSERT(meta);
    meta->setParent(this);
}


Model::Meta::Block* AbstractLanguage::blockMeta(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _blocks.size());
    return _blocks.at(index);
}


int AbstractLanguage::indexFromName(
    const QString& name
) const
{
    return _lookup.value(name,-1);
}


Model::Meta::Language* AbstractLanguage::meta(
) const
{
    Q_ASSERT(_meta);
    return _meta;
}


int AbstractLanguage::size(
) const
{
    return _blocks.size();
}


void AbstractLanguage::appendBlock(
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


void AbstractLanguage::appendBlocks(
    const QList<Model::Meta::Block*>& metas
)
{
    for (auto meta: metas)
    {
        appendBlock(meta);
    }
}
