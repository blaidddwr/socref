#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QCryptographicHash>
#include <QDomDocument>
#include "project.h"
#include "abstractprojectfactory.h"
#include "abstractparserfactory.h"
#include "abstractblockfactory.h"
#include "abstractblock.h"
#include "blockmodel.h"
#include "exception.h"
#include "domelementreader.h"
#include "scanthread.h"



using namespace std;
const char* Project::_nameTag {"name"};
const char* Project::_typeTag {"type"};
const char* Project::_scandirectoryTag {"scandir"};
const char* Project::_scanFiltersTag {"filters"};
const char* Project::_rootTag {"root"};
const char* Project::_idTag {"id"};






Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::handleFileChanged);
   makeRoot();
   _scanFilters = AbstractProjectFactory::instance().defaultFilters(_type);
}






Project::Project(const QString &path):
   _path(path)
{
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
   DomElementReader reader(document.documentElement());
   QDomElement type;
   QDomElement root;
   reader.set(_nameTag,&_name);
   reader.set(_typeTag,&type);
   reader.set(_scandirectoryTag,&_scanDirectory);
   reader.set(_scanFiltersTag,&_scanFilters);
   reader.set(_rootTag,&root);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Could not find all required XML elements of project."));
      throw e;
   }
   QDir directory {QFileInfo(path).dir()};
   if ( !directory.cd(_scanDirectory) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Scan directory in XML file is invalid; failed changing directory from %1 to %2.")
               .arg(directory.canonicalPath())
               .arg(_scanDirectory));
      throw e;
   }
   _scanDirectory = directory.canonicalPath();
   readTypeElement(type);
   makeRoot();
   _root->read(root);
   setFileHash(xmlBytes);
   addPath(_path);
   emit saved();
}






void Project::save()
{
   if ( _path.isEmpty() )
   {
      Exception::LogicError e;
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
   QDomElement name {document.createElement(_nameTag)};
   QDomElement type {document.createElement(_typeTag)};
   QDomElement scandir {document.createElement(_scandirectoryTag)};
   QDomElement filters {document.createElement(_scanFiltersTag)};
   QDomElement root {_root->write(document)};
   name.appendChild(document.createTextNode(_name));
   type.setAttribute(_idTag,QString::number(_type));
   type.appendChild(document.createTextNode(AbstractProjectFactory::instance().name(_type)));
   scandir.appendChild(
            document.createTextNode(QFileInfo(_path).dir().relativeFilePath(_scanDirectory)));
   filters.appendChild(document.createTextNode(_scanFilters));
   root.setTagName(_rootTag);
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
   if ( !oldPath.isEmpty() ) removePath(oldPath);
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






QString Project::name() const
{
   return _name;
}






QString Project::path() const
{
   return _path;
}






int Project::type() const
{
   return _type;
}






QString Project::scanDirectory() const
{
   return _scanDirectory;
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
         e.setDetails(
                  tr("Attempting to set scan directory as '%1' which is not a directory.")
                  .arg(path));
         throw e;
      }
      _scanDirectory = info.canonicalFilePath();
      signalModified();
   }
}






QString Project::scanFilters() const
{
   return _scanFilters;
}






void Project::setScanFilters(const QString& filters)
{
   if ( _scanFilters != filters )
   {
      _scanFilters = filters;
      signalModified();
   }
}






bool Project::isNew() const
{
   return _path.isEmpty();
}






bool Project::isModified() const
{
   return _modified;
}






BlockModel* Project::model() const
{
   return _model;
}






std::unique_ptr<ScanThread> Project::prepareScanner() const
{
   return unique_ptr<ScanThread>(
            new ScanThread(
               AbstractProjectFactory::instance().parserFactory(_type,_root)
               ,_scanDirectory
               ,_scanFilters.split(' ')));
}






void Project::blockModified()
{
   signalModified();
}






void Project::handleFileChanged()
{
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) ) return;
   QByteArray data = file.readAll();
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);
   if ( hash.result() != _hash ) emit changed();
}






void Project::readTypeElement(const QDomElement& type)
{
   DomElementReader reader(type);
   _type = reader.attributeToInt(_idTag);
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
      e.setDetails(
               tr("Read in invalid type name %1 when it should be %2.")
               .arg(typeName)
               .arg(factory.name(_type)));
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






void Project::makeRoot()
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
