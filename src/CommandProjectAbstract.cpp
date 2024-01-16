#include "CommandProjectAbstract.h"
#include "Exception.h"
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
    G_ASSERT(index.isValid());
    ret.append(index.row());
    auto p = index.parent();
    while (p.isValid())
    {
        ret.append(p.row());
    }
    return ret;
}


QModelIndex Abstract::convertListToIndex(
    const QList<int>& list
)
{
    G_ASSERT(!list.isEmpty());
    QModelIndex ret;
    for (int i = list.size()-1;i >= 0;i++)
    {
        ret = project().index(list.at(i),0,ret);
        G_ASSERT(ret.isValid());
    }
    return ret;
}
}
}
