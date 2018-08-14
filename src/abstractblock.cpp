#include "abstractblock.h"
#include <QDomDocument>
#include <QMap>
#include <socutil/sut_exceptions.h>
#include "abstractblockfactory.h"



using namespace std;
using namespace Sut;
//



/*!
 * The name for version attributes. 
 */
const char* AbstractBlock::_versionTag {"version"};
/*!
 * The tag name for data elements. 
 */
const char* AbstractBlock::_dataTag {"data"};
/*!
 * The name for type attributes. 
 */
const char* AbstractBlock::_typeTag {"type"};






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Read only pointer to the root block. 
 */
const AbstractBlock* AbstractBlock::root() const
{
   // Iterate up the tree of parents until the root pointer is found. 
   const AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();

   // Return the root pointer. 
   return root;
}






/*!
 * This returns a pointer to the parent of this block. If this block is the root 
 * block then null is returned. 
 *
 * @return If this block is the root then a null pointer, else a pointer to this 
 *         block's parent. 
 */
AbstractBlock* AbstractBlock::parent() const
{
   // Return the qt object of this object cast as an abstract block. 
   return qobject_cast<AbstractBlock*>(QObject::parent());
}






/*!
 * This returns the number of children this block contains. 
 *
 * @return Number of children this block contains. 
 */
int AbstractBlock::size() const
{
   // Return this block's child list size. 
   return _children.size();
}






/*!
 * This returns a read only reference to this block's list of children blocks. The 
 * list is a list of pointers to all child blocks. 
 *
 * @return Reference to this block's list of children. 
 */
const QList<AbstractBlock*>& AbstractBlock::list() const
{
   // Return a constant reference to this block's internal children list. 
   return _children;
}






/*!
 * This gets the index where the child with the given pointer is stored in this 
 * block's list of children. If no child with the given pointer is found then -1 is 
 * returned. 
 *
 * @param pointer Pointer of child block to match in this block's list and return 
 *                its index. 
 *
 * @return If a match is found then the index of the child with the given pointer, 
 *         else -1. 
 */
int AbstractBlock::indexOf(AbstractBlock* pointer) const
{
   // Return the index of the given child pointer using the qt method of this block's 
   // child list. 
   return _children.indexOf(pointer);
}






/*!
 * This returns a pointer to this block's child with the given index. If the index 
 * is out of range an exception is thrown. 
 *
 * @param index Index of the child whose pointer is returned. 
 *
 * @return Pointer to this block's child with given index. 
 */
AbstractBlock* AbstractBlock::get(int index) const
{
   // Make sure the given index is within range. 
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get child %1 when only %2 children exist.")
                   .arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // Return a pointer to this block's child with the given index. 
   return _children.at(index);
}






/*!
 * Makes an exact copy of this block. This uses other virtual functions to create a 
 * copy of this block. The new block returned has no parent. 
 *
 * @return Exact copy of this block's data as a new block. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::makeCopy() const
{
   // Create a new blank block of the same type as this block. 
   unique_ptr<AbstractBlock> ret {makeBlank()};

   // Make sure the new blank block is the same type as this block. 
   if ( type() != ret->type() )
   {
      Exception::LogicError e;
      e.setDetails(tr("Cannot copy data from a different block type."));
      throw e;
   }

   // Copy this block's children and data to the new block. 
   ret->copyChildren(this);
   ret->copyDataFrom(this);

   // Return the new block. 
   return ret;
}






/*!
 * Tests if this block contains any children of the given type. 
 *
 * @param type The block type to search for within this block's list of children. 
 *
 * @return True if this block contains any children of the given type, else false. 
 */
bool AbstractBlock::containsType(int type) const
{
   // Iterate through list of this block's children. 
   for (auto child : qAsConst(_children))
   {
      // If the child matches the given type then return true. 
      if ( child->type() == type ) return true;
   }

   // Return false because no child was found with the given type. 
   return false;
}






/*!
 * Tests if this block contains any children of any type given. 
 *
 * @param types List of block types to search for within this block's list of 
 *              children. 
 *
 * @return True if this block contains any children of any type given, else false. 
 */
