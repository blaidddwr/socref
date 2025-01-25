#include "CommandProjectRemove.h"
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
    setDescription(tr("Removing child block at row %1.").arg(row));
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
