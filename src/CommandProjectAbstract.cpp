#include "CommandProjectAbstract.h"
#include "Exceptions.h"
#include "ModelProject.h"
namespace Command {
namespace Project {


Abstract::Abstract(
    Model::Project* parent
):
    QObject(parent)
{
    G_ASSERT(parent);
}


Model::Project& Abstract::project(
) const
{
    auto ret = qobject_cast<Model::Project*>(parent());
    G_ASSERT(ret);
    return *ret;
}


QList<int> Abstract::convertIndexToList(
    const QModelIndex& index
)
{
    QList<int> ret;
    auto p = index;
    while (p.isValid())
    {
        ret.append(p.row());
        p = p.parent();
    }
    return ret;
}


QModelIndex Abstract::convertListToIndex(
    const QList<int>& list
)
{
    QModelIndex ret;
    for (int i = list.size()-1;i >= 0;i++)
    {
        ret = project().index(list.at(i),0,ret);
    }
    return ret;
}
}
}
