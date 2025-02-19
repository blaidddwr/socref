#include "CommandProjectInsert.h"
#include "AbstractBlock.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Insert::Insert(
    AbstractBlock* block
    ,int row
    ,const QModelIndex& parentIndex
    ,Model::Project* parent
):
    Insert(row,parentIndex,parent)
{
    _block = block;
    Q_ASSERT(_block);
    Q_ASSERT(_row >= 0);
    Q_ASSERT(_row <= project().rowCount(parentIndex));
    _block->setParent(this);
    _description = tr("Inserting block at row %3.").arg(row);
}


QString Insert::description(
) const
{
    return _description;
}


bool Insert::redo(
)
{
    return insert();
}


bool Insert::undo(
)
{
    return remove();
}


Insert::Insert(
    int row
    ,const QModelIndex& parentIndex
    ,Model::Project* parent
):
    AbstractProject(parent)
    ,_parent(convertIndexToList(parentIndex))
    ,_row(row)
{
    Q_ASSERT(_row >= 0);
    Q_ASSERT(_row <= project().rowCount(parentIndex));
}


bool Insert::insert(
)
{
    if (!_block)
    {
        return false;
    }
    auto parent = convertListToIndex(_parent);
    project().beginInsertRows(parent,_row,_row);
    project().block(parent)->insert(_row,_block);
    auto block = _block;
    connect(
        _block
        ,&AbstractBlock::displayIconChanged
        ,&project()
        ,[this,block] ()
        {
            project().onBlockDisplayIconChanged(block);
        }
        );
    connect(
        _block
        ,&AbstractBlock::displayTextChanged
        ,&project()
        ,[this,block] ()
        {
            project().onBlockDisplayTextChanged(block);
        }
        );
    _block = nullptr;
    project().endInsertRows();
    project().setModified(true);
    return true;
}


bool Insert::remove(
)
{
    if (_block)
    {
        return false;
    }
    auto parent = convertListToIndex(_parent);
    project().beginRemoveRows(parent,_row,_row);
    _block = project().block(parent)->take(_row);
    disconnect(_block,&AbstractBlock::displayIconChanged,&project(),nullptr);
    disconnect(_block,&AbstractBlock::displayTextChanged,&project(),nullptr);
    _block->setParent(this);
    project().endRemoveRows();
    project().setModified(true);
    return true;
}


void Insert::setDescription(
    const QString& value
)
{
    _description = value;
}
}
}
