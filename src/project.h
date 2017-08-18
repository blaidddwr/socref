#ifndef PROJECT_H
#define PROJECT_H
#include <QObject>



class QFileSystemWatcher;
class AbstractBlockFactory;
class BlockModel;



// @@ Project
class Project : public QObject
{
   Q_OBJECT
public:
   Project(int type);
   Project(const QString& path);
   bool setPath(const QString& path);
   void reload();
   void save() const;
   void setName(const QString& name);
   QString getName() const;
   int getType() const;
   QString getScanDirectory() const;
   void setScanDirectory(const QString& path);
   QString getScanFilters() const;
   void setScanFilters(const QString& filters);
   BlockModel* getModel() const;
   bool isNew() const;
   bool isModified() const;
signals:
   // @@ Project::modified()
   void modified();
   // @@ Project::saved()
   void saved();
   // @@ Project::fileChanged()
   void fileChanged();
private slots:
   void blockModified() {}
   void saveFileChanged() {}
private:
   QString _path;
   QString _name;
   int _type;
   QString _scanDirectory;
   QString _scanFilters;
   bool _modified {false};
   const AbstractBlockFactory& _factory;
   BlockModel* _model;
   QFileSystemWatcher* _fileWatcher {nullptr};
   QByteArray _fileHash;
};



#endif
