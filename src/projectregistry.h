#ifndef PROJECTREGISTRY_H
#define PROJECTREGISTRY_H
#include <QObject>
#include <QMap>

#include "singleton.h"



class Project;



// @@ ProjectRegistry
class ProjectRegistry : public QObject, public Singleton<ProjectRegistry>
{
   Q_OBJECT
public:
   ProjectRegistry() = default;
   bool exists(const QString& path) const;
   Project* get(const QString& path) const;
   void add(const QString& path, Project* pointer);
   bool remove(const QString& path);
signals:
   // @@ ProjectRegistry::projectOverwitten(Project*,project*)
   void projectOverwritten(Project* oldProject, Project* newProject);
private:
   QMap<QString,Project*> _projects;
};



#endif
