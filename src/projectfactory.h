#ifndef PROJECTFACTORY_H
#define PROJECTFACTORY_H
#include "abstractprojectfactory.h"
//



/*!
 * This implements the abstract project factory class that provides all factories 
 * and information needed for a project type. 
 */
class ProjectFactory : public AbstractProjectFactory
{
public:
   /*!
    * Defines all project types ending with the total size of all project types. 
    */
   enum 
   {
      /*!
       * Defines the C++/Qt project type. 
       */
      CppQtType
      /*!
       * Defines the total number of all project types. 
       */
      ,Total
   };
   virtual int size() const override final;
   virtual QString name(int type) const override final;
   virtual QString defaultFilters(int type) const override final;
   virtual std::unique_ptr<QDialog> makeSettings(int type) const override final;
   virtual const AbstractBlockFactory& blockFactory(int type) const override final;
   virtual std::unique_ptr<AbstractParserFactory> makeParserFactory(int type, AbstractBlock* root) const override final;
};



#endif
