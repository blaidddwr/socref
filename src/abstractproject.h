#ifndef ABSTRACTPROJECT_H
#define ABSTRACTPROJECT_H
#include <QObject>



class AbstractBlockFactory;



// @@ AbstractProject
class AbstractProject : public QObject
{
   Q_OBJECT
public:
   virtual AbstractBlockFactory& getFactory() const = 0;
};



#endif
