#include "basic_blockfactory.h"
#include <QFile>
#include "basic_block.h"



namespace Basic
{



/*!
 * The name of the display attribute for XML basic block definitions.
 */
const QString BlockFactory::_displayTag {"display"};
/*!
 * The name of the root attribute for XML basic block definitions.
 */
const QString BlockFactory::_rootTag {"root"};
/*!
 * The tag name of the build list element for XML basic block definitions.
 */
const QString BlockFactory::_buildTag {"buildlist"};






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @return See interface docs.
 */
int BlockFactory::size() const
{
   // If this factory has not been checked for consistency then do so.
   Q_ASSERT(check());

   // Return the number of basic block definitions.
   return _typeDisplayNames.size();
}






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @param type See interface docs.
 *
 * @return See interface docs.
 */
QString BlockFactory::name(int type) const
{
   // If this factory has not been checked for consistency then do so.
   Q_ASSERT(check());

   // Make sure the given type is not out of range.
   Q_ASSERT(type >= 0);
   Q_ASSERT(type < _typeDisplayNames.size());

   // Return the display name of the given block type.
   return _typeDisplayNames.at(type);
}






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @param elementName See interface docs.
 *
 * @return See interface docs.
 */
int BlockFactory::typeByElementName(const QString& elementName) const
{
   // If this factory has not been checked for consistency then do so.
   Q_ASSERT(check());

   // Return the index of the given element name or -1 if it is not found.
   return _typeElementNames.indexOf(elementName);
}






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @param type See interface docs.
 *
 * @param isDefault See interface docs.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Abstract::Block> BlockFactory::createBlock(int type, bool isDefault) const
{
   // If this factory has not been checked for consistency then do so.
   Q_ASSERT(check());

   // Make sure the given type is not out of range.
   Q_ASSERT(type >= 0);
   Q_ASSERT(type < _typeDisplayNames.size());

   // Create a new basic block, implemented or generic.
   Soc::Ut::QPtr<Block> ret {createBasicBlock(type)};
   if ( !ret ) ret.reset(new Block);

   // Initialize the new basic block with its XML definition and default state.
   ret->initialize(type,this,_typeDefinitions.at(type),_buildLists.at(type),isDefault);

   // Return the new basic block.
   return std::move(ret);
}






/*!
 * Implements _Abstract::BlockFactory_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Abstract::Block> BlockFactory::createRootBlock() const
{
   // If this factory has not been checked for consistency then do so.
   Q_ASSERT(check());

   // Make a new root block without default values and return it.
   return createBlock(_rootType,false);
}






/*!
 * Constructs a new basic block factory, reading in the XML configuration file
 * at the given path.
 *
 * @param xmlPath The path of the XML configuration file defining all basic
 *                block types this new factory can produce.
 */
BlockFactory::BlockFactory(const QString& xmlPath)
{
   // Read in the XML, then add all type definitions, and then build all build lists
   // for each type defined.
   read(xmlPath);
   buildLists();
}






/*!
 * This interface creates and returns the class that implements the given basic
 * block type, if any. If this is a generic basic block type that does not
 * require a specialized class then a null pointer is returned. The default
 * implementation always returns a null pointer.
 *
 * @param type The block type of the basic block that is made if not generic.
 *
 * @return New instance of class implementation of the given basic block type or
 *         a null pointer if this basic block type is generic.
 */
Soc::Ut::QPtr<Block> BlockFactory::createBasicBlock(int type) const
{
   Q_UNUSED(type)

   // Return a null pointer.
   return nullptr;
}






/*!
 * Reads in the XML configuration file at the given path.
 *
 * @param path The path of the XML configuration file defining all basic block
 *             types this new factory can produce.
 */
void BlockFactory::read(const QString& path)
{
   // Open the XML file from the given path, making sure it worked.
   QFile file(path);
   bool ok {file.open(QIODevice::ReadOnly)};
   Q_ASSERT(ok);

   // Create a new XML document and set its contents to the opened XML file.
   QDomDocument document;
   document.setContent(&file);

   // Iterate through all children of the XML document.
   QDomNode node {document.documentElement().firstChild()};
   while ( !node.isNull() )
   {
      // If the current node is an element then read it in as a definition.
      if ( node.isElement() ) readDefinition(node.toElement());

      // Move to the next sibling node.
      node = node.nextSibling();
   }
}






/*!
 * Reads in a single basic block type definition from the given root XML
 * element, appending it to this factory's list of basic block type definitions.
 *
 * @param element The XML element that defines a basic block type.
 */
void BlockFactory::readDefinition(const QDomElement& element)
{
   // Make sure the given element has the display attribute.
   Q_ASSERT(element.hasAttribute(_displayTag));

   // Check to see if the given element has the root element.
   if ( element.hasAttribute(_rootTag) )
   {
      // Make sure the root block type has not already been set.
      Q_ASSERT(_rootType == -1);

      // Set the root block type to the basic block type definition about to be added.
      _rootType = _typeDisplayNames.size();
   }

   // Add the new basic block type definition of this factory's list.
   _typeDisplayNames << element.attribute(_displayTag);
   _typeElementNames << element.tagName();
   _typeDefinitions << element;
}






/*!
 * Builds the build lists for all basic block types that his basic block factory
 * defines.
 */
void BlockFactory::buildLists()
{
   // Iterate through all type definitions of this factory.
   for (auto element: qAsConst(_typeDefinitions))
   {
      // Initialize the next build list.
      QList<int> list;

      // Iterate through all nodes of the type definition.
      QDomNode node {element.firstChild()};
      while ( !node.isNull() )
      {
         // If the node is an element and matches the build list tag name then build a
         // build list using the found build list XML element.
         if ( node.isElement() )
         {
            QDomElement child {node.toElement()};
            if ( child.tagName() == _buildTag )
            {
               list = buildList(child);
            }
         }

         // Move to the next sibling.
         node = node.nextSibling();
      }

      // Add the next build list to the list of type definitions for this factory.
      _buildLists << list;
   }
}






/*!
 * Builds the next build list for this factory's basic block type definitions
 * using the given XML element as the build list element and returning the built
 * build list.
 *
 * @param element Build list XML element used to build the build list.
 *
 * @return Build list for the next block type definition using the given build
 *         list element.
 */
QList<int> BlockFactory::buildList(const QDomElement& element)
{
   // Initialize the return build list that will be returned.
   QList<int> ret;

   // Iterate through all children nodes of the given element.
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if this node is an element.
      if ( node.isElement() )
      {
         // Find the index of the block type the element is referencing, checking to make
         // sure it is valid.
         QDomElement child {node.toElement()};
         int index {_typeElementNames.indexOf(child.tagName())};
         Q_ASSERT(index != -1);

         // Append the block type to the build list.
         ret << index;
      }

      // Move to the next sibling.
      node = node.nextSibling();
   }

   // Return the built build list.
   return ret;
}






/*!
 * Makes sure this factory's list of basic block definitions match with this
 * factory's implementation's element name interface. If a mismatch is detected
 * then false is returned.
 *
 * @return True is no mismatch is detected or false if a mismatch is detected.
 */
bool BlockFactory::check() const
{
   // If consistency has already been checked then return true immediately.
   if ( _isChecked ) return true;

   // Iterate through all basic block type definitions of this factory.
   for(int i = 0; i < _typeElementNames.size() ;++i)
   {
      // Make sure the element name matches between this factory's list and the element
      // name interface, returning false if there is a mismatch.
      if ( _typeElementNames.at(i) != elementName(i) ) return false;
   }

   // Mark this factory as having its consistency checked and return true.
   _isChecked = true;
   return true;
}

}
