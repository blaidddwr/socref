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
    Abstract(parent)
    ,_block(block)
    ,_parent(convertIndexToList(parentIndex))
    ,_row(row)
{
    G_ASSERT(_block);
    G_ASSERT(_row >= 0);
    G_ASSERT(_row <= project().rowCount(parentIndex));
    _block->setParent(this);
    _blockScope = _block->scope();
    _parentScope = blockFromIndex(parentIndex)->scope();
}


QString Insert::description(
) const
{
    auto ret = tr("Inserting block %1 into block %2 at row %3.");
    return ret.arg(_blockScope,_parentScope).arg(_row);
}


bool Insert::redo(
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


bool Insert::undo(
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
}
}
