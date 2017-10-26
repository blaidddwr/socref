#ifndef PROJECT_H
#define PROJECT_H
#include <QFileSystemWatcher>



class QXmlStreamReader;
class AbstractBlock;
class BlockModel;



//@@
class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   explicit Project(int type);
   explicit Project(const QString& path);
   void save();
   void saveAs(const QString& path);
   void setName(const QString& name);
   QString getName() const;
   QString getPath() const;
   int getType() const;
   QString getScanDirectory() const;
   void setScanDirectory(const QString& path);
   QString getScanFilters() const;
   void setScanFilters(const QString& filters);
   bool isNew() const;
   bool isModified() const;
   BlockModel* getModel() const;
signals:
   //@@
   void nameChanged();
   //@@
   void modified();
   //@@
   void saved();
   //@@
   void changed();
private slots:
   void blockModified();
   void handleFileChanged();
private:
   void readTypeElement(QXmlStreamReader& xml);
   void signalModified();
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



//@@
inline QString Project::getName() const { return _name; }

//@@
inline QString Project::getPath() const { return _path; }

//@@
inline int Project::getType() const { return _type; }

//@@
inline QString Project::getScanDirectory() const { return _scanDirectory; }

//@@
inline QString Project::getScanFilters() const { return _scanFilters; }

//@@
inline bool Project::isNew() const { return _path.isEmpty(); }

//@@
inline bool Project::isModified() const { return _modified; }

//@@
inline BlockModel* Project::getModel() const { return _model; }

//@@
inline void Project::blockModified() { signalModified(); }



#endif
