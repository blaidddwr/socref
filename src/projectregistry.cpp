#include <QFileInfo>

#include "projectregistry.h"






// @@ ProjectRegistry::exists(const QString&) const
bool ProjectRegistry::exists(const QString& path) const
{
   QFileInfo info(path);
   if ( !info.exists() )
   {
       return false;
   }
   return _projects.contains(info.absoluteFilePath());
}






// @@ ProjectRegistry::get(const QString&) const
Project* ProjectRegistry::get(const QString& path) const
{
   QFileInfo info(path);
   if ( !info.exists() )
   {
       return nullptr;
   }
   auto i = _projects.constFind(info.absoluteFilePath());
   if ( i == _projects.constEnd() )
   {
      return nullptr;
   }
   return *i;
}






// @@ ProjectRegistry::add(const QString&,Project*)
void ProjectRegistry::add(const QString& path, Project* pointer)
{
   QFileInfo info(path);
   if ( !info.exists() )
   {
       return;
   }
   auto i = _projects.find(info.absoluteFilePath());
   if ( i != _projects.end() )
   {
      emit projectOverwritten(*i,pointer);
      *i = pointer;
      return;
   }
   _projects.insert(info.absoluteFilePath(),pointer);
}






// @@ ProjectRegistry::remove(const QString&)
bool ProjectRegistry::remove(const QString& path)
{
   QFileInfo info(path);
   if ( !info.exists() )
   {
       return false;
   }
   return _projects.remove(info.absoluteFilePath()) > 0;
}
