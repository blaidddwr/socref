#include "CommandProjectRemove.h"
#include "BlockAbstract.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Remove::Remove(
    int row
    ,const QModelIndex& parentIndex
    ,Model::Project* parent
):
    Insert(row,parentIndex,parent)
{
    auto parentScope = project().block(parentIndex)->scope();
    auto description = tr("Removing child block at row %1 from block %2.");
    setDescription(description.arg(row).arg(parentScope));
}


bool Remove::redo(
)
{
    return remove();
}


bool Remove::undo(
)
{
    return insert();
}
}
}
