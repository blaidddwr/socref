#include "AbstractBlockWidget.h"
#include "AbstractBlock.h"
#include "ModelProject.h"
#include "WidgetDialogCode.h"


AbstractBlockWidget::AbstractBlockWidget(
    const AbstractBlock* block
    ,QWidget* parent
):
    QWidget(parent)
    ,_block(block)
{
    Q_ASSERT(_block);
    connect(
        const_cast<AbstractBlock*>(_block)
        ,&QObject::destroyed
        ,this
        ,&AbstractBlockWidget::onBlockDestroyed
        );
    connect(
        const_cast<AbstractBlock*>(_block)
        ,&AbstractBlock::codeChanged
        ,this
        ,&AbstractBlockWidget::onBlockCodeChanged
        );
}


void AbstractBlockWidget::showCode(
)
{
    Q_ASSERT(_block);
    if (!_block->code().isEmpty())
    {
        Widget::Dialog::Code(_block,this).exec();
    }
}


bool AbstractBlockWidget::modified(
) const
{
    return _modified;
}


void AbstractBlockWidget::setIndex(
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


void AbstractBlockWidget::setModel(
    Model::Project* model
)
{
    if (_model)
    {
        disconnect(_model,&QObject::destroyed,this,&AbstractBlockWidget::onModelDestroyed);
    }
    _model = model;
    if (_model)
    {
        connect(_model,&QObject::destroyed,this,&AbstractBlockWidget::onModelDestroyed);
    }
}


void AbstractBlockWidget::setModified(
    bool value
)
{
    if (_modified != value)
    {
        _modified = value;
        emit modifiedChanged(value);
    }
}


void AbstractBlockWidget::touch(
)
{
    setModified(true);
}


void AbstractBlockWidget::abortSet(
)
{
    Q_ASSERT(_model);
    Q_ASSERT(_index.isValid());
    Q_ASSERT(_index.model() == _model);
    Q_ASSERT(_block);
    Q_ASSERT(_index.internalPointer() == _block);
    _model->abortSet();
}


AbstractBlock* AbstractBlockWidget::beginSet(
)
{
    Q_ASSERT(_model);
    Q_ASSERT(_index.isValid());
    Q_ASSERT(_index.model() == _model);
    Q_ASSERT(_block);
    Q_ASSERT(_index.internalPointer() == _block);
    return _model->beginSet(_index);
}


const AbstractBlock* AbstractBlockWidget::block(
) const
{
    return _block;
}


void AbstractBlockWidget::finishSet(
)
{
    Q_ASSERT(_model);
    Q_ASSERT(_index.isValid());
    Q_ASSERT(_index.model() == _model);
    Q_ASSERT(_block);
    Q_ASSERT(_index.internalPointer() == _block);
    _model->finishSet();
}


void AbstractBlockWidget::onBlockCodeChanged(
    const QMap<QString,QStringList>& value
)
{
    emit codeChanged(value);
}


void AbstractBlockWidget::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        _block = nullptr;
    }
}


void AbstractBlockWidget::onModelDestroyed(
    QObject* object
)
{
    if (_model == object)
    {
        _model = nullptr;
    }
}
