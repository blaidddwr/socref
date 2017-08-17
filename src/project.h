#ifndef PROJECT_H
#define PROJECT_H
#include <QObject>



// @@ Project
class Project : public QObject
{
   Q_OBJECT
public:
   Project() {}
   Project(const QString& open) {}
};



#endif
