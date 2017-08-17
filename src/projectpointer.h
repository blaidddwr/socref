#ifndef PROJECTPOINTER_H
#define PROJECTPOINTER_H
#include <memory>
#include <QString>



class Project;



// @@ ProjectPointer
class ProjectPointer : public std::shared_ptr<Project>
{
public:
   ProjectPointer() = default;
   ProjectPointer(const QString& absolutePath, std::shared_ptr<Project> pointer);
   ~ProjectPointer();
   ProjectPointer(const ProjectPointer& object);
   ProjectPointer(ProjectPointer&& object);
   ProjectPointer& operator=(const ProjectPointer& object);
   ProjectPointer& operator=(ProjectPointer&& object);
private:
   QString _absolutePath;
};



#endif