bool AbstractBlock::containsType(const QList<int>& types) const
{
   // Iterate through list of this block's children. 
   for (auto child : qAsConst(_children))
   {
      // If the child matches any of the given types then return true. 
      if ( types.contains(child->type()) ) return true;
   }

   // Return false because no child was found with the given type. 
   return false;
}






/*!
 * Writes out this block's data as an XML element. This includes all children as 
 * child XML elements which have their write functions called recursively. 
 *
 * @param document  
 *
 * @return XML element that stores this block's data and all children underneath 
 *         it. 
 */
QDomElement AbstractBlock::write(QDomDocument& document) const
{
   // Create the data element for this bock's data. 
   QDomElement data {writeData(document)};

   // Create a new element that will contain this block's data and all children. 
   QDomElement ret {document.createElement(factory().elementName(type()))};

   // Set the type attribute and add the data element for this bock. 
   ret.setAttribute(_typeTag,type());
   ret.appendChild(data);

   // Add all of this block's children elements. 
   for (auto child : _children) ret.appendChild(child->write(document));

   // Return the element containing all data and children of this block. 
   return ret;
}






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Pointer to the root block. 
 */
AbstractBlock* AbstractBlock::root()
{
   // Iterate up the tree of parents until the root pointer is found. 
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();

   // Return the root pointer. 
   return root;
}






/*!
 * Sets the value of the field with the given index for this block. If the given 
 * index is out of range then an exception is thrown. 
 *
 * @param index Index of the field whose value is set to the given value. 
 *
 * @param value Value that is set to the field with the given index. 
 */
void AbstractBlock::setField(int index, const QVariant& value)
{
   // Make sure the given index is within range. 
   if ( index < 0 || index >= fieldSize() )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given block field %1 is out of range (%2 total).")
                   .arg(index)
                   .arg(fieldSize()));
      throw e;
   }

   // Make sure the new value for the field is different from the currently set 
   // value. 
   if ( value != field(index) )
   {
      // Call the appropriate interfaces to update the field's value. 
      quietlySetField(index,value);

      // If this block is not currently being read in from a file then notify this field 
      // has been modified. 
      if ( !_readIn ) fieldModified(index);
   }
}






/*!
 * Moves the child block with the given index up the list by one where the very top 
 * of the list is index 0. If the given index is already at the top of the list or 
 * out of range then this does nothing. 
 *
 * @param index The index of the child block moved up by one. 
 */
void AbstractBlock::moveUp(int index)
{
   // Make sure the given index is within range and not already at the top of the 
   // list. 
   if ( index < 1 || index >= _children.size() ) return;

   // Swap the child pointer at the given index with the pointer directly above it in 
   // this block's child list and notify of modification. 
   std::swap(_children[index - 1],_children[index]);
   notifyModified();

   // Starting with this block iterate up the tree of parents, calling their child 
   // moved interface, until the root block is reached or the child moved interface 
   // returns false. 
   AbstractBlock* notify {this};
   while ( notify && notify->childMoved(_children.at(index - 1)) ) notify = notify->parent();
}






/*!
 * Moves the child block with the given index down the list by one where the very 
 * top of the list is index 0. If the given index is already at the bottom of the 
 * list or out of range then this does nothing. 
 *
 * @param index The index of the child block moved down by one. 
 */
void AbstractBlock::moveDown(int index)
{
   // Make sure the given index is within range and not already at the bottom of the 
   // list. 
   if ( index < 0 || index >= (_children.size() - 1) ) return;

   // Swap the child pointer at the given index with the pointer directly below it in 
   // this block's child list and notify of modification. 
   std::swap(_children[index],_children[index + 1]);
   notifyModified();

   // Starting with this block iterate up the tree of parents, calling their child 
   // moved interface, until the root block is reached or the child moved interface 
   // returns false. 
   AbstractBlock* notify {this};
   while ( notify && notify->childMoved(_children.at(index + 1)) ) notify = notify->parent();
}






/*!
 * Inserts a new child block into this block's list of children at the given index. 
 * If the given index is less than 0 it is prepended to this block's list, else if 
 * the given index is greater than this block's list size then it is appended. If 
 * the given pointer to the new block is null or it is a type this block cannot 
 * contain then an exception is thrown. 
 *
 * @param index The index where the new block is inserted. 
 *
 * @param child Pointer to the new block that is inserted as this block's child. 
 */
