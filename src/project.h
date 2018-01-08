#ifndef PROJECT_H
#define PROJECT_H
#include <QFileSystemWatcher>

#include "abstractblock.h"
#include "blockmodel.h"



class QDomElement;



class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   explicit Project(int type);
   explicit Project(const QString& path);
   Project& save();
   Project& saveAs(const QString& path);
   Project& setName(const QString& name);
   QString name() const { return _name; }
   QString path() const { return _path; }
   int type() const { return _type; }
   QString scanDirectory() const { return _scanDirectory; }
   Project& setScanDirectory(const QString& path);
   QString scanFilters() const { return _scanFilters; }
   Project& setScanFilters(const QString& filters);
   bool isNew() const { return _path.isEmpty(); }
   bool isModified() const { return _modified; }
   BlockModel* model() const { return _model; }
signals:
   void nameChanged();
   void modified();
   void saved();
   void changed();
private slots:
   void blockModified() { signalModified(); }
   void handleFileChanged();
private:
   void readTypeElement(const QDomElement& type);
   void signalModified();
   void setFileHash(const QByteArray& bytes);
   void createRoot();
   static const char* _nameString;
   static const char* _typeString;
   static const char* _scandirString;
   static const char* _filtersString;
   static const char* _rootString;
   static constexpr int _readTotal {5};
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
