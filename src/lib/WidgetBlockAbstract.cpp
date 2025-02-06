#include "WidgetBlockAbstract.h"
#include "BlockAbstract.h"
#include "ModelProject.h"
#include "WidgetDialogCode.h"
namespace Widget {
namespace Block {


Abstract::Abstract(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    QWidget(parent)
    ,_block(block)
{
    Q_ASSERT(_block);
    connect(
        const_cast<::Block::Abstract*>(_block)
        ,&QObject::destroyed
        ,this
        ,&Abstract::onBlockDestroyed
        );
    connect(
        const_cast<::Block::Abstract*>(_block)
        ,&::Block::Abstract::codeChanged
        ,this
        ,&Abstract::onBlockCodeChanged
        );
}


void Abstract::showCode(
)
{
    Q_ASSERT(_block);
    if (!_block->code().isEmpty())
    {
        Dialog::Code(_block,this).exec();
    }
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
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.internalPointer() == _block);
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
    Q_ASSERT(_model);
    Q_ASSERT(_index.isValid());
    Q_ASSERT(_index.model() == _model);
    Q_ASSERT(_block);
    Q_ASSERT(_index.internalPointer() == _block);
    _model->abortSet();
}


void Abstract::finishSet(
)
{
    Q_ASSERT(_model);
    Q_ASSERT(_index.isValid());
    Q_ASSERT(_index.model() == _model);
    Q_ASSERT(_block);
    Q_ASSERT(_index.internalPointer() == _block);
    _model->finishSet();
}


void Abstract::onBlockCodeChanged(
    const QMap<QString,QStringList>& value
)
{
    emit codeChanged(value);
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
