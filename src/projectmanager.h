#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <QString>
#include <memory>
#include <QMap>

#include "singleton.h"
#include "projectpointer.h"
#include "project.h"



// @@ ProjectManager
class ProjectManager : public Singleton<ProjectManager>
{
public:
   ProjectManager() = default;
   ProjectManager(const ProjectManager&) = delete;
   ProjectPointer getProject(const QString& path);
   bool hasProject(const QString& absolutePath) const;
   void close(const QString& absolutePath);
private:
   QMap<QString,std::shared_ptr<Project>> _projects;
};






// @@ ProjectManager::hasProject(const QString&)
inline bool ProjectManager::hasProject(const QString& absolutePath) const
{
   return _projects.contains(absolutePath);
}






// @@ ProjectManager::close(const QString&)
inline void ProjectManager::close(const QString& absolutePath)
{
   _projects.remove(absolutePath);
}



#endif
