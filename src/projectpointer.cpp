#include "projectpointer.h"
#include "projectmanager.h"

#define MIN_USE_COUNT 2






// @@ ProjectPointer::ProjectPointer(const QString&, std::shared_ptr<Project>)
ProjectPointer::ProjectPointer(const QString& absolutePath
                               , std::shared_ptr<Project> pointer):
   std::shared_ptr<Project>(pointer),
   _absolutePath(absolutePath)
{}






// @@ ProjectPointer::~ProjectPointer()
ProjectPointer::~ProjectPointer()
{
   if ( use_count() <= MIN_USE_COUNT )
   {
      ProjectManager::getInstance().close(_absolutePath);
   }
}






ProjectPointer::ProjectPointer(const ProjectPointer& object):
   std::shared_ptr<Project>(object),
   _absolutePath(object._absolutePath)
{}






ProjectPointer::ProjectPointer(ProjectPointer&& object):
   std::shared_ptr<Project>(std::move(object)),
   _absolutePath(object._absolutePath)
{
   object._absolutePath.clear();
}






ProjectPointer& ProjectPointer::operator=(ProjectPointer&& object)
{
   _absolutePath = object._absolutePath;
   object._absolutePath.clear();
   std::shared_ptr<Project>::operator =(std::move(object));
   return *this;
}






ProjectPointer& ProjectPointer::operator=(const ProjectPointer& object)
{
   _absolutePath = object._absolutePath;
   std::shared_ptr<Project>::operator =(object);
   return *this;
}
