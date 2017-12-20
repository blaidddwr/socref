#ifndef PROJECT_H
#define PROJECT_H
#include <QFileSystemWatcher>



class QXmlStreamReader;
class AbstractBlock;
class BlockModel;



class Project : public QFileSystemWatcher
{
   Q_OBJECT
public:
   explicit Project(int type);
   explicit Project(const QString& path);
   void save();
   void saveAs(const QString& path);
   void setName(const QString& name);
   QString name() const { return _name; }
   QString path() const { return _path; }
   int type() const { return _type; }
   QString scanDirectory() const { return _scanDirectory; }
   void setScanDirectory(const QString& path);
   QString scanFilters() const { return _scanFilters; }
   void setScanFilters(const QString& filters);
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
   void readTypeElement(QXmlStreamReader& xml);
   void signalModified();
   void setFileHash(const QByteArray& bytes);
   void createRoot();
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
