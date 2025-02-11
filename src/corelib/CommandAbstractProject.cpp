#include "CommandAbstractProject.h"
#include "ModelProject.h"
namespace Command {


AbstractProject::AbstractProject(
    Model::Project* parent
):
    QObject(parent)
{
    Q_ASSERT(parent);
}


Model::Project& AbstractProject::project(
) const
{
    auto ret = qobject_cast<Model::Project*>(parent());
    Q_ASSERT(ret);
    return *ret;
}


QList<int> AbstractProject::convertIndexToList(
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


QModelIndex AbstractProject::convertListToIndex(
    const QList<int>& list
)
{
    QModelIndex ret;
    for (int i = list.size()-1;i >= 0;i--)
    {
        ret = project().index(list.at(i),0,ret);
    }
    return ret;
}
}