void AbstractBlock::insert(int index, std::unique_ptr<AbstractBlock>&& child)
{
   // Make sure the given pointer is not null. 
   if ( !child )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }

   // Make sure the given block's type can be a child of this block. 
   if ( !buildList().contains(child->type()) )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.")
                   .arg(child->type())
                   .arg(type()));
      throw e;
   }

   // Insert the given block as a child of this block at the given index and notify 
   // of modification. 
   AbstractBlock* adopted {child.release()};
   adopted->setParent(this,index);
   notifyModified();

   // Starting with this block iterate up the tree of parents, calling their child 
   // added interface, until the root block is reached or the child added interface 
   // returns false. 
   AbstractBlock* notify {this};
   while ( notify && notify->childAdded(adopted) ) notify = notify->parent();
}






/*!
 * Takes the child block at the given index this block contains. The child block 
 * returned no longer has a parent. If the index is out of range then an exception 
 * is thrown. 
 *
 * @param index The index of the child that is taken. 
 *
 * @return Pointer to child that was taken from this block. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::take(int index)
{
   // Make sure the given index is within range. 
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot take child %1 when only %2 children exist.")
                   .arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // Remove the child from this block's child list at the given index, saving the 
   // pointer and notifying of modification. 
   unique_ptr<AbstractBlock> ret {_children.at(index)};
   ret->setParent(nullptr);
   notifyModified();

   // Starting with this block iterate up the tree of parents, calling their child 
   // removed interface, until the root block is reached or the child removed 
   // interface returns false. 
   AbstractBlock* notify {this};
   while ( notify && notify->childRemoved(ret.get()) ) notify = notify->parent();

   // Return the removed child pointer. 
   return ret;
}






/*!
 * Removes the child block at the given index, deleting it and any children it may 
 * contain. If the given index is out of range then an exception is thrown. 
 *
 * @param index The index of the child that is removed and deleted. 
 */
void AbstractBlock::remove(int index)
{
   // Take this block's child from the given index and delete it. 
   take(index).reset();
}






/*!
 * Reads in block data from an XML element. This clears any existing data this 
 * block contains, including the removal and deletion of all existing children. 
 * This recursively adds any children contained in the XML reading in their data 
 * from child XML elements. 
 *
 * @param element The XML element that stores block data this block reads as input. 
 */
void AbstractBlock::read(const QDomElement& element)
{
   // Delete and clear any existing children this block may contain. 
   qDeleteAll(_children);
   _children.clear();

   // Iterate through all children nodes of the given element. 
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check if the node is an element. 
      if ( node.isElement() )
      {
         // If the tag name is the data tag then read in this block's data else read in the 
         // element as a new child for this block. 
         QDomElement element {node.toElement()};
         if ( element.tagName() == _dataTag )
         {
            readData(element);
         }
         else readChild(element);
      }

      // Move to the next node sibling. 
      node = node.nextSibling();
   }

   // Make sure all children of this block is allowed to this block's child list. 
   for (auto child: qAsConst(_children))
   {
      if ( !buildList().contains(child->type()) )
      {
         Exception::ReadError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Loaded illegal child block type '%1' to block type '%2'")
                      .arg(factory().name(child->type()))
                      .arg(factory().name(type())));
         throw e;
      }
   }
}






/*!
 * This interface is called whenever a child below this block has modified its 
 * name. This keeps getting called on the next block parent until this returns 
 * false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has modified its name. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childNameModified(AbstractBlock* child)
{
   // Return false. 
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever a new child below this block has been added. 
 * This keeps getting called on the next block parent until this returns false. The 
 * default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childAdded(AbstractBlock* child)
{
   // Return false. 
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever an existing child below this block has been 
 * removed. This keeps getting called on the next block parent until this returns 
 * false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has been removed from its former 
 *              parent block. This object can be deleted right after this call. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childRemoved(AbstractBlock* child)
{
   // Return false. 
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever an existing child below this block has been 
 * moved. This keeps getting called on the next block parent until this returns 
 * false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has been moved in its parent's list 
 *              of children. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childMoved(AbstractBlock* child)
{
   // Return false. 
   Q_UNUSED(child)
   return false;
}






/*!
 * Returns the data version of the data this block is reading in. If this block is 
 * not reading in data then this is the current version for this block's data. 
 *
 * @return Data version of the data this block is reading in or the current version 
 *         if data is not being read in. 
 */
