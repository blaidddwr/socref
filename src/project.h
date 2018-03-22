#ifndef PROJECT_H
#define PROJECT_H
#include <memory>
#include <QFileSystemWatcher>
#include "global.h"



class QDomElement;
//



/*!
 * This is a single open project which contains everything for that project, 
 * watching the file it was opened or saved with for changes made by any outside 
 * source. If the file this project is associated with is changed by an outside 
 * source a signal is emitted. This class also provides all generic information 
 * of a project along with being able to edit it. This generic information 
 * includes the name, scan directory, and scan filters. The block model for this 
 * project's block tree data and the ability to create a scan thread object for 
 * scanning and parsing is also provided. 
 */
class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   Project(int type);
   Project(const QString& path);
   bool isNew() const;
   bool isModified() const;
   int type() const;
   QString path() const;
   BlockModel* model();
   QString name() const;
   void setName(const QString& newName);
   QString scanDirectory() const;
   void setScanDirectory(const QString& newPath);
   QString scanFilters() const;
   void setScanFilters(const QString& newFilters);
   std::unique_ptr<ScanThread> makeScanner() const;
   void save();
   void saveAs(const QString& path);
signals:
   /*!
    */
   void nameChanged();
   /*!
    */
   void modified();
   /*!
    */
   void saved();
   /*!
    */
   void changed();
private slots:
   void blockModified();
   void fileChanged();
private:
   void signalModified();
   void setFileHash(const QByteArray& bytes);
   void readTypeElement(const QDomElement& element);
   void makeRoot();
   /*!
    */
   static const char* _nameTag;
   /*!
    */
   static const char* _typeTag;
   /*!
    */
   static const char* _scanDirectoryTag;
   /*!
    */
   static const char* _scanFiltersTag;
   /*!
    */
   static const char* _rootTag;
   /*!
    */
   static const char* _idTag;
   /*!
    */
   bool _modified {false};
   /*!
    */
   int _type {-1};
   /*!
    */
   QString _path;
   /*!
    */
   QString _name;
   /*!
    */
   QString _scanDirectory;
   /*!
    */
   QString _scanFilters;
   /*!
    */
   AbstractBlock* _root;
   /*!
    */
   BlockModel* _model;
   /*!
    */
   QByteArray _hash;
};



#endif
