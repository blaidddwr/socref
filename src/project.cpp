#include "project.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QCryptographicHash>
#include <QDomDocument>
#include <exception.h>
#include "abstractprojectfactory.h"
#include "abstractparserfactory.h"
#include "abstractblockfactory.h"
#include "abstractblock.h"
#include "blockmodel.h"
#include "domelementreader.h"
#include "scanthread.h"



using namespace std;
//



/*!
 */
const char* Project::_nameTag {"name"};
/*!
 */
const char* Project::_typeTag {"type"};
/*!
 */
const char* Project::_scanDirectoryTag {"scandir"};
/*!
 */
const char* Project::_scanFiltersTag {"filters"};
/*!
 */
const char* Project::_rootTag {"root"};
/*!
 */
const char* Project::_idTag {"id"};






/*!
 *
 * @param type  
 */
Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);
   makeRoot();
   _scanFilters = AbstractProjectFactory::instance().defaultFilters(_type);
}






/*!
 *
 * @param path  
 */
Project::Project(const QString& path):
   _path(path)
{
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);
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
   reader.set(_scanDirectoryTag,&_scanDirectory);
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






/*!
 */
bool Project::isNew() const
{
   return _path.isEmpty();
}






/*!
 */
bool Project::isModified() const
{
   return _modified;
}






/*!
 */
int Project::type() const
{
   return _type;
}






/*!
 */
QString Project::path() const
{
   return _path;
}






/*!
 */
BlockModel* Project::model()
{
   return _model;
}






/*!
 */
QString Project::name() const
{
   return _name;
}






/*!
 *
 * @param newName  
 */
void Project::setName(const QString& newName)
{
   if ( _name != newName )
   {
      _name = newName;
      emit nameChanged();
      signalModified();
   }
}






/*!
 */
QString Project::scanDirectory() const
{
   return _scanDirectory;
}






/*!
 *
 * @param newPath  
 */
void Project::setScanDirectory(const QString& newPath)
{
   QFileInfo current(_scanDirectory);
   QFileInfo info(newPath);
   if ( current.canonicalFilePath() != info.canonicalFilePath() )
   {
      if ( !info.isDir() )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(
                  tr("Attempting to set scan directory as '%1' which is not a directory.")
                  .arg(newPath));
         throw e;
      }
      _scanDirectory = info.canonicalFilePath();
      signalModified();
   }
}






/*!
 */
QString Project::scanFilters() const
{
   return _scanFilters;
}






/*!
 *
 * @param newFilters  
 */
void Project::setScanFilters(const QString& newFilters)
{
   if ( _scanFilters != newFilters )
   {
      _scanFilters = newFilters;
      signalModified();
   }
}






/*!
 */
std::unique_ptr<ScanThread> Project::makeScanner() const
{
   return unique_ptr<ScanThread>(
            new ScanThread(
               AbstractProjectFactory::instance().makeParserFactory(_type,_root)
               ,_scanDirectory
               ,_scanFilters.split(' ')));
}






/*!
 */
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
   QDomElement scandir {document.createElement(_scanDirectoryTag)};
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






/*!
 *
 * @param path  
 */
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






/*!
 */
void Project::blockModified()
{
   signalModified();
}






/*!
 */
void Project::fileChanged()
{
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) ) return;
   QByteArray data = file.readAll();
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);
   if ( hash.result() != _hash ) emit changed();
}






/*!
 */
void Project::signalModified()
{
   if ( !_modified )
   {
      _modified = true;
      emit modified();
   }
}






/*!
 *
 * @param bytes  
 */
void Project::setFileHash(const QByteArray& bytes)
{
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(bytes);
   _hash = hash.result();
}






/*!
 *
 * @param element  
 */
void Project::readTypeElement(const QDomElement& element)
{
   DomElementReader reader(element);
   _type = reader.attributeToInt(_idTag);
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   if ( _type < 0 || _type >= factory.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(_type).arg(factory.size()-1));
      throw e;
   }
   QString typeName = element.text();
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






/*!
 */
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
   _root->QObject::setParent(this);
   connect(_root,&AbstractBlock::modified,this,&Project::blockModified);
   _model = new BlockModel(_root,this);
}