int AbstractBlock::dataVersion() const
{
   // Return this object's internal version state, which can be either the version 
   // being read in or the current version. 
   return _version;
}






/*!
 * Notifies this block has been modified by finding its root block and emitting its 
 * modified signal. 
 */
void AbstractBlock::notifyModified()
{
   // Find the root block of this block and emit its modified signal. 
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
   emit root->modified();
}






/*!
 * Notifies this block's name has been modified by finding its root block and 
 * emitting its name modified signal. If this is called by a root block an 
 * exception is thrown because the root has no name. 
 */
void AbstractBlock::notifyNameModified()
{
   // Get a pointer to this block's parent block. 
   AbstractBlock* root {parent()};

   // Make sure the parent block pointer is not null. 
   if ( !root )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot notify a name change of the root block."));
      throw e;
   }

   // Starting with this block's parent iterate up the tree of parents, calling their 
   // child name modified interface, until the root block is reached or the child 
   // name modified interface returns false. 
   AbstractBlock* notify {root};
   while ( notify && notify->childNameModified(this) ) notify = notify->parent();

   // Find the root block of this block and emit its name modified signal. 
   while ( root->parent() ) root = root->parent();
   emit root->nameModified(this);
}






/*!
 * Notifies this block's body has been modified by finding its root block and 
 * emitting its name modified signal. If this is called by a root block an 
 * exception is thrown because the root has no body. 
 */
void AbstractBlock::notifyBodyModified()
{
   // Get a pointer to this block's parent block. 
   AbstractBlock* root {parent()};

   // Make sure the parent block pointer is not null. 
   if ( !root )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot notify a body change of the root block."));
      throw e;
   }

   // Find the root block of this block and emit its body modified signal. 
   while ( root->parent() ) root = root->parent();
   emit root->bodyModified(this);
}






/*!
 * Reads in this block's field data from the given XML element. This overwrites any 
 * values this block's fields may already contain. 
 *
 * @param element The XML element that is read in as this block's data. 
 */
void AbstractBlock::readData(const QDomElement& element)
{
   // Set this block's version number to the version attribute from the given XML 
   // element and the read in state to true. 
   _version = element.attribute(_versionTag,"0").toInt();
   _readIn = true;

   // Create a mapping used for all string list fields. 
   QMap<int,QStringList> lists;

   // Iterate through all fields of this block. 
   for (int i = 0; i < fieldSize() ;++i)
   {
      // Determine the field type and set its value to the default state. 
      switch (fieldType(i))
      {
      case Boolean:
         setField(i,false);
         break;
      case String:
         setField(i,QString());
         break;
      case StringList:
         setField(i,QStringList());

         // For the string list type also add an empty string list to the mapping. 
         lists.insert(i,QStringList());
         break;
      }
   }

   // Iterate through all children nodes of the given element. 
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check if the node is an element. 
      if ( node.isElement() )
      {
         // Try to find the field index with the given tag name of the element. 
         QDomElement element {node.toElement()};
         int i {fieldIndexOf(element.tagName())};
         if ( i >= 0 )
         {
            // Set the value of the field with the found index based off its type. 
            switch (fieldType(i))
            {
            case Boolean:
               quietlySetField(i,true);
               break;
            case String:
               quietlySetField(i,element.text());
               break;
            case StringList:

               // If this is a string list append the element's text as a new string to the 
               // string list's temporary mapping. 
               lists[i] << element.text();
               break;
            }
         }
      }

      // Move to the next sibling node. 
      node = node.nextSibling();
   }

   // Iterate through the string list fields mapping and set their respective fields. 
   for ( auto i = lists.begin(); i != lists.end() ;++i)
   {
      quietlySetField(i.key(),*i);
   }

   // Set this block's data version to the current version and the read in state to 
   // false. 
   _version = version();
   _readIn = false;
}






/*!
 * Writes out this block's field data to the returned XML element, using the given 
 * XML document to create the returned element. 
 *
 * @param document XML document used for creating the returned data element. 
 *
 * @return XML element containing all this block's field data. 
 */
