#include <QFileInfo>

#include "projectmanager.h"
#include "abstractprojectfactory.h"






// @@ ProjectManager::getProject(const QString&)
ProjectPointer ProjectManager::getProject(const QString& path)
{
   QFileInfo info(path);
   QString absolutePath = info.absoluteFilePath();
   auto i = _projects.constFind(absolutePath);
   if ( i != _projects.constEnd() )
   {
      return ProjectPointer(absolutePath,*i);
   }
   std::shared_ptr<Project> newProject {new Project(path)};
   _projects.insert(absolutePath,newProject);
   return ProjectPointer(absolutePath,newProject);
}
