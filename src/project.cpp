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
 * The tag name for the name element. 
 */
const char* Project::_nameTag {"name"};
/*!
 * The tag name for the type element. 
 */
const char* Project::_typeTag {"type"};
/*!
 * The tag name for the scan directory element. 
 */
const char* Project::_scanDirectoryTag {"scandir"};
/*!
 * The tag name for the scanning file filters element. 
 */
const char* Project::_scanFiltersTag {"filters"};
/*!
 * The tag name for the root block element. 
 */
const char* Project::_rootTag {"root"};
/*!
 * The name of the id attribute. 
 */
const char* Project::_idTag {"id"};






/*!
 * This constructs a new project of the given type. 
 *
 * @param type The project type this new project is constructed as. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Connect the file changed signal, create this project's root block, and set 
 *    this project's file filters to the default for this project's type. 
 */
Project::Project(int type):
   _type(type),
   _scanDirectory(".")
{
   // 1
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);
   makeRoot();
   _scanFilters = AbstractProjectFactory::instance().defaultFilters(_type);
}






/*!
 * This constructs a project loaded from the file from the given path. If any error 
 * occurs from loading the project file an exception is thrown. 
 *
 * @param path The path of the file that this project is loaded from. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Connect the file changed signal and get the contents of this project's file 
 *    as a byte array. 
 *
 * 2. Open a new Qt XML document with the byte array, reading in its contents for 
 *    all required elements. If all required elements are not found then throw an 
 *    exception, else go to the next step. 
 *
 * 3. Read in the scan directory, type element, and root block along with all of 
 *    its children blocks after creating a new block root. 
 *
 * 4. Add this project's file path to the files being watched for changes and emit 
 *    the saved signal. 
 */
Project::Project(const QString& path):
   _path(path)
{
   // 1
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);
   QByteArray xmlBytes {read()};

   // 2
   QDomDocument document;
   document.setContent(xmlBytes);
   QDomElement project {document.documentElement()};
   DomElementReader reader(project);
   QDomElement type;
   QDomElement root;
   QString scanDirectory;
   reader.set(_nameTag,&_name);
   reader.set(_typeTag,&type);
   reader.set(_scanDirectoryTag,&scanDirectory);
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

   // 3
   convertScanDirectory(scanDirectory);
   readTypeElement(type);
   makeRoot();
   _root->read(root);

   // 4
   addPath(_path);
   emit saved();
}






/*!
 * Tests if this is a new project that has not yet been saved to a file. 
 *
 * @return True if this is a new unsaved project or false otherwise. 
 */
bool Project::isNew() const
{
   return _path.isEmpty();
}






/*!
 * Tests of this project has unsaved modifications. 
 *
 * @return True if this project has unsaved content or false otherwise. 
 */
bool Project::isModified() const
{
   return _modified;
}






/*!
 * Returns the project type for this project. 
 *
 * @return This project's type. 
 */
int Project::type() const
{
   return _type;
}






/*!
 * Returns the path of the file associated with this open project. If this is a new 
 * project that has not been saved yet this returns an empty string. 
 *
 * @return Path to file associated with this project or an empty string. 
 */
QString Project::path() const
{
   return _path;
}






/*!
 * Returns the name of this project. 
 *
 * @return The project's name. 
 */
QString Project::name() const
{
   return _name;
}






/*!
 * Returns the scan directory path of this project in its canonical form. 
 *
 * @return Canonical path to this project's scan directory. 
 */
QString Project::scanDirectory() const
{
   return _scanDirectory;
}






/*!
 * Returns this project's file filters used for scanning of source files. 
 *
 * @return File filters used for this project's scanning. 
 */
QString Project::scanFilters() const
{
   return _scanFilters;
}






/*!
 * Makes and returns a pointer to a new scan thread that can be used for scanning 
 * and parsing matched source files in the scan directory of this project. 
 *
 * @return Pointer to new scan thread object. 
 */
std::unique_ptr<ScanThread> Project::makeScanner() const
{
   return unique_ptr<ScanThread>(new ScanThread(AbstractProjectFactory::instance().makeParserFactory(_type,_root)
                                                ,_scanDirectory
                                                ,_scanFilters.split(' ')));
}






