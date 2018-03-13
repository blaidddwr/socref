#ifndef ABSTRACTPROJECTFACTORY_H
#define ABSTRACTPROJECTFACTORY_H
#include <memory>
#include <QString>
#include "singletonfactory.h"
#include "global.h"
//



/*!
 */
class AbstractProjectFactory : public SingletonFactory<AbstractProjectFactory>
{
public:
   /*!
    */
   virtual int size() const = 0;
   /*!
    *
    * @param type  
    */
   virtual QString name(int type) const = 0;
   /*!
    *
    * @param type  
    */
   virtual QString defaultFilters(int type) const = 0;
   /*!
    *
    * @param type  
    */
   virtual const AbstractBlockFactory& blockFactory(int type) const = 0;
   /*!
    *
    * @param type  
    *
    * @param root  
    */
   virtual std::unique_ptr<AbstractParserFactory> makeParserFactory(int type, AbstractBlock* root) const = 0;
};



#endif
