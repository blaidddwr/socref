#include "CommandProjectSet.h"
#include "BlockAbstract.h"
#include "Exceptions.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Set::Set(
    const QHash<QString,QVariant>& previousState
    ,const QHash<QString,QVariant>& nextState
    ,const QModelIndex& index
    ,Model::Project* parent
):
    Abstract(parent)
    ,_nextState(nextState)
    ,_previousState(previousState)
    ,_location(convertIndexToList(index))
{
    G_ASSERT(index.isValid());
    G_ASSERT(project().block(index));
    auto blockScope = project().block(index)->scope();
    _description = tr("Set state of block %1.").arg(blockScope);
}


QString Set::description(
) const
{
    return _description;
}


bool Set::redo(
)
{
    auto index = convertListToIndex(_location);
    project().block(index)->setState(_nextState);
    emit project().dataChanged(index,index,{Qt::DisplayRole});
    return true;
}


bool Set::undo(
)
{
    auto index = convertListToIndex(_location);
    project().block(index)->setState(_previousState);
    emit project().dataChanged(index,index,{Qt::DisplayRole});
    return true;
}
}
}
