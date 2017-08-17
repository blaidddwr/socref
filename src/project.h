#ifndef PROJECT_H
#define PROJECT_H
#include <QObject>



class AbstractBlockFactory;
class BlockModel;



// @@ Project
class Project : public QObject
{
   Q_OBJECT
public:
   Project(int type);
   Project(const QString& open);
   void save(const QString& path) const;
   void setName(const QString& name);
   QString getName() const;
   void retain();
   void release();
private:
   QString _name;
   AbstractBlockFactory* _factory {nullptr};
   BlockModel* _model {nullptr};
};



#endif
