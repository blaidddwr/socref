#ifndef ABSTRACTPROJECTFACTORY_H
#define ABSTRACTPROJECTFACTORY_H
#include <QString>

#include "singletonfactory.h"



class AbstractBlockFactory;



//@@
class AbstractProjectFactory : public SingletonFactory<AbstractProjectFactory>
{
public:
   //@@
   virtual int getSize() const = 0;
   //@@
   virtual QString getName(int type) const = 0;
   //@@
   virtual QString getDefaultFilters(int type) const = 0;
   //@@
   virtual const AbstractBlockFactory& getBlockFactory(int type) const = 0;
};



#endif
