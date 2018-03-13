#ifndef PROJECTFACTORY_H
#define PROJECTFACTORY_H
#include "abstractprojectfactory.h"
//



/*!
 */
class ProjectFactory : public AbstractProjectFactory
{
public:
   /*!
    */
   enum 
   {
      /*!
       */
      CppQtType
      /*!
       */
      ,Total
   };
   virtual int size() const override final;
   virtual QString name(int type) const override final;
   virtual QString defaultFilters(int type) const override final;
   virtual const AbstractBlockFactory& blockFactory(int type) const override final;
   virtual std::unique_ptr<AbstractParserFactory> makeParserFactory(int type, AbstractBlock* root) const override final;
};



#endif
