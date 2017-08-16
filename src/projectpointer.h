#ifndef PROJECTPOINTER_H
#define PROJECTPOINTER_H
#include <memory>
#include <QString>



class AbstractProject;



// @@ ProjectPointer
class ProjectPointer : public std::shared_ptr<AbstractProject>
{
public:
   ProjectPointer();
   ProjectPointer(const QString& absolutePath, AbstractProject* pointer);
   ~ProjectPointer();
   ProjectPointer(const ProjectPointer& object);
   ProjectPointer(ProjectPointer&& object);
   ProjectPointer& operator=(const ProjectPointer& object);
   ProjectPointer& operator=(ProjectPointer&& object);
private:
   QString _absolutePath;
};



#endif
