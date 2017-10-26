#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QFileSystemWatcher>
#include <QDir>
#include <QCryptographicHash>

#include "project.h"
#include "abstractprojectfactory.h"
#include "abstractblockfactory.h"
#include "abstractblock.h"
#include "blockmodel.h"
#include "xmlelementparser.h"
#include "exception.h"






//@@
Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   // connect file watcher signal
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::handleFileChanged);

   // make sure project type is valid
   AbstractProjectFactory& factory {AbstractProjectFactory::getInstance()};
   if ( _type < 0 || _type >= factory.getSize() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invald project type %1 when max is %2.").arg(type).arg(factory.getSize()-1));
      throw e;
   }

   // get default file filters, make new root block
   _scanFilters = factory.getDefaultFilters(_type);
   _root = factory.getBlockFactory(_type).makeRootBlock();

   // make sure root block was created
   if ( !_root )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Expected pointer to new root block object when null was given."));
      throw e;
   }

   // initialize root block and create block model
   _root->setParent(this);
   _model = new BlockModel(_root,this);
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
      ,Root
   };

   // connect file watcher signal
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::handleFileChanged);

   // open project file for reading and make sure it worked
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for reading: %2").arg(_path).arg(file.errorString()));
      throw e;
   }

   // make xml stream reader
   QByteArray xmlBytes = file.readAll();
   QXmlStreamReader xml(xmlBytes);

   // initialize xml element parser
   XMLElementParser parser(xml,0,-1);
   parser.addKeyword("name",true).addKeyword("type",true).addKeyword("scandir",true)
         .addKeyword("filters",true).addKeyword("root",true);
   int element;

   // do loop until parser has reached end
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      // determine which element was found
      switch (element)
      {
      case Name:
         // get name value from xml
         _name = xml.readElementText();
         break;
      case Type:
         // read in type element
         readTypeElement(xml);
         break;
      case ScanDirectory:
         // get scan directory value from xml
         _scanDirectory = xml.readElementText();
         break;
      case ScanFilters:
         // get scan filters value from xml
         _scanFilters = xml.readElementText();
         break;
      case Root:
         // make sure type has been read in first
         if ( _type < 0 )
         {
            Exception::ReadError e;
            MARK_EXCEPTION(e);
            e.setDetails(tr("Cannot read in blocks until project type is read in."));
            throw e;
         }

         // make new root block
         _root = AbstractProjectFactory::getInstance().getBlockFactory(_type).makeRootBlock();

         // make sure root block was created and set parent
         if ( !_root )
         {
            Exception::InvalidUse e;
            MARK_EXCEPTION(e);
            e.setDetails(tr("Expected pointer to new root block object when null was given."));
            throw e;
         }
         _root->setParent(this);

         // read in block data and make new block model
         _root->read(xml);
         _model = new BlockModel(_root,this);
         break;
      }
   }

   // make sure all required information was read in
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Could not find all required xml elements of project."));
      throw e;
   }

   // compute new file hash and write to file
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(xmlBytes);
   _hash = hash.result();

   // add path to file watcher and emit saved signal
   addPath(_path);
   emit saved();
}






//@@
void Project::save()
{
   // make sure this is not a new project without a path
   if ( _path.isEmpty() )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to save new project that has no path."));
      throw e;
   }

   // open file for writing and truncation making sure it worked
   QFile xmlFile(_path);
   if ( !xmlFile.open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for writing: %2").arg(_path).arg(xmlFile.errorString()));
      throw e;
   }

   // initialize xml stream writer
   QByteArray xmlBytes;
   QXmlStreamWriter xml(&xmlBytes);
   xml.setAutoFormatting(true);

   // write beginning of xml
   xml.writeStartDocument();
   xml.writeStartElement("project");

   // write project name
   xml.writeTextElement("name",_name);

   // write project type information
   xml.writeStartElement("type");
   xml.writeTextElement("id",QString::number(_type));
   xml.writeTextElement("name",AbstractProjectFactory::getInstance().getName(_type));
   xml.writeEndElement();

   // write scan directory and filters
   QFileInfo info(_path);
   xml.writeTextElement("scandir",info.dir().relativeFilePath(_scanDirectory));
   xml.writeTextElement("filters",_scanFilters);

   // write out blocks starting with root
   xml.writeStartElement("root");
   _root->write(xml);
   xml.writeEndElement();

   // write end of document and check for xml errors
   xml.writeEndDocument();
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file %1.").arg(_path));
      throw e;
   }

   // compute new file hash and write to file
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(xmlBytes);
   _hash = hash.result();
   xmlFile.write(xmlBytes);

   // set modified to false and emit saved signal
   _modified = false;
   emit saved();
}






//@@
void Project::saveAs(const QString& path)
{
   // save old path, update to new path and try to save
   QString oldPath = _path;
   _path = path;
   try
   {
      save();
   }

   // if any exception occurs set path back to old and rethrow
   catch (...)
   {
      _path = oldPath;
      throw;
   }

   // everything worked so update file watcher
   removePath(oldPath);
   addPath(_path);
}






//@@
void Project::setName(const QString& name)
{
   // check if new name is different from old name
   if ( _name != name )
   {
      // set new name, emit name changed signal and signal modified
      _name = name;
      emit nameChanged();
      signalModified();
   }
}






//@@
void Project::setScanDirectory(const QString& path)
{
   // check if new directory is different from currently set
   QFileInfo current(_scanDirectory);
   QFileInfo info(path);
   if ( current.canonicalFilePath() != info.canonicalFilePath() )
   {
      // make sure path is a directory
      if ( !info.isDir() )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Attempting to set scan directory as '%1' which is not a directory.")
                      .arg(path));
         throw e;
      }

      // set new scan directory as canonical path and signal modified
      _scanDirectory = info.canonicalFilePath();
      signalModified();
   }
}






//@@
void Project::setScanFilters(const QString& filters)
{
   // check if new filters are different from currently set
   if ( _scanFilters != filters )
   {
      // update filters and signal modified
      _scanFilters = filters;
      signalModified();
   }
}






//@@
void Project::handleFileChanged()
{
   // attempt to open the project file
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      // if project file cannot be opened emit changed signal
      emit changed();
      return;
   }

   // get hash of project file
   QByteArray data = file.readAll();
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);

   // if hashes are different emit changed signal
   if ( hash.result() != _hash )
   {
      emit changed();
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

   // get project factory and initialize parser
   AbstractProjectFactory& factory {AbstractProjectFactory::getInstance()};
   XMLElementParser parser(xml);
   parser.addKeyword("id",true).addKeyword("name",true);
   int element;

   // parse xml until end of type element is reached
   while ( ( element = parser() ) != XMLElementParser::End )
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
               e.setDetails(tr("Failed reading in type as integer."));
               throw e;
            }

            // make sure project type is valid and get its block factory
            if ( _type < 0 || _type >= factory.getSize() )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(_type)
                            .arg(factory.getSize()-1));
               throw e;
            }
            break;
         }
      case Name:
         {
            // read in type name and make sure project type name matches factory
            QString typeName = xml.readElementText();
            if ( typeName != factory.getName(_type) )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Read in invalid type name %1 when it should be %2.").arg(typeName)
                            .arg(factory.getName(_type)));
               throw e;
            }
            break;
         }
      }
   }

   // make sure all elements were read in
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements for project type."));
      throw e;
   }
}






//@@
void Project::signalModified()
{
   // make sure project is not already modified
   if ( !_modified )
   {
      // set project to modified and emit signal
      _modified = true;
      emit modified();
   }
}
