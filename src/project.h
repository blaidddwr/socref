#ifndef PROJECT_H
#define PROJECT_H
#include <memory>
#include <QFileSystemWatcher>
#include "global.h"



class QDomElement;



class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   explicit Project(int type);
   explicit Project(const QString& path);
   void save();
   void saveAs(const QString& path);
   void setName(const QString& name);
   QString name() const;
   QString path() const;
   int type() const;
   QString scanDirectory() const;
   void setScanDirectory(const QString& path);
   QString scanFilters() const;
   void setScanFilters(const QString& filters);
   bool isNew() const;
   bool isModified() const;
   BlockModel* model() const;
   std::unique_ptr<ScanThread> prepareScanner() const;
signals:
   void nameChanged();
   void modified();
   void saved();
   void changed();
private slots:
   void blockModified();
   void handleFileChanged();
private:
   void readTypeElement(const QDomElement& type);
   void signalModified();
   void setFileHash(const QByteArray& bytes);
   void makeRoot();
   static const char* _nameTag;
   static const char* _typeTag;
   static const char* _scandirectoryTag;
   static const char* _scanFiltersTag;
   static const char* _rootTag;
   static const char* _idTag;
   QString _path;
   QByteArray _hash;
   QString _name;
   int _type {-1};
   QString _scanDirectory;
   QString _scanFilters;
   bool _modified {false};
   AbstractBlock* _root;
   BlockModel* _model;
};



#endif
