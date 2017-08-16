#include "projectpointer.h"






ProjectPointer::ProjectPointer(const ProjectPointer& object):
   std::shared_ptr<AbstractProject>(object),
   _absolutePath(object._absolutePath)
{}






ProjectPointer::ProjectPointer(ProjectPointer&& object):
   std::shared_ptr<AbstractProject>(std::move(object)),
   _absolutePath(object._absolutePath)
{
   object._absolutePath.clear();
}






ProjectPointer& ProjectPointer::operator=(ProjectPointer&& object)
{
   _absolutePath = object._absolutePath;
   object._absolutePath.clear();
   std::shared_ptr<AbstractProject>::operator =(std::move(object));
   return *this;
}






ProjectPointer& ProjectPointer::operator=(const ProjectPointer& object)
{
   _absolutePath = object._absolutePath;
   std::shared_ptr<AbstractProject>::operator =(object);
   return *this;
}
