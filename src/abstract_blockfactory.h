#ifndef ABSTRACT_BLOCKFACTORY_H
#define ABSTRACT_BLOCKFACTORY_H
#include <QString>
#include <socutil/soc_ut_qptr.h>
#include "abstract.h"
//



namespace Abstract
{
   /*!
    * This represents the factory that can produce blocks for a specific project type. 
    * There should only be one instance of this class for each project type. Along 
    * with making new blocks this factory also gives basic information about each 
    * block type such as its display and element names. The two element name functions 
    * must mirror one another in an implementation or undefined behavior will occur. 
    */
   class BlockFactory
   {
   public:
      /*!
       * Virtual destructor so implemented classes have their destructor called 
       * correctly. 
       */
      virtual ~BlockFactory() = default;
      /*!
       * This interface returns the project type of this block factory. 
       *
       * @return Project type. 
       */
      virtual int type() const = 0;
      /*!
       * This interface returns the number of block types that exist for this factory. 
       * Block types must be defined for all integers from 0 to one less than the number 
       * of block types returned by this interface. 
       *
       * @return Number of block types that exist for this factory. 
       */
      virtual int size() const = 0;
      /*!
       * This interface returns the display name for the given block type. 
       *
       * @param type Block type whose display name is returned. 
       *
       * @return Display name for the given block type. 
       */
      virtual QString name(int type) const = 0;
      /*!
       * This interface returns the integer type of the block with the given element 
       * name. If no block type exists with the given name then -1 is returned. 
       *
       * @param elementName The given element name whose integer type is returned if 
       *                    found. 
       *
       * @return The integer type of the block with the given element name if found or -1 
       *         if no block type is found with that element name. 
       */
      virtual int typeByElementName(const QString& elementName) const = 0;
      /*!
       * This interface returns the element name for the given block type. The element 
       * name is used as the tag name in XML elements for reading/writing of blocks. 
       *
       * @param type Block type whose element name is returned. 
       *
       * @return Element name for the given block type. 
       */
      virtual QString elementName(int type) const = 0;
      /*!
       * This interface makes a new block of the given type and returns its pointer. The 
       * new block is expected to have either a null data or new default state, depending 
       * on the given flag. 
       *
       * @param type Block type which is made and returned. 
       *
       * @param isDefault True if the new block is initialized to a default state or 
       *                  false if it is uninitialized in a null state. 
       *
       * @return Pointer to new block with given type. 
       */
      virtual Soc::Ut::QPtr<Abstract::Block> makeBlock(int type, bool isDefault) const = 0;
      /*!
       * This interface makes a new root block and returns its pointer. The root block is 
       * used as the root block for all projects of this project type. The new root block 
       * is expected to have a null data state. 
       *
       * @return Pointer to new root block for this project type. 
       */
      virtual Soc::Ut::QPtr<Abstract::Block> makeRootBlock() const = 0;
   };
}



#endif