/*!
 * Returns a pointer to the block model for this project used to edit the block 
 * data contained within it. 
 *
 * @return Pointer to this project's block model. 
 */
BlockModel* Project::model()
{
   return _model;
}






/*!
 * Sets a new name for this project with the given name. 
 *
 * @param newName The new name this project's name is set to. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given new name is different than this project's current name then set 
 *    this project's name to the new name given, emit the name changed signal, and 
 *    signal this project has been modified. 
 */
void Project::setName(const QString& newName)
{
   // 1
   if ( _name != newName )
   {
      _name = newName;
      emit nameChanged();
      signalModified();
   }
}






/*!
 * Sets a new scan directory path for this project to the given path. The given 
 * path can be relative to this program's present working directory. If the given 
 * path is not a valid directory then an exception is thrown. 
 *
 * @param newPath Path that this project's scan directory is set to. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given path is the same as this project's current scan directory path 
 *    then return. 
 *
 * 2. If the given new path is not a valid directory then throw an exception. 
 *
 * 3. Set this project's scan directory to the new path given in its canonical form 
 *    and signal this project has been modified. 
 */
void Project::setScanDirectory(const QString& newPath)
{
   // 1
   QFileInfo current(_scanDirectory);
   QFileInfo info(newPath);
   if ( current.canonicalFilePath() == info.canonicalFilePath() ) return;

   // 2
   if ( !info.isDir() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to set scan directory as '%1' which is not a directory.")
                   .arg(newPath));
      throw e;
   }

   // 3
   _scanDirectory = info.canonicalFilePath();
   signalModified();
}






/*!
 * Sets new scanning file filters for this project. These filters are used to 
 * filter out what files are matched for parsing when a scan thread of this project 
 * is used. The filter filters must be separated by spaces. 
 *
 * @param newFilters The file filters that this project will use for scanning. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given scan filters is different than this project's current scan 
 *    filters then set this project's scan filters to the new filters given and 
 *    signal this project has been modified. 
 */
void Project::setScanFilters(const QString& newFilters)
{
   // 1
   if ( _scanFilters != newFilters )
   {
      _scanFilters = newFilters;
      signalModified();
   }
}






/*!
 * Saves this project to its associated file. If this is a new project that has 
 * never been saved then an exception is thrown. If any writing error occurs while 
 * saving an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this project has no path to save to then throw an exception, else go to 
 *    the next step. 
 *
 * 2. Create a new Qt XML document, appending its XML processing instructions and 
 *    creating the root project element. 
 *
 * 3. Save this project's basic information, its type element, and its root block 
 *    along with all children blocks to the project element. 
 *
 * 4. Save the Qt XML document as a byte array and save the byte array to this 
 *    project's file, setting this project as not modified and emitting the saved 
 *    signal. 
 */
void Project::save()
{
   // 1
   if ( _path.isEmpty() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to save new project that has no path."));
      throw e;
   }

   // 2
   QDomDocument document;
   document.appendChild(document.createProcessingInstruction("xml"
                                                             ,"version=\"1.0\" encoding=\"UTF-8\""));
   QDomElement project {document.createElement("project")};

   // 3
   saveBasicInfo(document,&project);
   QDomElement type {document.createElement(_typeTag)};
   type.setAttribute(_idTag,QString::number(_type));
   type.appendChild(document.createTextNode(AbstractProjectFactory::instance().name(_type)));
   project.appendChild(type);
   QDomElement root {_root->write(document)};
   root.setTagName(_rootTag);
   project.appendChild(root);
   document.appendChild(project);

   // 4
   QByteArray xmlBytes {document.toByteArray(3)};
   write(xmlBytes);
   _modified = false;
   emit saved();
}






/*!
 * Saves this project to a new file with the given path. If any writing error 
 * occurs while saving an exception is thrown. 
 *
 * @param path The path to the new file this project is saved to. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Save this project's current file path to _oldPath_, set this project's file 
 *    path to the new path given, and then call this project's save method. 
 *
 * 2. If any exception is caught while saving then revert this project and its file 
 *    watcher back to the old path and throw the caught exception. 
 *
 * 3. If _oldPath_ is not empty then remove the path from this file watcher. Add 
 *    the new path to this file watcher. 
 */
