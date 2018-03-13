#ifndef PROJECT_H
#define PROJECT_H
#include <memory>
#include <QFileSystemWatcher>
#include "global.h"



class QDomElement;
//



/*!
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
