#include "AbstractBlock.h"
#include <QtCore>
#include "ModelMetaBlock.h"


AbstractBlock::AbstractBlock(
    Model::Meta::Block* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    Q_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&AbstractBlock::onMetaDestroyed);
}


void AbstractBlock::append(
    AbstractBlock* block
)
{
    Q_ASSERT(block);
    Q_ASSERT(meta()->allowList().contains(block->meta()->index()));
    Q_ASSERT(!_children.contains(block));
    Q_ASSERT(!qobject_cast<AbstractBlock*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&AbstractBlock::onChildDestroyed);
    _children.append(block);
    block->addEvent(_children.size()-1);
}


QMap<QString,QStringList>& AbstractBlock::code(
)
{
    return _code;
}


const QMap<QString,QStringList>& AbstractBlock::code(
) const
{
    return _code;
}


AbstractBlock* AbstractBlock::copy(
    QObject* parent
) const
{
    auto ret = create(parent);
    ret->setState(state());
    for (auto child: _children)
    {
        ret->append(child->copy());
    }
    return ret;
}


QList<AbstractBlock*> AbstractBlock::descendants(
) const
{
    QList<AbstractBlock*> ret = _children;
    for (auto child: _children)
    {
        ret += child->descendants();
    }
    return ret;
}


AbstractBlock* AbstractBlock::get(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _children.size());
    return _children.at(index);
}


int AbstractBlock::indexOf(
    const AbstractBlock* block
) const
{
    return _children.indexOf(block);
}


void AbstractBlock::insert(
    int index
    ,AbstractBlock* block
)
{
    Q_ASSERT(block);
    Q_ASSERT(index >= 0);
    Q_ASSERT(index <= _children.size());
    Q_ASSERT(meta()->allowList().contains(block->meta()->index()));
    Q_ASSERT(!_children.contains(block));
    Q_ASSERT(!qobject_cast<AbstractBlock*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&AbstractBlock::onChildDestroyed);
    _children.insert(index,block);
    block->addEvent(index);
}


Model::Meta::Block* AbstractBlock::meta(
) const
{
    Q_ASSERT(_meta);
    return _meta;
}


void AbstractBlock::move(
    int from
    ,int to
)
{
    Q_ASSERT(from >= 0);
    Q_ASSERT(from < _children.size());
    Q_ASSERT(to >= 0);
    Q_ASSERT(to < _children.size());
    _children.move(from,to);
    _children.at(to)->moveEvent(from,to);
}


const QString& AbstractBlock::rootFileName(
)
{
    static const QString ret = "ROOT";
    return ret;
}


int AbstractBlock::size(
) const
{
    return _children.size();
}


AbstractBlock* AbstractBlock::take(
    int index
)
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _children.size());
    auto ret = _children.takeAt(index);
    disconnect(ret,&QObject::destroyed,this,&AbstractBlock::onChildDestroyed);
    ret->removeEvent(index);
    ret->setParent(nullptr);
    return ret;
}


void AbstractBlock::touchCode(
)
{
    emit codeChanged(_code);
}


void AbstractBlock::addEvent(
    int index
)
{
    Q_UNUSED(index);
}


void AbstractBlock::moveEvent(
    int from
    ,int to
)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
}


void AbstractBlock::removeEvent(
    int index
)
{
    Q_UNUSED(index);
}


void AbstractBlock::onChildDestroyed(
    QObject* object
)
{
    auto index = _children.indexOf(object);
    if (index != -1)
    {
        _children.removeAt(index);
    }
}


void AbstractBlock::onMetaDestroyed(
    QObject* object
)
{
    if (_meta == object)
    {
        _meta = nullptr;
    }
}
