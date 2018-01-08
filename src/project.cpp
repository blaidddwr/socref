#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QCryptographicHash>
#include <QDomDocument>

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
   _scanFilters = AbstractProjectFactory::instance().defaultFilters(_type);
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
      ,Total
   };
   QStringList tags {"name","type","scandir","filters","root"};
   QVector<bool> readTags(5,false);
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
   QDomDocument document;
   document.setContent(xmlBytes);
   QDomNode node = document.documentElement().firstChild();
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (tags.indexOf(element.tagName()))
         {
         case Name:
            _name = element.text();
            readTags[Name] = true;
            break;
         case Type:
            readTypeElement(element);
            readTags[Type] = true;
            break;
         case ScanDirectory:
            _scanDirectory = element.text();
            readTags[ScanDirectory] = true;
            break;
         case ScanFilters:
            _scanFilters = element.text();
            readTags[ScanFilters] = true;
            break;
         case Root:
            createRoot();
            _root->read(element);
            readTags[Root] = true;
            break;
         }
      }
      node = node.nextSibling();
   }
   if ( readTags.contains(false) )
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






Project& Project::save()
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
   QDomDocument document;
   document.appendChild(document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));
   QDomElement project {document.createElement("project")};
   QDomElement name {document.createElement("name")};
   QDomElement type {document.createElement("type")};
   QDomElement scandir {document.createElement("scandir")};
   QDomElement filters {document.createElement("filters")};
   QDomElement root {_root->write(document)};
   name.appendChild(document.createTextNode(_name));
   type.setAttribute("id",QString::number(_type));
   type.appendChild(document.createTextNode(AbstractProjectFactory::instance().name(_type)));
   scandir.appendChild(document.createTextNode(QFileInfo(_path).dir().relativeFilePath(_scanDirectory)));
   filters.appendChild(document.createTextNode(_scanFilters));
   root.setTagName("root");
   project.appendChild(name);
   project.appendChild(type);
   project.appendChild(scandir);
   project.appendChild(filters);
   project.appendChild(root);
   document.appendChild(project);
   QByteArray xmlBytes {document.toByteArray(3)};
   if ( xmlFile.write(xmlBytes) == -1 )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Error writing to file %1.").arg(_path));
      throw e;
   }
   setFileHash(xmlBytes);
   _modified = false;
   emit saved();
   return *this;
}






Project& Project::saveAs(const QString& path)
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
   return *this;
}






Project& Project::setName(const QString& name)
{
   if ( _name != name )
   {
      _name = name;
      emit nameChanged();
      signalModified();
   }
   return *this;
}






Project& Project::setScanDirectory(const QString& path)
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
   return *this;
}






Project& Project::setScanFilters(const QString& filters)
{
   if ( _scanFilters != filters )
   {
      _scanFilters = filters;
      signalModified();
   }
   return *this;
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






void Project::readTypeElement(const QDomElement& type)
{
   if ( !type.hasAttribute("id") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Type attribute missing from project file."));
      throw e;
   }
   bool ok;
   _type = type.attribute("id").toInt(&ok);
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in type as integer."));
      throw e;
   }
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   if ( _type < 0 || _type >= factory.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(_type).arg(factory.size()-1));
      throw e;
   }
   QString typeName = type.text();
   if ( typeName != factory.name(_type) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type name %1 when it should be %2.").arg(typeName).arg(factory.name(_type)));
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
