#ifndef PROJECTREGISTRY_H
#define PROJECTREGISTRY_H
#include <QObject>
#include <QMap>

#include "singleton.h"



class Project;



class ProjectRegistry : public QObject, public Singleton<ProjectRegistry>
{
   Q_OBJECT
public:
   ProjectRegistry() = default;
   ProjectRegistry(const ProjectRegistry&) = delete;
   bool exists(const QString& path) const;
   void add(const QString& path, Project* pointer);
   bool remove(const QString& path);
signals:
   void projectOverwritten(Project* newProject);
private:
   QMap<QString,Project*> _projects;
};



#endif
