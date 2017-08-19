#include <QDir>

#include "project.h"
#include "abstractprojectfactory.h"
#include "abstractblockfactory.h"






//@@
Project::Project(int type):
   _type(type),
   _factory(&AbstractProjectFactory::getInstance().getBlockFactory(_type)),
   _scanDirectory("."),
   _scanFilters(AbstractProjectFactory::getInstance().getDefaultFilters(_type))
{}






//@@
Project::Project(const QString& path)
{
}






//@@
bool Project::setPath(const QString& path)
{
}






//@@
void Project::reload()
{
}






//@@
void Project::save() const
{
}






//@@
void Project::setName(const QString& name)
{
}






//@@
QString Project::getName() const
{
}






//@@
int Project::getType() const
{
}






//@@
QString Project::getScanDirectory() const
{
}






//@@
void Project::setScanDirectory(const QString& path)
{
}






//@@
QString Project::getScanFilters() const
{
}






//@@
void Project::setScanFilters(const QString& filters)
{
}






//@@
BlockModel* Project::getModel() const
{
}






//@@
bool Project::isNew() const
{
}






//@@
bool Project::isModified() const
{
}






//@@
void Project::blockModified()
{
   // make sure project is not already modified
   if ( !_modified )
   {
      // set project to modified and emit signal
      _modified = true;
      emit modified();
   }
}






//@@
void Project::saveFileChanged()
{
   // check if last modified is not equal to one saved
   QFileInfo info(_path);
   if ( _fileLastModified != info.lastModified() )
   {
      // update last modified time and emit file changed signal
      _fileLastModified = info.lastModified();
      emit fileChanged();
   }
}