QDomElement AbstractBlock::writeData(QDomDocument& document) const
{
   // Create the return element and set its version attribute. 
   QDomElement ret {document.createElement(_dataTag)};
   ret.setAttribute(_versionTag,QString::number(version()));

   // Iterate through all fields for this block. 
   for (int i = 0; i < fieldSize() ;++i)
   {
      // Determine the field type and append the necessary elements to the return 
      // element to save the field's value. 
      switch (fieldType(i))
      {
      case Boolean:
         if ( field(i).toBool() ) ret.appendChild(document.createElement(fieldTag(i)));
         break;
      case String:
         {
            // If this string field's value is not empty then append an element containing its 
            // value to the return element. 
            QString value {field(i).toString()};
            if ( !value.isEmpty() )
            {
               QDomElement element {document.createElement(fieldTag(i))};
               element.appendChild(document.createTextNode(value));
               ret.appendChild(element);
            }
            break;
         }
      case StringList:
         {
            // Append elements for each string the list contains, each element containing the 
            // value of each string. 
            for (auto string: field(i).toStringList())
            {
               QDomElement element {document.createElement(fieldTag(i))};
               element.appendChild(document.createTextNode(string));
               ret.appendChild(element);
            }
            break;
         }
      }
   }

   // Return the element containing all field data for this block. 
   return ret;
}






/*!
 * Copies all children from the given block and appends the copies to the list of 
 * this block's children. This does not remove any previous children already part 
 * of this block. 
 *
 * @param parent Pointer to block whose children are copied. 
 */
void AbstractBlock::copyChildren(const AbstractBlock* parent)
{
   // Iterate through all the children of the given block. 
   for (auto child : qAsConst(parent->_children))
   {
      // Make a copy of the child and append it to this block. 
      child->makeCopy().release()->setParent(this,size());
   }
}






/*!
 * Copies all field data from the given block to this one, overwriting all field 
 * data this block currently contains. This assumes the given block is the same 
 * type as this one. 
 *
 * @param other Pointer to the block whose field data is copied to this block. 
 */
void AbstractBlock::copyDataFrom(const AbstractBlock* other)
{
   // Iterate through all fields for this block. 
   for (int i = 0; i < fieldSize() ;++i)
   {
      // Set the value of this block's field to the value of the other block's same 
      // field. 
      quietlySetField(i,other->field(i));
   }
}






/*!
 * Creates a new child reading it in from the given XML element. The new child is 
 * then appended to this block's list of children. 
 *
 * @param element XML element used to read in the new child. 
 */
void AbstractBlock::readChild(const QDomElement& element)
{
   // Read in the type attribute from the given child element. 
   bool ok;
   int type {element.attribute(_typeTag,"nan").toInt(&ok)};

   // Make sure reading in the type did not fail. 
   if ( !ok )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Type attribute is not an integer."));
      throw e;
   }

   // Make sure the read in type is within range of this block's factory. 
   if ( type < 0 || type >= factory().size() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(factory().size()));
      throw e;
   }

   // Create a new block with the read in type and append it to this block's child 
   // list. 
   unique_ptr<AbstractBlock> child {factory().makeBlock(type,false)};
   AbstractBlock* back {child.get()};
   child.release()->setParent(this,size());

   // Have the new block read in its data and children from the given element. 
   back->read(element);
}






/*!
 * Sets this block's parent to the given block, adding it to its new parent's list 
 * with the given index. If the index is less than 1 it is prepended, else if it is 
 * greater than the size of the list it is appended. This will remove any previous 
 * parent this block may have had. The new parent can be a null pointer which means 
 * this block will have no parent. 
 *
 * @param parent Pointer to the new parent for this block. 
 *
 * @param index The index where this block will be added to its new parent's list. 
 */
void AbstractBlock::setParent(AbstractBlock* parent, int index)
{
   // If this block already has a parent then remove it from the parent's child list. 
   if ( AbstractBlock* oldParent = AbstractBlock::parent() )
   {
      oldParent->_children.removeOne(this);
   }

   // Set this block's parent to the new parent, if any. 
   QObject::setParent(parent);

   // If the new parent is not null then insert this block into the new parent's 
   // child list at the given index. 
   if ( parent )
   {
      parent->_children.insert(index,this);
   }
}
