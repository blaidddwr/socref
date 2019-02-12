#ifndef ABSTRACT_PROJECTFACTORY_H
#define ABSTRACT_PROJECTFACTORY_H
#include <QString>
#include <socutil/soc_ut_singletonfactory.h>
#include <socutil/soc_ut_qptr.h>
#include "abstract.h"
#include "global.h"



class QDialog;
//



namespace Abstract
{
   /*!
    * This represents the factory that provides all other factories and information to 
    * define a project type. A project type is defined as a specific programming 
    * language that a project references. This is a global singleton class so only one 
    * instance of this factory is available for the entire application. A new factory 
    * instance must be set at initialization of the program and should not be set 
    * again after that. To maintain backwards compatibility all previously defined 
    * project type element names must never change and the two element methods must 
    * mirror one another. 
    */
   class ProjectFactory : public Soc::Ut::SingletonFactory<ProjectFactory>
   {
   public:
      /*!
       * Virtual destructor so implemented classes have their destructor called 
       * correctly. 
       */
      virtual ~ProjectFactory() = default;
      /*!
       * This interface returns the number of project types that exist. Project types 
       * must be defined for all integers from 0 to one less than the number of project 
       * types returned by this interface. 
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
       * This interface returns the project type with the given element name. If no 
       * project type exists with the given name then -1 is returned. 
       *
       * @param elementName The given element name of the project type that is returned 
       *                    if found. 
       *
       * @return The integer value of the project type with the given element name if 
       *         found or -1 if no project type is found with the given element name. 
       */
      virtual int typeByElementName(const QString& elementName) const = 0;
      /*!
       * This interface returns the element name for the given project type. The element 
       * name is used as the tag name in the XML project file to determine what project 
       * type it is. 
       *
       * @param type Project type whose element name is returned. 
       *
       * @return Element name for the given project type. 
       */
      virtual QString elementName(int type) const = 0;
      /*!
       * This interface returns the default filters for the scanning of source files the 
       * given project type possesses. 
       *
       * @param type Project type whose default file filters are returned. 
       *
       * @return Default file filters for the given project type. 
       */
      virtual QString defaultFilters(int type) const = 0;
      /*!
       * This interface makes a new settings dialog for the given project type. The 
       * settings dialog for a project type should contain all general settings for that 
       * project type, saving the settings in the qt settings class. 
       *
       * @param type Project type whose settings dialog is returned. 
       *
       * @return Pointer to the new settings dialog for the given project type. 
       */
      virtual Soc::Ut::QPtr<QDialog> makeSettings(int type) const = 0;
      /*!
       * This interface returns a read only reference to the block factory for the given 
       * project type. 
       *
       * @param type Project type whose block factory is returned as a reference. 
       *
       * @return Read only reference to block factory for the given project type. 
       */
      virtual const Abstract::BlockFactory& blockFactory(int type) const = 0;
      /*!
       * This interface creates and returns a mapping of scanner objects for the given 
       * root block that will be used for scanning all source files that match any of the 
       * mapped scanner objects returned. The key for the returned mapping are file names 
       * that are matched with any source files found in the scanning directory. 
       * Ownership of all returned scanner objects are released once returned. 
       *
       * @param root Root block of existing project that will be parsed with the returned 
       *             mapping of scanner objects. 
       *
       * @return Mapping of scanner objects used for scanning source files of the given 
       *         root block's project. 
       */
      virtual QMap<QString,Scanner*> createScannerMap(const Block* root) const = 0;
   };
}



#endif
