#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QFileSystemWatcher>
#include <QDir>
#include <QCryptographicHash>

#include "project.h"
#include "abstractprojectfactory.h"
#include "abstractblockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"






Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::handleFileChanged);
   createRoot();
}






Project::Project(const QString &path):
   _path(path)
{
   enum
   {
      Name = 0
      ,Type
      ,ScanDirectory
      ,ScanFilters
      ,Root
   };
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::handleFileChanged);
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for reading: %2").arg(_path).arg(file.errorString()));
      throw e;
   }
   QByteArray xmlBytes = file.readAll();
   QXmlStreamReader xml(xmlBytes);
   XMLElementParser parser(xml,0,-1);
   parser.addKeyword("name",true).addKeyword("type",true).addKeyword("scandir",true)
         .addKeyword("filters",true).addKeyword("root",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Name:
         _name = xml.readElementText();
         break;
      case Type:
         readTypeElement(xml);
         break;
      case ScanDirectory:
         _scanDirectory = xml.readElementText();
         break;
      case ScanFilters:
         _scanFilters = xml.readElementText();
         break;
      case Root:
         createRoot();
         _root->read(xml);
         break;
      }
   }
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Could not find all required xml elements of project."));
      throw e;
   }
   setFileHash(xmlBytes);
   addPath(_path);
   emit saved();
}






void Project::save()
{
   if ( _path.isEmpty() )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to save new project that has no path."));
      throw e;
   }
   QFile xmlFile(_path);
   if ( !xmlFile.open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for writing: %2").arg(_path).arg(xmlFile.errorString()));
      throw e;
   }
   QByteArray xmlBytes;
   QXmlStreamWriter xml(&xmlBytes);
   xml.setAutoFormatting(true);
   xml.writeStartDocument();
   xml.writeStartElement("project");
   xml.writeTextElement("name",_name);
   xml.writeStartElement("type");
   xml.writeTextElement("id",QString::number(_type));
   xml.writeTextElement("name",AbstractProjectFactory::instance().name(_type));
   xml.writeEndElement();
   QFileInfo info(_path);
   xml.writeTextElement("scandir",info.dir().relativeFilePath(_scanDirectory));
   xml.writeTextElement("filters",_scanFilters);
   xml.writeStartElement("root");
   _root->write(xml);
   xml.writeEndElement();
   xml.writeEndDocument();
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file %1.").arg(_path));
      throw e;
   }
   setFileHash(xmlBytes);
   xmlFile.write(xmlBytes);
   _modified = false;
   emit saved();
}






void Project::saveAs(const QString& path)
{
   QString oldPath = _path;
   _path = path;
   try
   {
      save();
   }
   catch (...)
   {
      _path = oldPath;
      throw;
   }
   if ( !oldPath.isEmpty() )
   {
      removePath(oldPath);
   }
   addPath(_path);
}






void Project::setName(const QString& name)
{
   if ( _name != name )
   {
      _name = name;
      emit nameChanged();
      signalModified();
   }
}






void Project::setScanDirectory(const QString& path)
{
   QFileInfo current(_scanDirectory);
   QFileInfo info(path);
   if ( current.canonicalFilePath() != info.canonicalFilePath() )
   {
      if ( !info.isDir() )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Attempting to set scan directory as '%1' which is not a directory.").arg(path));
         throw e;
      }
      _scanDirectory = info.canonicalFilePath();
      signalModified();
   }
}






void Project::setScanFilters(const QString& filters)
{
   if ( _scanFilters != filters )
   {
      _scanFilters = filters;
      signalModified();
   }
}






void Project::handleFileChanged()
{
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      return;
   }
   QByteArray data = file.readAll();
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);
   if ( hash.result() != _hash )
   {
      emit changed();
   }
}






void Project::readTypeElement(QXmlStreamReader& xml)
{
   enum
   {
      Id = 0
      ,Name
   };
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   XMLElementParser parser(xml);
   parser.addKeyword("id",true).addKeyword("name",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Id:
         {
            bool ok;
            _type = xml.readElementText().toInt(&ok);
            if ( !ok )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Failed reading in type as integer."));
               throw e;
            }
            if ( _type < 0 || _type >= factory.size() )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(_type)
                            .arg(factory.size()-1));
               throw e;
            }
            break;
         }
      case Name:
         {
            QString typeName = xml.readElementText();
            if ( typeName != factory.name(_type) )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Read in invalid type name %1 when it should be %2.").arg(typeName)
                            .arg(factory.name(_type)));
               throw e;
            }
            break;
         }
      }
   }
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements for project type."));
      throw e;
   }
}






void Project::signalModified()
{
   if ( !_modified )
   {
      _modified = true;
      emit modified();
   }
}






void Project::setFileHash(const QByteArray& bytes)
{
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(bytes);
   _hash = hash.result();
}






void Project::createRoot()
{
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   if ( _type < 0 || _type >= factory.size() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invald project type %1 when max is %2.").arg(_type).arg(factory.size() - 1));
      throw e;
   }
   _root = factory.blockFactory(_type).makeRootBlock().release();
   if ( !_root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Expected pointer to new root block object when null was given."));
      throw e;
   }
   _root->setParent(this);
   connect(_root,&AbstractBlock::modified,this,&Project::blockModified);
   _model = new BlockModel(_root,this);
}
