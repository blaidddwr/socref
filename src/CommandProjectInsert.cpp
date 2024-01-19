#include "CommandProjectInsert.h"
#include "BlockAbstract.h"
#include "Exception.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Insert::Insert(
    Block::Abstract* block
    ,int row
    ,const QModelIndex& parentIndex
    ,Model::Project* parent
):
    Insert(row,parentIndex,parent)
{
    _block = block;
    G_ASSERT(_block);
    G_ASSERT(_row >= 0);
    G_ASSERT(_row <= project().rowCount(parentIndex));
    _block->setParent(this);
    auto blockScope = _block->scope();
    auto parentScope = blockFromIndex(parentIndex)->scope();
    _description = tr("Inserting block %1 into block %2 at row %3.");
    _description = _description.arg(blockScope,parentScope);
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
    Abstract(parent)
    ,_parent(convertIndexToList(parentIndex))
    ,_row(row)
{
    G_ASSERT(_row >= 0);
    G_ASSERT(_row <= project().rowCount(parentIndex));
}


bool Insert::insert(
)
{
    if (!_block)
    {
        return false;
    }
    auto parent = convertListToIndex(_parent);
    project().beginInsertColumns(parent,_row,_row);
    blockFromIndex(parent)->insert(_row,_block);
    _block = nullptr;
    project().endInsertColumns();
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
    _block = blockFromIndex(parent)->take(_row);
    _block->setParent(this);
    project().endRemoveRows();
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
