#include "CommandProjectSet.h"
#include "BlockAbstract.h"
#include "Exception.h"
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
}


bool Set::redo(
)
{
    project().block(convertListToIndex(_location))->setState(_nextState);
    return true;
}


bool Set::undo(
)
{
    project().block(convertListToIndex(_location))->setState(_previousState);
    return true;
}
}
}
