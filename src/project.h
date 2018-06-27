#ifndef PROJECT_H
#define PROJECT_H
#include <memory>
#include <QFileSystemWatcher>
#include "global.h"



class QDomDocument;
class QDomElement;
//



/*!
 * This is a single open project which contains everything for that project, 
 * watching the file it was opened or saved with for changes made by any outside 
 * source. If the file this project is associated with is changed by an outside 
 * source a signal is emitted. This class also provides all generic information of 
 * a project along with being able to edit it. This generic information includes 
 * the name, scan directory, and scan filters. The block model for this project's 
 * block tree data and the ability to create a scan thread object for scanning and 
 * parsing is also provided. 
 */
class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   explicit Project(int type);
   explicit Project(const QString& path);
   bool isNew() const;
   bool isModified() const;
   int type() const;
   QString path() const;
   QString name() const;
   QString scanDirectory() const;
   QString scanFilters() const;
   std::unique_ptr<ScanThread> makeScanner() const;
   BlockModel* model();
   void setName(const QString& value);
   void setScanDirectory(const QString& path);
   void setScanFilters(const QString& value);
   void save();
   void saveAs(const QString& path);
signals:
   /*!
    * Signals that this project's name has changed. 
    */
   void nameChanged();
   /*!
    * Signals that this project has been modified and now has unsaved changes. 
    */
   void modified();
   /*!
    * Signals that this project has been saved and no longer has unsaved changes. 
    */
   void saved();
   /*!
    * Signals that this project's file has been changed by an outside source and no 
    * longer contains the last save of this project. 
    */
   void saveFileChanged();
private slots:
   void blockModified();
   void fileChanged();
private:
   void signalModified();
   QByteArray read();
   void convertScanDirectory(const QString& path);
   void readTypeElement(const QDomElement& element);
   void write(const QByteArray& data);
   void setFileHash(const QByteArray& bytes);
   void makeRoot();
   /*!
    * The tag name for the name element. 
    */
   static const char* _nameTag;
   /*!
    * The tag name for the type element. 
    */
   static const char* _typeTag;
   /*!
    * The tag name for the scan directory element. 
    */
   static const char* _scanDirectoryTag;
   /*!
    * The tag name for the scanning file filters element. 
    */
   static const char* _scanFiltersTag;
   /*!
    * The tag name for the root block element. 
    */
   static const char* _rootTag;
   /*!
    * The name of the id attribute. 
    */
   static const char* _idTag;
   /*!
    * The modification state of this project. True if this project has unsaved 
    * modifications or false otherwise. 
    */
   bool _modified {false};
   /*!
    * The project type for this project. 
    */
   int _type {-1};
   /*!
    * The path where this project's save file is located. 
    */
   QString _path;
   /*!
    * The name of this project. 
    */
   QString _name;
   /*!
    * The absolute canonical path of this project's scanning directory. 
    */
   QString _scanDirectory;
   /*!
    * The file filters this project uses to match files when scanning. 
    */
   QString _scanFilters;
   /*!
    * Pointer to this project's root block. 
    */
   AbstractBlock* _root;
   /*!
    * Pointer to this project's block model. 
    */
   BlockModel* _model;
   /*!
    * The hash value of this project's save file used to determine if an outside 
    * source wrote to said save file. 
    */
   QByteArray _hash;
};



#endif
