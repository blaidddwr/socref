#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QFileSystemWatcher>

#include "project.h"
#include "abstractprojectfactory.h"
#include "abstractblockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"






//@@
Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   // make sure project type is valid
   AbstractProjectFactory& factory {AbstractProjectFactory::getInstance()};
   if ( _type < 0 || _type >= factory.getSize() )
   {
      // report invalid project type as exception
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Domain Error"));
      e.setDetails(tr("Invald project type given as argument."));
      throw e;
   }

   // set block factory and scan filters
   _factory = &factory.getBlockFactory(_type);
   _scanFilters = factory.getDefaultFilters(_type);
}






//@@
Project::Project(const QString &path):
   _path(path)
{
   // define enumeration for different elements
   enum
   {
      Name = 0
      ,Type
      ,ScanDirectory
      ,ScanFilters
   };

   // open project file for reading
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      // report file open error as exception
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Open Error"));
      e.setDetails(tr("Failed opening project file for reading."));
      throw e;
   }

   // make xml stream reader and initialize validators
   QXmlStreamReader xml(&file);
   bool nameRead {false};
   bool typeRead {false};
   bool scanDirectoryRead {false};
   bool scanFiltersRead {false};

   // initialize xml element parser
   XMLElementParser parse(xml,0,-1);
   parse.addKeyword("name").addKeyword("type");
   int element;

   // do loop until parser has reached end
   while ( ( element = parse() ) != -1 )
   {
      // determine which element was found
      switch (element)
      {
      case Name:
         // get name value from xml and mark as read
         _name = xml.readElementText();
         nameRead = true;
         break;
      case Type:
         // read in type element and mark as read
         readTypeElement(xml);
         typeRead = true;
         break;
      case ScanDirectory:
         // get scan directory value from xml and mark as read
         _scanDirectory = xml.readElementText();
         scanDirectoryRead = true;
         break;
      case ScanFilters:
         // get scan filters value from xml and mark as read
         _scanFilters = xml.readElementText();
         scanFiltersRead = true;
         break;
      }
   }

   // make sure all required information was read in
   if ( !nameRead || !typeRead || !scanDirectoryRead || !scanFiltersRead )
   {
      // report read error as exception
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Invalid project file; could not find all required xml elements of"
                      " project."));
      throw e;
   }

   // make sure project type is valid and get its block factory
   AbstractProjectFactory& factory {AbstractProjectFactory::getInstance()};
   if ( _type < 0 || _type >= factory.getSize() )
   {
      // report invalid project type as exception
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Invalid project file; type is invalid."));
      throw e;
   }
   _factory = &factory.getBlockFactory(_type);

   // Add file watcher and set last modified timestamp
   _fileWatcher = new QFileSystemWatcher(QStringList(_path),this);
   connect(_fileWatcher,&QFileSystemWatcher::fileChanged,this,&Project::saveFileChanged);
   QFileInfo info(_path);
   _fileLastModified = info.lastModified();
}






//@@
void Project::save() const
{
}






//@@
void Project::saveAs()
{
}






//@@
void Project::setScanDirectory(const QString& path)
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






//@@
void Project::readTypeElement(QXmlStreamReader& xml)
{
}
