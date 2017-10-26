#ifndef PROJECTFACTORY_H
#define PROJECTFACTORY_H
#include "abstractprojectfactory.h"



class ProjectFactory : public AbstractProjectFactory
{
public:
   enum {
      CppQtType = 0
      ,Total
   };
   virtual int getSize() const override final;
   virtual QString getName(int type) const override final;
   virtual QString getDefaultFilters(int type) const override final;
   virtual const AbstractBlockFactory& getBlockFactory(int type) const override final;
};



//@@
inline int ProjectFactory::getSize() const { return Total; }



#endif
