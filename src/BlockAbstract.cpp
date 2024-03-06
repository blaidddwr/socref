#include "BlockAbstract.h"
#include <QtCore>
#include <random>
#include "ModelMetaBlock.h"
#include "gassert.h"
namespace Block {


Abstract::Abstract(
    Model::Meta::Block* meta
    ,QObject* parent
):
    QObject(parent)
    ,_meta(meta)
{
    G_ASSERT(meta);
    connect(meta,&QObject::destroyed,this,&Abstract::onMetaDestroyed);
}


void Abstract::append(
    Abstract* block
)
{
    G_ASSERT(block);
    G_ASSERT(meta()->allowList().contains(block->meta()->index()));
    G_ASSERT(!_children.contains(block));
    G_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    _children.append(block);
    block->addEvent(_children.size()-1);
}


Abstract* Abstract::copy(
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


QList<Abstract*> Abstract::descendants(
) const
{
    QList<Block::Abstract*> ret = _children;
    for (auto child: _children)
    {
        ret += child->descendants();
    }
    return ret;
}


Abstract* Abstract::get(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _children.size());
    return _children.at(index);
}


int Abstract::indexOf(
    const Abstract* block
) const
{
    return _children.indexOf(block);
}


void Abstract::insert(
    int index
    ,Abstract* block
)
{
    G_ASSERT(block);
    G_ASSERT(index >= 0);
    G_ASSERT(index <= _children.size());
    G_ASSERT(meta()->allowList().contains(block->meta()->index()));
    G_ASSERT(!_children.contains(block));
    G_ASSERT(!qobject_cast<Abstract*>(block->parent()));
    block->setParent(this);
    connect(block,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    _children.insert(index,block);
    block->addEvent(index);
}


Model::Meta::Block* Abstract::meta(
) const
{
    G_ASSERT(_meta);
    return _meta;
}


void Abstract::move(
    int from
    ,int to
)
{
    G_ASSERT(from >= 0);
    G_ASSERT(from < _children.size());
    G_ASSERT(to >= 0);
    G_ASSERT(to < _children.size());
    _children.move(from,to);
    _children.at(to)->moveEvent(from,to);
}


const QString& Abstract::rootFileName(
)
{
    static const QString ret = "ROOT";
    return ret;
}


int Abstract::size(
) const
{
    return _children.size();
}


Abstract* Abstract::take(
    int index
)
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _children.size());
    auto ret = _children.takeAt(index);
    disconnect(ret,&QObject::destroyed,this,&Abstract::onChildDestroyed);
    ret->removeEvent(index);
    ret->setParent(nullptr);
    return ret;
}


void Abstract::addEvent(
    int index
)
{
    Q_UNUSED(index);
}


void Abstract::moveEvent(
    int from
    ,int to
)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
}


void Abstract::removeEvent(
    int index
)
{
    Q_UNUSED(index);
}


void Abstract::onChildDestroyed(
    QObject* object
)
{
    auto index = _children.indexOf(object);
    if (index != -1)
    {
        _children.removeAt(index);
    }
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
