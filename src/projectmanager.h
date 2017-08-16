#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <QString>

#include "singleton.h"
#include "projectpointer.h"



// @@ ProjectManager
class ProjectManager : public Singleton<ProjectManager>
{
public:
   ProjectManager();
   ProjectManager(const ProjectManager&) = delete;
   ProjectPointer open(const QString& path);
   void close(const QString& absolutePath);
};



#endif
