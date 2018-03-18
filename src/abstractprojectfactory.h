#ifndef ABSTRACTPROJECTFACTORY_H
#define ABSTRACTPROJECTFACTORY_H
#include <memory>
#include <QString>
#include "singletonfactory.h"
#include "global.h"
//



/*!
 * This represents the factory that defines all other factories and information 
 * to define a project type. A project type is defined as a specific programming 
 * language that a project references. This is a global singleton class so only 
 * one instance of this factory is available for the entire program. A new 
 * factory instance must be set at initialization of the program and should not 
 * be set again after that. To maintain backwards compatibility all previously 
 * defined project types must maintain the specific integer that defines them. 
 */
class AbstractProjectFactory : public SingletonFactory<AbstractProjectFactory>
{
public:
   /*!
    * This interface returns the number of project types that exist. Project types 
    * must be defined for all integers from 0 to one less than the number of 
    * project types returned by this interface. 
    *
    * @return Number of project types that exist. 
    */
   virtual int size() const = 0;
   /*!
    * This interface returns the display name for the given project type. 
    *
    * @param type Project type whose display name is returned. 
    *
    * @return Display name for the given project type. 
    */
   virtual QString name(int type) const = 0;
   /*!
    * This interface returns the default filters for the scanning of source files 
    * the given project type possesses. 
    *
    * @param type Project type whose default file filters are returned. 
    *
    * @return Default file filters for the given project type. 
    */
   virtual QString defaultFilters(int type) const = 0;
   /*!
    * This interface returns a read only reference to the block factory for the 
    * given project type. 
    *
    * @param type Project type whose block factory is returned as a reference. 
    *
    * @return Read only reference to block factory for the given project type. 
    */
   virtual const AbstractBlockFactory& blockFactory(int type) const = 0;
   /*!
    * This interface makes a new parser factory for the given project type with the 
    * given block pointer as the root of an existing project to be parsed. 
    *
    * @param type Project type whose parser factory type is made and returned. 
    *
    * @param root Pointer to root block of existing project that will be parsed 
    *             with the returned parser factory. 
    *
    * @return Pointer to created parser factory for the given project type. 
    */
   virtual std::unique_ptr<AbstractParserFactory> makeParserFactory(int type, AbstractBlock* root) const = 0;
};



#endif
