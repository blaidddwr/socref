#include "abstract_block.h"
#include <socutil/soc_ut_qptr.h>
#include "abstract_blockfactory.h"



using namespace Abstract;
//



/*!
 * The name for version attributes. 
 */
const char* Block::_versionTag {"version"};
/*!
 * The tag name for data elements. 
 */
const char* Block::_dataTag {"data"};
/*!
 * The name for type attributes. 
 */
const char* Block::_typeTag {"type"};






/*!
 * This interface makes an exact copy of this block. A default implementation of 
 * this interface is given that uses other virtual functions to create a copy of 
 * this block. The new block returned has no parent. 
 *
 * @return Exact copy of this block's data as a new block. 
 */
Soc::Ut::QPtr<Abstract::Block> Block::makeCopy() const
{
   // Create a new blank block identical to this block's type. 
   Soc::Ut::QPtr<Block> ret {makeBlank()};

   // Make sure the new block is the same type. 
   Q_ASSERT(type() == ret->type());

   // Copy this block's children and implementation data to the new block. 
   ret->copyChildren(this);
   ret->copyDataFrom(this);

   // Return the new block. 
   return ret;
}






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Read only pointer to the root block. 
 */
const Abstract::Block* Block::root() const
{
   // Iterate up the tree of parents until the root pointer is found. 
   const Block* root {this};
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
Abstract::Block* Block::parent() const
{
   // Return the qt object of this object cast as an abstract block. 
   return qobject_cast<Block*>(QObject::parent());
}






/*!
 * This returns the number of children this block contains. 
 *
 * @return Number of children this block contains. 
 */
int Block::size() const
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
const QList<Abstract::Block*>& Block::list() const
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
int Block::indexOf(Block* pointer) const
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
Abstract::Block* Block::get(int index) const
{
   // Make sure the given index is within range. 
   Q_ASSERT(index >= 0);
   Q_ASSERT(index < _children.size());

   // Return a pointer to this block's child with the given index. 
   return _children.at(index);
}






/*!
 * Tests if this block contains any children of the given type. 
 *
 * @param type The block type to search for within this block's list of children. 
 *
 * @return True if this block contains any children of the given type, else false. 
 */
bool Block::containsType(int type) const
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
bool Block::containsType(const QList<int>& types) const
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
QDomElement Block::write(QDomDocument& document) const
{
   // Create the data element for this bock's data. 
   QDomElement data {writeData(document)};
   data.setTagName(_dataTag);

   // Create a new element that will contain this block's data and all children. 
   QDomElement ret {document.createElement(factory().elementName(type()))};

   // Add the data element for the return element. 
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
Abstract::Block* Block::root()
{
   // Iterate up the tree of parents until the root pointer is found. 
   Block* root {this};
   while ( root->parent() ) root = root->parent();

   // Return the root pointer. 
   return root;
}






/*!
 * Moves the child block with the given index up the list by one where the very top 
 * of the list is index 0. If the given index is already at the top of the list or 
 * out of range then this does nothing. 
 *
 * @param index The index of the child block moved up by one. 
 */
void Block::moveUp(int index)
{
   Q_ASSERT(index > 0);
   Q_ASSERT(index < _children.size());
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
   Block* notify {this};
   while ( notify && notify->childMoved(_children.at(index - 1)) ) notify = notify->parent();
}






/*!
 * Moves the child block with the given index down the list by one where the very 
 * top of the list is index 0. If the given index is already at the bottom of the 
 * list or out of range then this does nothing. 
 *
 * @param index The index of the child block moved down by one. 
 */
void Block::moveDown(int index)
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
   Block* notify {this};
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
void Block::insert(int index, Soc::Ut::QPtr<Block>&& child)
{
   // Make sure the given pointer is not null. 
   Q_CHECK_PTR(child.get());
   Q_ASSERT(buildList().contains(child->type()));

   // Make sure the given block's type can be a child of this block. 
   Block* adopted {child.release(this)};
   _children.insert(index,adopted);
   notifyModified();

   // Insert the given block as a child of this block at the given index and notify 
   // of modification. 
   Block* notify {this};
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
Soc::Ut::QPtr<Abstract::Block> Block::take(int index)
{
   // Make sure the given index is within range. 
   Q_ASSERT(index >= 0);
   Q_ASSERT(index < _children.size());

   // Remove the child from this block's child list at the given index, saving the 
   // pointer and notifying of modification. 
   Soc::Ut::QPtr<Block> ret {_children.at(index)};
   _children.removeAll(ret.get());
   notifyModified();

   // Starting with this block iterate up the tree of parents, calling their child 
   // removed interface, until the root block is reached or the child removed 
   // interface returns false. 
   Block* notify {this};
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
void Block::remove(int index)
{
   // Take this block's child from the given index and delete it. 
   take(index).reset(nullptr);
}






/*!
 * Reads in block data from an XML element. This clears any existing data this 
 * block contains, including the removal and deletion of all existing children. 
 * This recursively adds any children contained in the XML reading in their data 
 * from child XML elements. 
 *
 * @param element The XML element that stores block data this block reads as input. 
 */
void Block::read(const QDomElement& element)
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
      Q_ASSERT(buildList().contains(child->type()));
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
bool Block::childNameModified(Block* child)
{
   // Return false. 
   Q_UNUSED(child);
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
bool Block::childAdded(Block* child)
{
   // Return false. 
   Q_UNUSED(child);
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
bool Block::childRemoved(Block* child)
{
   // Return false. 
   Q_UNUSED(child);
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
bool Block::childMoved(Block* child)
{
   // Return false. 
   Q_UNUSED(child);
   return false;
}






/*!
 * Notifies this block has been modified by finding its root block and emitting its 
 * modified signal. 
 */
void Block::notifyModified()
{
   // Find the root block of this block and emit its modified signal. 
   Block* root {this};
   while ( root->parent() ) root = root->parent();
   emit root->modified();
}






/*!
 * Notifies this block's name has been modified by finding its root block and 
 * emitting its name modified signal. If this is called by a root block an 
 * exception is thrown because the root has no name. 
 */
void Block::notifyNameModified()
{
   // Get a pointer to this block's parent block. 
   Block* root {parent()};

   // Make sure the parent block pointer is not null. 
   Q_CHECK_PTR(root);

   // Starting with this block's parent iterate up the tree of parents, calling their 
   // child name modified interface, until the root block is reached or the child 
   // name modified interface returns false. 
   Block* notify {root};
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
void Block::notifyBodyModified()
{
   // Get a pointer to this block's parent block. 
   Block* root {parent()};

   // Make sure the parent block pointer is not null. 
   Q_CHECK_PTR(root);

   // Find the root block of this block and emit its body modified signal. 
   while ( root->parent() ) root = root->parent();
   emit root->bodyModified(this);
}






/*!
 * Copies all children from the given block and appends the copies to the list of 
 * this block's children. This does not remove any previous children already part 
 * of this block. 
 *
 * @param parent Pointer to block whose children are copied. 
 */
void Block::copyChildren(const Block* parent)
{
   // Iterate through all the children of the given block. 
   for (auto child : qAsConst(parent->_children)) _children << child->makeCopy().release(this);
}






/*!
 * Creates a new child reading it in from the given XML element. The new child is 
 * then appended to this block's list of children. 
 *
 * @param element XML element used to read in the new child. 
 */
void Block::readChild(const QDomElement& element)
{
   // Read in the type attribute from the given child element. 
   int type {factory().typeByElementName(element.tagName())};

   // Make sure reading in the type did not fail. 
   Q_ASSERT(type >= 0);

   // Make sure the read in type is within range of this block's factory. 
   Q_ASSERT(type < factory().size());

   // Create a new block with the read in type and append it to this block's child 
   // list. 
   Soc::Ut::QPtr<Block> child {factory().makeBlock(type,false)};
   Block* back {child.release(this)};
   _children << back;

   // Have the new block read in its data and children from the given element. 
   back->read(element);
}
