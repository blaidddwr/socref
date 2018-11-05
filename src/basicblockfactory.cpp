#include "basicblockfactory.h"
#include <QFile>
#include <socutil/sut_exceptions.h>
#include "basicblock.h"



using namespace Sut;
//



/*!
 * The name of the display attribute for XML basic block definitions. 
 */
const char* BasicBlockFactory::_displayTag {"display"};
/*!
 * The name of the root attribute for XML basic block definitions. 
 */
const char* BasicBlockFactory::_rootTag {"root"};
/*!
 * The tag name of the build list element for XML basic block definitions. 
 */
const char* BasicBlockFactory::_buildTag {"buildlist"};






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int BasicBlockFactory::size() const
{
   // If this factory has not been checked for consistency then do so. 
   if ( !_isChecked ) check();

   // Return the number of basic block definitions. 
   return _typeDisplayNames.size();
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BasicBlockFactory::name(int type) const
{
   // If this factory has not been checked for consistency then do so. 
   if ( !_isChecked ) check();

   // Make sure the given type is not out of range. 
   if ( type < 0 || type >= _typeDisplayNames.size() )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Given block type %1 is out of range.").arg(type));
      throw e;
   }

   // Return the display name of the given block type. 
   return _typeDisplayNames.at(type);
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param elementName See interface docs. 
 *
 * @return See interface docs. 
 */
int BasicBlockFactory::typeByElementName(const QString& elementName) const
{
   // If this factory has not been checked for consistency then do so. 
   if ( !_isChecked ) check();

   // Return the index of the given element name or -1 if it is not found. 
   return _typeElementNames.indexOf(elementName);
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @param isDefault See interface docs. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlockFactory::makeBlock(int type, bool isDefault) const
{
   // If this factory has not been checked for consistency then do so. 
   if ( !_isChecked ) check();

   // Make sure the given type is not out of range. 
   if ( type < 0 || type >= _typeDisplayNames.size() )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Given block type %1 is out of range.").arg(type));
      throw e;
   }

   // Create a new basic block, implemented or generic. 
   Sut::QPtr<BasicBlock> ret {makeBasicBlock(type)};
   if ( !ret ) ret.reset(new BasicBlock);

   // Initialize the new basic block with its XML definition and default state. 
   ret->initialize(type,this,_typeDefinitions.at(type),_buildLists.at(type),isDefault);

   // Return the new basic block. 
   return ret;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlockFactory::makeRootBlock() const
{
   // If this factory has not been checked for consistency then do so. 
   if ( !_isChecked ) check();

   // Make a new root block without default values and return it. 
   return makeBlock(_rootType,false);
}






/*!
 * Constructs a new basic block factory, reading in the XML configuration file at 
 * the given path. 
 *
 * @param xmlPath The path of the XML configuration file defining all basic block 
 *                types this new factory can produce. 
 */
BasicBlockFactory::BasicBlockFactory(const QString& xmlPath)
{
   // Read in the XML, then add all type definitions, and then build all build lists 
   // for each type defined. 
   read(xmlPath);
   buildLists();
}






/*!
 * This interface creates the class that implements the given basic block type, if 
 * any. If this is a generic basic block type that does not require a specialized 
 * class then a null pointer is returned. The default implementation always returns 
 * a null pointer. 
 *
 * @param type The block type of the basic block that is made if not generic. 
 *
 * @return Pointer to a new class implementation of the given basic block type or a 
 *         null pointer if this basic block type is generic. 
 */
Sut::QPtr<BasicBlock> BasicBlockFactory::makeBasicBlock(int type) const
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
void BasicBlockFactory::read(const QString& path)
{
   // Open the XML file from the given path, making sure it worked. 
   QFile file(path);
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::OpenError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot open file %1 for reading: %2")
                   .arg(path)
                   .arg(file.errorString()));
      throw e;
   }

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
 * Reads in a single basic block type definition from the given root XML element, 
 * appending it to this factory's list of basic block type definitions. 
 *
 * @param element The XML element that defines a basic block type. 
 */
void BasicBlockFactory::readDefinition(const QDomElement& element)
{
   // Make sure the given element has the display attribute. 
   if ( !element.hasAttribute(_displayTag) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Block type element definition %1 does not have required 'display' attribute.")
                   .arg(element.tagName()));
      throw e;
   }

   // Check to see if the given element has the root element. 
   if ( element.hasAttribute(_rootTag) )
   {
      // Make sure the root block type has not already been set. 
      if ( _rootType != -1 )
      {
         Exception::ReadError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Root block type is redefined to %1 when it is already defined as %2.")
                      .arg(element.tagName())
                      .arg(_typeElementNames.at(_rootType)));
         throw e;
      }

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
void BasicBlockFactory::buildLists()
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
 * Builds the next build list for this factory's basic block type definitions using 
 * the given XML element as the build list element and returning the built build 
 * list. 
 *
 * @param element Build list XML element used to build the build list. 
 *
 * @return Build list for the next block type definition using the given build list 
 *         element. 
 */
QList<int> BasicBlockFactory::buildList(const QDomElement& element)
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
         if ( index == -1 )
         {
            Exception::ReadError e;
            SUT_MARK_EXCEPTION(e);
            e.setDetails(QObject::tr("Unknown block type '%1' in build list.")
                         .arg(child.tagName()));
            throw e;
         }

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
 * then an exception is thrown. 
 */
void BasicBlockFactory::check() const
{
   // Iterate through all basic block type definitions of this factory. 
   for(int i = 0; i < _typeElementNames.size() ;++i)
   {
      // Make sure the element name matches between this factory's list and the element 
      // name interface. 
      if ( _typeElementNames.at(i) != elementName(i) )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Block type %1 has mismatched element names ('%2' != '%3').")
                      .arg(i)
                      .arg(_typeElementNames.at(i))
                      .arg(elementName(i)));
         throw e;
      }
   }

   // Mark this factory as having its consistency checked. 
   _isChecked = true;
}
