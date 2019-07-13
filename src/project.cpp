#include "project.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QCryptographicHash>
#include <QDomDocument>
#include <socutil/ReadError>
#include <socutil/WriteError>
#include "abstract_block.h"
#include "abstract_blockfactory.h"
#include "abstract_projectfactory.h"
#include "blockmodel.h"
#include "scanthread.h"
#include "dictionarymodel.h"
using Soc::Ut::ReadError;
using Soc::Ut::WriteError;



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
 * The tag name for the custom dictionary element.
 */
const char* Project::_dictionaryTag {"dictionary"};
/*!
 * The name of the id attribute.
 */
const char* Project::_idTag {"id"};






/*!
 * This constructs a new project of the given type.
 *
 * @param type The project type this new project is constructed as.
 */
Project::Project(int type)
   :
   _type(type),
   _scanDirectory("."),
   _model(new BlockModel(this)),
   _dictionary(new DictionaryModel(this))
{
   // Connect this project's custom dictionary modified signal.
   connect(_dictionary,&DictionaryModel::modified,this,&Project::projectModified);

   // Initialize this new project's root block, file watcher signal, and default scan
   // filters.
   makeRoot();
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);
   _scanFilters = Abstract::ProjectFactory::instance().defaultFilters(_type);
}






/*!
 * This constructs a project loaded from the file from the given path. If any
 * error occurs from loading the project file an exception is thrown.
 *
 * @param path The path of the file that this project is loaded from.
 */
Project::Project(const QString& path)
   :
   _path(path),
   _model(new BlockModel(this)),
   _dictionary(new DictionaryModel(this))
{
   // Connect this project's custom dictionary modified signal.
   connect(_dictionary,&DictionaryModel::modified,this,&Project::projectModified);

   // Initialize the enumeration and static string list used for identifying
   // different elements to read in based off their tag names.
   enum {Name,Type,ScanDir,ScanFilters,Dictionary,Root};
   static const QStringList tags
   {
      _nameTag,_typeTag,_scanDirectoryTag,_scanFiltersTag,_dictionaryTag,_rootTag
   };

   // Initialize this project's file watcher signal.
   connect(this,&QFileSystemWatcher::fileChanged,this,&Project::fileChanged);

   // Read in the entire contents of the file located at the given path.
   QByteArray xmlBytes {read()};
   QDomDocument document;

   // Load the read in file contents as an XML document and make sure it worked.
   if (!document.setContent(xmlBytes))
   {
      throw ReadError(qUtf8Printable(tr("Failed opening %1 as XML.").arg(path)),"");
   }

   // Iterate through all child nodes of the root XML document element.
   QDomNode node {document.documentElement().firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if this node is an element.
      if ( node.isElement() )
      {
         // Figure out what element this is based off its tag name and set the appropriate
         // data for this project.
         QDomElement element {node.toElement()};
         switch (tags.indexOf(element.tagName()))
         {
         case Name:
            _name = element.text();
            break;
         case Type:
            readTypeElement(element);
            break;
         case ScanDir:
            convertScanDirectory(element.text());
            break;
         case ScanFilters:
            _scanFilters = element.text();
            break;
         case Dictionary:
            _dictionary->read(element);
            break;
         case Root:

            // First make a blank root block for this project and then read in the root block
            // element.
            makeRoot();
            _root->read(element);
            break;
         }
      }

      // Move to the next node sibling.
      node = node.nextSibling();
   }

   // Make sure the required elements were found in the XML document.
   if ( _type == -1 )
   {
      throw ReadError(qUtf8Printable(tr("Type element not found in %1.").arg(path)),"");
   }
   if ( _scanDirectory.isEmpty() )
   {
      throw ReadError(qUtf8Printable(tr("Scan directory element not found in %1.").arg(path)),"");
   }

   // Add this project's file path to its file watcher.
   addPath(_path);
}






/*!
 * Tests if this is a new project that has not yet been saved to a file.
 *
 * @return True if this is a new unsaved project or false otherwise.
 */
