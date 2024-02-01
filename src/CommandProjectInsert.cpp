#include "CommandProjectInsert.h"
#include "BlockAbstract.h"
#include "Exceptions.h"
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
    auto parentScope = project().block(parentIndex)->scope();
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
    project().beginInsertRows(parent,_row,_row);
    project().block(parent)->insert(_row,_block);
    auto p = &project();
    auto b = _block;
    connect(b,&Block::Abstract::displayIconChanged,p,[p,b](){ p->onBlockDisplayIconChanged(b); });
    connect(b,&Block::Abstract::displayTextChanged,p,[p,b](){ p->onBlockDisplayTextChanged(b); });
    _block = nullptr;
    project().endInsertRows();
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
    disconnect(_block,&Block::Abstract::displayIconChanged,&project(),nullptr);
    disconnect(_block,&Block::Abstract::displayTextChanged,&project(),nullptr);
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