void Project::saveAs(const QString& path)
{
   // 1
   QString oldPath = _path;
   _path = path;
   try
   {
      save();
   }

   // 2
   catch (...)
   {
      _path = oldPath;
      throw;
   }

   // 3
   if ( !oldPath.isEmpty() ) removePath(oldPath);
   addPath(_path);
}






/*!
 * Called when a block contained within this project has been modified. This in 
 * turn causes this project to change its state to modified. 
 */
void Project::blockModified()
{
   signalModified();
}






/*!
 * Called when this project's file has changed. This is called even if the project 
 * itself is saved to the file, so this also checks to see it that is the case or 
 * not. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Open this project's file and read in its entire contents as a byte array. If 
 *    opening or reading fails then return. 
 *
 * 2. Check the saved hash of the saved file with the current hash of the saved 
 *    file. If the hashes are different then signal the save file has changed from 
 *    an outside source, else do nothing. 
 */
void Project::fileChanged()
{
   // 1
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) ) return;
   QByteArray data = file.readAll();
   if ( file.error() != QFileDevice::NoError ) return;

   // 2
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);
   if ( hash.result() != _hash ) emit saveFileChanged();
}






/*!
 * Emits the modified signal of this project if and only if this project is not 
 * already modified. If the project is already modified this does nothing. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this project is not modified then set it as modified and emit the modified 
 *    signal. 
 */
void Project::signalModified()
{
   // 1
   if ( !_modified )
   {
      _modified = true;
      emit modified();
   }
}






/*!
 * Reads in this project's file and returns its entire contents as a byte array. If 
 * opening or reading this project's file fails then an exception is thrown. 
 *
 * @return Contents of this project's file as a byte array. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Open this project's file and read all its contents as a byte array. If 
 *    opening or reading failed then throw an exception. 
 *
 * 2. Set this project's file hash, used for file watching, and return the byte 
 *    array. 
 */
QByteArray Project::read()
{
   // 1
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for reading: %2").arg(_path).arg(file.errorString()));
      throw e;
   }
   QByteArray ret = file.readAll();
   if ( file.error() != QFileDevice::NoError )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Error reading from file %1: %2").arg(_path).arg(file.errorString()));
      throw e;
   }

   // 2
   setFileHash(ret);
   return ret;
}






/*!
 * Converts the given scan directory path into its absolute canonical form and 
 * saves that form to this project's scan directory path. If the given path is not 
 * a valid directory than an exception is thrown. 
 *
 * @param path The given path to that is converted to its canonical form and saved 
 *             as this project's scan directory. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given path is not a valid directory then throw an exception. 
 *
 * 2. Convert the given path to its canonical form and save it to this project's 
 *    scan directory path. 
 */
void Project::convertScanDirectory(const QString& path)
{
   // 1
   QDir directory {QFileInfo(_path).dir()};
   if ( !directory.cd(path) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Scan directory in XML file is invalid; failed changing directory from %1 to %2.")
                   .arg(directory.canonicalPath())
                   .arg(_scanDirectory));
      throw e;
   }

   // 2
   _scanDirectory = directory.canonicalPath();
}






/*!
 * Reads the given element as the XML type element for this project, setting this 
 * project's type. If the given XML element is not a valid type element then an 
 * exception is thrown. 
 *
 * @param element The XML element that is read in as this project's type element. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the project type from the given XML element as an integer and set 
 *    this project's type to that value. If the read in type is out of range for 
 *    all possible project types then throw an exception. 
 *
 * 2. Read in the text field of the given XML element and make sure it matches the 
 *    name of the project type read in. If they do not match then throw an 
 *    exception. 
 */
void Project::readTypeElement(const QDomElement& element)
{
   // 1
   DomElementReader reader(element);
   _type = reader.attributeToInt(_idTag);
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   if ( _type < 0 || _type >= factory.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.")
                   .arg(_type)
                   .arg(factory.size() - 1));
      throw e;
   }

   // 2
   QString typeName = element.text();
   if ( typeName != factory.name(_type) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type name %1 when it should be %2.")
                   .arg(typeName)
                   .arg(factory.name(_type)));
      throw e;
   }
}






