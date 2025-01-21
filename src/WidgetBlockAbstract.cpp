#include "WidgetBlockAbstract.h"
#include "BlockAbstract.h"
#include "ModelProject.h"
#include "gassert.h"
namespace Widget {
namespace Block {


Abstract::Abstract(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    QWidget(parent)
    ,_block(block)
{
    G_ASSERT(_block);
    connect(
        const_cast<::Block::Abstract*>(_block)
        ,&QObject::destroyed
        ,this
        ,&Abstract::onBlockDestroyed
    );
}


bool Abstract::modified(
) const
{
    return _modified;
}


void Abstract::setIndex(
    const QPersistentModelIndex& index
)
{
    G_ASSERT(index.isValid());
    G_ASSERT(index.internalPointer() == _block);
    if (_index != index)
    {
        _index = index;
    }
}


void Abstract::setModel(
    Model::Project* model
)
{
    if (_model)
    {
        disconnect(_model,&QObject::destroyed,this,&Abstract::onModelDestroyed);
    }
    _model = model;
    if (_model)
    {
        connect(_model,&QObject::destroyed,this,&Abstract::onModelDestroyed);
    }
}


void Abstract::setModified(
    bool value
)
{
    if (_modified != value)
    {
        _modified = value;
        emit modifiedChanged(value);
    }
}


void Abstract::touch(
)
{
    setModified(true);
}


void Abstract::abortSet(
)
{
    G_ASSERT(_model);
    G_ASSERT(_index.isValid());
    G_ASSERT(_index.model() == _model);
    G_ASSERT(_block);
    G_ASSERT(_index.internalPointer() == _block);
    _model->abortSet();
}


void Abstract::finishSet(
)
{
    G_ASSERT(_model);
    G_ASSERT(_index.isValid());
    G_ASSERT(_index.model() == _model);
    G_ASSERT(_block);
    G_ASSERT(_index.internalPointer() == _block);
    _model->finishSet();
}


void Abstract::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        _block = nullptr;
    }
}


void Abstract::onModelDestroyed(
    QObject* object
)
{
    if (_model == object)
    {
        _model = nullptr;
    }
}
}
}
