#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <memory>
#include <QString>
#include <QIcon>
#include "global.h"
#include "gui.h"
//



/*!
 * This represents the factory that can produce blocks for a specific project type. 
 * There should only be one instance of this class for each project type. Along 
 * with making new blocks this factory also gives basic information about each 
 * block type such as its display and element names. To maintain backwards 
 * compatibility all previously defined block types must maintain the specific 
 * integer that defines them. 
 */
class AbstractBlockFactory
{
public:
   /*!
    * Virtual destructor so implemented classes have their destructor called 
    * correctly. 
    */
   virtual ~AbstractBlockFactory() = default;
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
    * new block is expected to have a null data state. 
    *
    * @param type Block type which is made and returned. 
    *
    * @return Pointer to new block with given type. 
    */
   virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const = 0;
   /*!
    * This interface makes a new root block and returns its pointer. The root block is 
    * used as the root block for all projects of this project type. The new root block 
    * is expected to have a null data state. 
    *
    * @return Pointer to new root block for this project type. 
    */
   virtual std::unique_ptr<AbstractBlock> makeRootBlock() const = 0;
};



#endif