/*!
 * Writes the given byte array to this project's file, overwriting anything the 
 * file contains beforehand. If opening or writing this project's file fails then 
 * an exception is thrown. 
 *
 * @param data The byte array that is written to this project's file. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Open this project's file and write the given byte array to the file, 
 *    overwriting and erasing any data the file contained beforehand. If opening or 
 *    writing failed then throw an exception. 
 *
 * 2. Set this project's file hash used for watching. 
 */
void Project::write(const QByteArray& data)
{
   // 1
   QFile file(_path);
   if ( !file.open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      Exception::OpenError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot open file %1 for writing: %2").arg(_path).arg(file.errorString()));
      throw e;
   }
   if ( file.write(data) != data.size() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Error writing to file %1: %2").arg(_path).arg(file.errorString()));
      throw e;
   }

   // 2
   setFileHash(data);
}






/*!
 * Saves this project's name, scan directory, and file filters to the given XML 
 * element as child elements. 
 *
 * @param document The XML document that is used to create the child elements. 
 *
 * @param project Pointer to the element which has child elements added to it to 
 *                save this project's basic information. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a name XML element and save this project's name to it, appending it to 
 *    the given project element.  
 *
 * 2. Create a scan directory XML element and save this project's scan directory, 
 *    appending it to the given project element. The scan directory path saved is 
 *    relative to the directory where this project's save file is located. 
 *
 * 3. Create a filters XML element and save this project's scan filters to it, 
 *    appending it to the given project element. 
 */
void Project::saveBasicInfo(QDomDocument& document, QDomElement* project)
{
   // 1
   QDomElement name {document.createElement(_nameTag)};
   name.appendChild(document.createTextNode(_name));
   project->appendChild(name);

   // 2
   QDomElement scandir {document.createElement(_scanDirectoryTag)};
   scandir.appendChild(document.createTextNode(QFileInfo(_path).dir().relativeFilePath(_scanDirectory)));
   project->appendChild(scandir);

   // 3
   QDomElement filters {document.createElement(_scanFiltersTag)};
   filters.appendChild(document.createTextNode(_scanFilters));
   project->appendChild(filters);
}






/*!
 * Sets this project's file hash with the hash of the given byte array. This is 
 * used for this project to watch for changes to its file. 
 *
 * @param bytes The byte array whose hash is set as this project's file hash value. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the hash value of the given byte array using md5 and set this project's 
 *    file hash to the new value. 
 */
void Project::setFileHash(const QByteArray& bytes)
{
   // 1
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(bytes);
   _hash = hash.result();
}






/*!
 * Makes a new root block for this project using the block factory this project's 
 * type, setting this project's root block pointer to the new root block created. 
 * If this project's type is out of range of possible project types or the block 
 * factory returns a null pointer than an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this project's type is out of range for possible project types then throw 
 *    an exception. 
 *
 * 2. Create a new root block using this project type's block factory, setting this 
 *    project's root block pointer to the new root block. If the block factory 
 *    returns a null pointer than throw an exception, else set the new root block's 
 *    parent to this object. 
 *
 * 3. Connect the block modified signal between the new root block and this 
 *    project. Create a new block model object with the new root block and this 
 *    object as its parent, setting this object's block model pointer to the new 
 *    block model. 
 */
void Project::makeRoot()
{
   // 1
   AbstractProjectFactory& factory {AbstractProjectFactory::instance()};
   if ( _type < 0 || _type >= factory.size() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invald project type %1 when max is %2.").arg(_type).arg(factory.size() - 1));
      throw e;
   }

   // 2
   _root = factory.blockFactory(_type).makeRootBlock().release();
   if ( !_root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Expected pointer to new root block object when null was given."));
      throw e;
   }
   _root->QObject::setParent(this);

   // 3
   connect(_root,&AbstractBlock::modified,this,&Project::blockModified);
   _model = new BlockModel(_root,this);
}
