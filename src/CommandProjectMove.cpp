#include "CommandProjectMove.h"
#include "BlockAbstract.h"
#include "Exception.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Move::Move(
    int from
    ,int to
    ,const QModelIndex& parentIndex
    ,Model::Project* parent
):
    Abstract(parent)
    ,_from(from)
    ,_to(to)
{
    G_ASSERT(from >= 0);
    G_ASSERT(to >= 0);
    G_ASSERT(from != to);
    auto rc = project().rowCount(parentIndex);
    G_ASSERT(from <= rc);
    G_ASSERT(to <= rc);
    auto parentScope = blockFromIndex(parentIndex)->scope();
    _description = tr("Moving child block at %1 to %2 in parent block %3.");
    _description = _description.arg(_from,_to).arg(parentScope);
}


QString Move::description(
) const
{
    return _description;
}


bool Move::redo(
)
{
    return move(_from,_to);
}


bool Move::undo(
)
{
    return move(_to,_from);
}


bool Move::move(
    int from
    ,int to
)
{
    auto parent = convertListToIndex(_parent);
    auto pb = blockFromIndex(parent);
    if (
        from >= pb->size()
        || to >= pb->size()
        )
    {
        return false;
    }
    if (to > from)
    {
        to++;
    }
    project().beginMoveRows(parent,from,from,parent,to);
    return true;
}
}
}
