#ifndef ABSTRACTPROJECTFACTORY_H
#define ABSTRACTPROJECTFACTORY_H
#include <QString>

#include "singletonfactory.h"
#include "abstractblockfactory.h"



class AbstractProjectFactory : public SingletonFactory<AbstractProjectFactory>
{
public:
   virtual int size() const = 0;
   virtual QString name(int type) const = 0;
   virtual QString defaultFilters(int type) const = 0;
   virtual const AbstractBlockFactory& blockFactory(int type) const = 0;
};



#endif
