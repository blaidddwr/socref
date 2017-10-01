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
      // report invalid project type
      Exception::DomainError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Domain Error"));
      e.setDetails(tr("Invald project type %1 when max is %2.").arg(type).arg(factory.getSize()-1));
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
      // report file open error
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Open Error"));
      e.setDetails(tr("Failed opening project file for reading: %1").arg(file.errorString()));
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
      // report read error
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Read Error"));
      e.setDetails(tr("Could not find all required xml elements of project."));
      throw e;
   }

   // make sure project type is valid and get its block factory
   AbstractProjectFactory& factory {AbstractProjectFactory::getInstance()};
   if ( _type < 0 || _type >= factory.getSize() )
   {
      // report invalid project type
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Read Error"));
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(_type)
                   .arg(factory.getSize()-1));
      throw e;
   }

   // make sure project type name matches factory
   if ( _typeName != factory.getName(_type) )
   {
      // report invalid project type name
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Read Error"));
      e.setDetails(tr("Read in invalid type name %1 when it should be %2.").arg(_typeName)
                   .arg(factory.getName(_type)));
      throw e;
   }

   // make new factory and add file watcher
   _factory = &factory.getBlockFactory(_type);
   _fileWatcher = new QFileSystemWatcher(QStringList(_path),this);
   connect(_fileWatcher,&QFileSystemWatcher::fileChanged,this,&Project::saveFileChanged);
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
void Project::readTypeElement(QXmlStreamReader& xml)
{
   // enumeration for different elements
   enum
   {
      Id = 0
      ,Name
   };

   // initialize element read markers
   bool idRead {false};
   bool nameRead {false};

   // initialize parser
   XMLElementParser parser(xml);
   parser.addKeyword("id").addKeyword("name");
   int element;

   // parse xml until end of type element is reached
   while ( ( element = parser() ) != -1 )
   {
      // figure out which element is found
      switch (element)
      {
      case Id:
         {
            // read in id
            bool ok;
            _type = xml.readElementText().toInt(&ok);

            // make sure read worked
            if ( !ok )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setTitle(tr("Project Read Error"));
               e.setDetails(tr("Failed reading in type as integer."));
               throw e;
            }

            // mark id as read
            idRead = true;
            break;
         }
      case Name:
         // read in type name and mark as read
         _typeName = xml.readElementText();
         nameRead = true;
         break;
      }
   }

   // make sure all elements were read in
   if ( !idRead || !nameRead )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Project Read Error"));
      e.setDetails(tr("Failed reading in all required elements for project type."));
      throw e;
   }
}