bool Project::isNew() const
{
   // Return a boolean based off this project's file path being empty or not.
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
 * Returns the path of the file associated with this open project. If this is a
 * new project that has not been saved yet this returns an empty string.
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
 * Creates and returns a mapping of scanner objects that can be used to parse
 * any source files that belong to this project.
 *
 * @return Mapping of scanner objects that can be used to parse any source files
 *         that belong to this project.
 */
const QMap<QString,Scanner*> Project::createScannerMap() const
{
   return Abstract::ProjectFactory::instance().createScannerMap(_root);
}






/*!
 * Returns a pointer to the block model for this project used to view and edit
 * the block data contained within it.
 *
 * @return Pointer to this project's block model.
 */
BlockModel* Project::model()
{
   return _model;
}






/*!
 * Returns a pointer to the dictionary model for this project used for storing
 * custom spell checking words.
 *
 * @return Pointer to this project's dictionary model.
 */
DictionaryModel* Project::dictionary()
{
   return _dictionary;
}






/*!
 * Sets a new name for this project with the given name.
 *
 * @param value The new name this project's name is set to.
 */
void Project::setName(const QString& value)
{
   // Make sure the new value is different from the current one.
   if ( _name != value )
   {
      // Set this project's name to the new value and signal this project's name has
      // changed and it has been modified.
      _name = value;
      emit nameChanged();
      signalModified();
   }
}






/*!
 * Sets a new scan directory path for this project to the given path. The given
 * path can be relative to this program's present working directory. If the
 * given path is not a valid directory then an exception is thrown.
 *
 * @param path Path that this project's scan directory is set to.
 */
void Project::setScanDirectory(const QString& path)
{
   // Get file info for this project's current scan directory and the new path given.
   QFileInfo current(_scanDirectory);
   QFileInfo info(path);

   // Make sure the new path is different from the current path.
   if ( current.canonicalFilePath() == info.canonicalFilePath() ) return;

   // Make sure the new path is a valid directory.
   Q_ASSERT(info.isDir());

   // Set this project's scan directory to the new path given in its canonical form
   // and signal modification.
   _scanDirectory = info.canonicalFilePath();
   signalModified();
}






/*!
 * Sets new scanning file filters for this project. These filters are used to
 * filter out what files are matched for parsing when a scan thread of this
 * project is used. The filter filters must be separated by spaces.
 *
 * @param value The file filters that this project will use for scanning.
 */
void Project::setScanFilters(const QString& value)
{
   // Make sure the new value is different from the current one.
   if ( _scanFilters != value )
   {
      // Update this project's scan filters to the new value and signal modification.
      _scanFilters = value;
      signalModified();
   }
}






/*!
 * Saves this project to its associated file. If this is a new project that has
 * never been saved then an exception is thrown. If any writing error occurs
 * while saving an exception is thrown.
 */
void Project::save()
{
   // Make sure this is not a new project and has a file path.
   Q_ASSERT(!_path.isEmpty());

   // Create a new XML document, appending its XML processing instructions.
   QDomDocument document;
   document.appendChild(document.createProcessingInstruction("xml"
                                                             ,"version=\"1.0\" encoding=\"UTF-8\""));

   // Create the name element saving this project's name.
   QDomElement name {document.createElement(_nameTag)};
   name.appendChild(document.createTextNode(_name));

   // Create the scan directory element saving this project's scan directory as the
   // path relative to the file path of this project.
   QDomElement scandir {document.createElement(_scanDirectoryTag)};
   scandir.appendChild(document.createTextNode(QFileInfo(_path).dir().relativeFilePath(_scanDirectory)));

   // Create the filters element saving this project's scan filters.
   QDomElement filters {document.createElement(_scanFiltersTag)};
   filters.appendChild(document.createTextNode(_scanFilters));

   // Create the type element saving this project type's element name.
   QDomElement type {document.createElement(_typeTag)};
   type.appendChild(document.createTextNode(Abstract::ProjectFactory::instance().elementName(_type)));

   // Create the dictionary element saving this project's custom dictionary data.
   QDomElement dictionary {_dictionary->write(document)};
   dictionary.setTagName(_dictionaryTag);

   // Create the root block element saving this project's root block and all its
   // children.
   QDomElement root {_root->write(document)};
   root.setTagName(_rootTag);

   // Create the root project element, adding all previous elements created, and then
   // add the root element to the XML document.
   QDomElement project {document.createElement("project")};
   project.appendChild(name);
   project.appendChild(type);
   project.appendChild(filters);
   project.appendChild(scandir);
   project.appendChild(dictionary);
   project.appendChild(root);
   document.appendChild(project);

   // Save the XML document to a byte array and then write it out to this project's
   // file path.
   QByteArray xmlBytes {document.toByteArray(2)};
   write(xmlBytes);

   // Set this project as not being modified and signal it has been saved.
   _modified = false;
   emit saved();
}






/*!
 * Saves this project to a new file with the given path. If any writing error
 * occurs while saving an exception is thrown.
 *
 * @param path The path to the new file this project is saved to.
 */
void Project::saveAs(const QString& path)
{
   // Remember this project's old path and then set it to the new path given.
   QString oldPath = _path;
   _path = path;
   try
   {
      // Attempt to save this project with the new path.
      save();
   }

   // If any exception is thrown while saving then revert this project's path back to
   // the old one.
   catch (...)
   {
      _path = oldPath;
      throw;
   }

   // Remove the old path from this project's file watcher if is exists and add the
   // new path.
   if ( !oldPath.isEmpty() ) removePath(oldPath);
   addPath(_path);
}






/*!
 * Called when a block contained within this project or its custom dictionary
 * has been modified. This in turn causes this project to change its state to
 * modified.
 */
void Project::projectModified()
{
   signalModified();
}






/*!
 * Called when this project's file has changed. This is called even if the
 * project itself is saved to the file, so this also checks to see it that is
 * the case or not.
 */
void Project::fileChanged()
{
   // Open this project's file and make sure it worked.
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) ) return;

   // Read in the entire contents of the file and make sure it worked.
   QByteArray data = file.readAll();
   if ( file.error() != QFileDevice::NoError ) return;

   // Generate the hash of the read in file's contents and if the hash is different
   // from this project's saved hash then signal the save file has changed.
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(data);
   if ( hash.result() != _hash ) emit saveFileChanged();
}






