#include "CommandProjectMove.h"
#include "BlockAbstract.h"
#include "Exceptions.h"
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
    _description = tr("Moving child block at row %1 to row %2.").arg(_from,_to);
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
    auto pb = project().block(parent);
    if (
        from >= pb->size()
        || to >= pb->size()
        )
    {
        return false;
    }
    auto qtTo = to;
    if (qtTo > from)
    {
        qtTo++;
    }
    project().beginMoveRows(parent,from,from,parent,qtTo);
    pb->move(from,to);
    project().endMoveRows();
    return true;
}
}
}
