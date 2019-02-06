#ifndef PROJECTFACTORY_H
#define PROJECTFACTORY_H
#include "abstractprojectfactory.h"
//



/*!
 * This is the global project factory for this application, providing all factories 
 * and information needed for each project type. 
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
       * Defines the GLSL project type. 
       */
      ,GLSLType
      /*!
       * Defines the total number of all project types. 
       */
      ,Total
   };
public:
   virtual int size() const override final;
   virtual QString name(int type) const override final;
   virtual int typeByElementName(const QString& elementName) const override final;
   virtual QString elementName(int type) const override final;
   virtual QString defaultFilters(int type) const override final;
   virtual Soc::Ut::QPtr<QDialog> makeSettings(int type) const override final;
   virtual const AbstractBlockFactory& blockFactory(int type) const override final;
   virtual Soc::Ut::QPtr<AbstractParserFactory> makeParserFactory(int type, const AbstractBlock* root) const override final;
};



#endif