/*!
 * Emits the modified signal of this project if and only if this project is not
 * already modified. If the project is already modified this does nothing.
 */
void Project::signalModified()
{
   // Make sure this project is not already modified.
   if ( !_modified )
   {
      // Set this project as modified and signal the same.
      _modified = true;
      emit modified();
   }
}






/*!
 * Reads in this project's file and returns its entire contents as a byte array.
 * If opening or reading this project's file fails then an exception is thrown.
 *
 * @return Contents of this project's file as a byte array.
 */
QByteArray Project::read()
{
   // Open this project's file and make sure it worked.
   QFile file(_path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      throw ReadError(qUtf8Printable(tr("Failed opening %1").arg(_path))
                      ,qUtf8Printable(file.errorString()));
   }

   // Read in the entire contents of this project's file and make sure it worked.
   QByteArray ret = file.readAll();
   if ( file.error() != QFileDevice::NoError )
   {
      throw ReadError(qUtf8Printable(tr("Failed reading %1").arg(_path))
                      ,qUtf8Printable(file.errorString()));
   }

   // Set this project's file hash with the read in byte array and return it.
   setFileHash(ret);
   return ret;
}






/*!
 * Converts the given scan directory path into its absolute canonical form and
 * saves that form to this project's scan directory path. If the given path is
 * not a valid directory than an exception is thrown.
 *
 * @param path The given path to that is converted to its canonical form and
 *             saved as this project's scan directory.
 */
void Project::convertScanDirectory(const QString& path)
{
   // Make sure the given path is a valid directory relative to the directory this
   // project's file is located.
   QDir directory {QFileInfo(_path).dir()};
   if ( !directory.cd(path) )
   {
      throw ReadError(qUtf8Printable(tr("The scanning directory %1 in %2 does not exist.")
                                     .arg(path)
                                     .arg(_path))
                      ,"");
   }

   // Set this project's scan directory to the path given in its canonical form.
   _scanDirectory = directory.canonicalPath();
}






/*!
 * Reads the given element as the XML type element for this project, setting
 * this project's type. If the given XML element is not a valid type element
 * then an exception is thrown.
 *
 * @param element The XML element that is read in as this project's type
 *                element.
 */
void Project::readTypeElement(const QDomElement& element)
{
   // Look up the project type with the element name supplied by the given XML
   // element.
   _type = Abstract::ProjectFactory::instance().typeByElementName(element.text());

   // Make sure the element name was recognized and a known type returned.
   if ( _type < 0 )
   {
      throw ReadError(qUtf8Printable(tr("Unknown project type '%1' in %2:%3.")
                                     .arg(element.text())
                                     .arg(_path)
                                     .arg(element.lineNumber()))
                      ,"");
   }
}






/*!
 * Writes the given byte array to this project's file, overwriting anything the
 * file contains beforehand. If opening or writing this project's file fails
 * then an exception is thrown.
 *
 * @param data The byte array that is written to this project's file.
 */
void Project::write(const QByteArray& data)
{
   // Open this project's file for truncated writing and make sure it worked.
   QFile file(_path);
   if ( !file.open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      throw WriteError(qUtf8Printable(tr("Failed opening %1").arg(_path))
                       ,qUtf8Printable(file.errorString()));
   }

   // Write out the given byte array to the opened file and make sure it worked.
   if ( file.write(data) != data.size() )
   {
      throw WriteError(qUtf8Printable(tr("Failed writing to %1").arg(_path))
                       ,qUtf8Printable(file.errorString()));
   }

   // Set this project's file hash with the given byte array.
   setFileHash(data);
}






/*!
 * Sets this project's file hash with the hash of the given byte array. This is
 * used for this project to watch for changes to its file.
 *
 * @param bytes The byte array whose hash is set as this project's file hash
 *              value.
 */
void Project::setFileHash(const QByteArray& bytes)
{
   // Get the hash value of the given byte array using md5 and set it to this
   // project's file hash.
   QCryptographicHash hash(QCryptographicHash::Md5);
   hash.addData(bytes);
   _hash = hash.result();
}






/*!
 * Makes a new root block for this project using the block factory this
 * project's type, setting this project's root block pointer to the new root
 * block created. If this project's type is out of range of possible project
 * types or the block factory returns a null pointer than an exception is
 * thrown.
 */
void Project::makeRoot()
{
   // Make sure this project's type is within range and valid.
   Abstract::ProjectFactory& factory {Abstract::ProjectFactory::instance()};
   Q_ASSERT(_type >= 0);
   Q_ASSERT(_type < factory.size());

   // Create a new root block for this project and make sure it is not null.
   _root = factory.blockFactory(_type).createRootBlock().release(this);
   Q_ASSERT(_root);

   // Connect its modified signal and set it to this project's block model root
   // block.
   connect(_root,&Abstract::Block::childUpdated,this,&Project::projectModified);
   _model->setRoot(_root);
}

