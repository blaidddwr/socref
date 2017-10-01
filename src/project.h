#ifndef PROJECT_H
#define PROJECT_H
#include <QObject>
#include <QDateTime>



class QFileSystemWatcher;
class AbstractBlockFactory;
class BlockModel;
class QXmlStreamReader;



//@@
class Project : public QObject
{
   Q_OBJECT
public:
   Project(int type);
   Project(const QString& path);
   void save() const;
   void saveAs(const QString& path);
   //@@
   void setName(const QString& name) { _name = name; }
   //@@
   QString getName() const { return _name; }
   //@@
   int getType() const { return _type; }
   //@@
   QString getScanDirectory() const { return _scanDirectory; }
   void setScanDirectory(const QString& path);
   //@@
   QString getScanFilters() const { return _scanFilters; }
   //@@
   void setScanFilters(const QString& filters) { _scanFilters = filters; }
   //@@
   BlockModel* getModel() const { return _model; }
   //@@
   bool isNew() const { return _path.isEmpty(); }
   //@@
   bool isModified() const { return _modified; }
signals:
   //@@
   void modified();
   //@@
   void saved();
   //@@
   void fileChanged();
private slots:
   void blockModified();
   //@@
   void saveFileChanged() { emit fileChanged(); }
private:
   void readTypeElement(QXmlStreamReader& xml);
   QString _path;
   QString _name;
   int _type {-1};
   QString _typeName;
   const AbstractBlockFactory* _factory {nullptr};
   QString _scanDirectory;
   QString _scanFilters;
   bool _modified {false};
   BlockModel* _model;
   QFileSystemWatcher* _fileWatcher {nullptr};
   int _fileChangeTicks {0};
};



#endif
