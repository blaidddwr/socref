#include "abstractblock.h"
#include <QDomDocument>
#include <exception.h>
#include "abstractblockfactory.h"
#include "domelementreader.h"



using namespace std;
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
 * This interface makes an exact copy of this block. A default implementation of 
 * this interface is given that uses other virtual functions to create a copy of 
 * this block. The new block returned has no parent. 
 *
 * @return Exact copy of this block's data as a new block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new empty block that is the same type as this block. 
 *
 * 2. Copy all of this block's children and its data. 
 *
 * 3. Return a pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::makeCopy() const
{
   unique_ptr<AbstractBlock> ret {makeBlank()};
   ret->copyChildren(this);
   ret->copyDataFrom(this);
   return ret;
}






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Pointer to the root block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through the parents of this block until the root block with no parent 
 *    is reached. 
 *
 * 2. Return the root node pointer. 
 */
AbstractBlock* AbstractBlock::root()
{
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
   return root;
}






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Read only pointer to the root block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through the parents of this block until the root block with no parent 
 *    is reached. 
 *
 * 2. Return the root node pointer. 
 */
const AbstractBlock* AbstractBlock::root() const
{
   const AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
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
   return qobject_cast<AbstractBlock*>(QObject::parent());
}






/*!
 * This returns the number of children this block contains. 
 *
 * @return Number of children this block contains. 
 */
int AbstractBlock::size() const
{
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
   return _children;
}






/*!
 * This gets the index where the child with the given pointer is stored in this 
 * node's list of children. If no child with the given pointer is found then -1 is 
 * returned. 
 *
 * @param pointer Pointer of child block to match in this node's list and return 
 *                its index. 
 *
 * @return If a match is found then the index of the child with the given pointer, 
 *         else -1. 
 */
int AbstractBlock::indexOf(AbstractBlock* pointer) const
{
   return _children.indexOf(pointer);
}






/*!
 * This returns a pointer to this node's child with the given index. If the index 
 * is out of range an exception is thrown. 
 *
 * @param index  
 *
 * @return Pointer to this node's child with given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception, else go to next 
 *    step. 
 *
 * 2. Return a pointer to this node's child with the given index. 
 */
AbstractBlock* AbstractBlock::get(int index) const
{
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Cannot get child %1 when only %2 children exist.")
               .arg(index)
               .arg(_children.size()));
      throw e;
   }
   return _children.at(index);
}






/*!
 * Tests if this node contains any children of the given type. 
 *
 * @param type The block type to search for within this block's list of children. 
 *
 * @return True if this node contains any children of the given type, else false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this node's children. 
 *
 * 2. If a child that matches the given type is found then return true, else if the 
 *    end of the list is reached with no match then return false. 
 */
bool AbstractBlock::containsType(int type) const
{
   for (auto child : qAsConst(_children))
   {
      if ( child->type() == type ) return true;
   }
   return false;
}






/*!
 * Tests if this node contains any children of any type given. 
 *
 * @param types List of block types to search for within this block's list of 
 *              children. 
 *
 * @return True if this node contains any children of any type given, else false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this node's children. 
 *
 * 2. If a child that matches the given type is found then return true, else if the 
 *    end of the list is reached with no match then return false. 
 */
bool AbstractBlock::containsType(const QList<int>& types) const
{
   for (auto child : qAsConst(_children))
   {
      if ( types.contains(child->type()) ) return true;
   }
   return false;
}






/*!
 * Moves the child block with the given index up the list by one where the very top 
 * of the list is index 0. If the given index is already at the top of the list or 
 * out of range then this does nothing. 
 *
 * @param index The index of the child block moved up by one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is already at the top of the list or out of range then do 
 *    nothing and exit, else go to the next step. 
 *
 * 2. Swap the given child in this block's list with the child just above it, 
 *    calling the interface function notifying a child has moved and emitting the 
 *    modified signal. 
 */
void AbstractBlock::moveUp(int index)
{
   if ( index > 0 && index < _children.size() )
   {
      std::swap(_children[index - 1],_children[index]);
      childMoved(_children.at(index - 1));
      emit modified();
   }
}






/*!
 * Moves the child block with the given index down the list by one where the very 
 * top of the list is index 0. If the given index is already at the bottom of the 
 * list or out of range then this does nothing. 
 *
 * @param index The index of the child block moved down by one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is at the end of the list or out of range then do nothing 
 *    and exit, else go to the next step. 
 *
 * 2. Swap the child with the given index in this block's list with the child just 
 *    below it, calling the interface function notifying a child has moved and 
 *    emitting the modified signal. 
 */
void AbstractBlock::moveDown(int index)
{
   if ( index >= 0 && index < (_children.size() - 1) )
   {
      std::swap(_children[index],_children[index + 1]);
      childMoved(_children.at(index + 1));
      emit modified();
   }
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given child pointer is null or its type is not contained in this 
 *    blocks build list then throw an exception, else go to the next step. 
 *
 * 2. Insert the new block into this block's list of children at the given index, 
 *    releasing it from its smart pointer and setting this block as its parent. 
 *
 * 3. Call the interface notifying a new child is added and emit the modified 
 *    signal. 
 */
void AbstractBlock::insert(int index, std::unique_ptr<AbstractBlock>&& child)
{
   if ( !child )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }
   if ( !buildList().contains(child->type()) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Cannot insert child block of type %1 to parent block of type %2.")
               .arg(child->type())
               .arg(type()));
      throw e;
   }
   AbstractBlock* child_ {child.release()};
   child_->setParent(this,index);
   childAdded(child_);
   emit modified();
}






/*!
 * Takes the child block at the given index this block contains. The child block 
 * returned no longer has a parent. If the index is out of range then an exception 
 * is thrown. 
 *
 * @param index The index of the child that is taken. 
 *
 * @return Pointer to child that was taken from this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception, else go to the 
 *    next step. 
 *
 * 2. Remove the child at the given index from this block's list of children saving 
 *    its pointer to a smart pointer. 
 *
 * 3. Set the child's parent to nothing, call the interface that notifies a child 
 *    is being removed, emit the modified signal, and return the smart pointer. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::take(int index)
{
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Cannot take child %1 when only %2 children exist.")
               .arg(index)
               .arg(_children.size()));
      throw e;
   }
   unique_ptr<AbstractBlock> ret {_children.at(index)};
   ret->setParent(nullptr);
   childRemoved(ret.get());
   emit modified();
   return ret;
}






/*!
 * Removes the child block at the given index, deleting it and any children it may 
 * contain. If the given index is out of range then an exception is thrown. 
 *
 * @param index The index of the child that is removed and deleted. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception, else go to the 
 *    next step. 
 *
 * 2. Take the pointer to the child at the given index from this block's list of 
 *    children, call the interface that notified a child is being removed, and emit 
 *    the modified signal. 
 *
 * 3. Delete the pointer by using a smart pointer when it goes out of scope. 
 */
void AbstractBlock::remove(int index)
{
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Cannot remove child %1 when only %2 children exist.")
               .arg(index)
               .arg(_children.size()));
      throw e;
   }
   unique_ptr<AbstractBlock> dead {_children.takeAt(index)};
   childRemoved(dead.get());
   emit modified();
}






/*!
 * Reads in block data from an XML element. This clears any existing data this 
 * block contains, including the removal and deletion of all existing children. 
 * This recursively adds any children contained in the XML reading in their data 
 * from child XML elements. 
 *
 * @param element The XML element that stores block data this block reads as input. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Clear and delete any existing children this block may contain. Then read in 
 *    this blocks data element. 
 *
 * 2. If the data element could not be found in the XML element then throw an 
 *    exception, else go to the next step. 
 *
 * 3. Call the interface which reads in the data for this block's type. 
 *
 * 4. Read in all children XML elements and add them to this block's list of 
 *    children, creating a new child for each one and calling their read function. 
 */
void AbstractBlock::read(const QDomElement& element)
{
   qDeleteAll(_children);
   _children.clear();
   QDomElement data;
   QList<QDomElement> children;
   DomElementReader reader(element);
   reader.set(_dataTag,&data);
   reader.read();
   readData(data,data.attribute(_versionTag).toInt());
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
   for (auto type : buildList()) reader.set(factory().elementName(type),&children,false);
   reader.read();
   for (auto child : qAsConst(children)) readChild(child);
}






/*!
 * Writes out this blocks data as an XML element. This includes all children as 
 * child XML elements which have their write functions called recursively. 
 *
 * @param document  
 *
 * @return XML element that stores this block's data and all children underneath 
 *         it. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new XML element and add the data element to it derived by calling 
 *    the interface for writing the data of this block's type as an XML element. 
 *
 * 2. Call each child's write function and add their returned elements to the new 
 *    XML element as children. 
 *
 * 3. Return the newly created XML element containing the data and all children. 
 */
QDomElement AbstractBlock::write(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   QDomElement data {writeData(document)};
   data.setTagName(_dataTag);
   data.setAttribute(_versionTag,QString::number(writeVersion()));
   ret.appendChild(data);
   for (auto child : _children)
   {
      QDomElement child_ {child->write(document)};
      child_.setTagName(factory().elementName(child->type()));
      child_.setAttribute(_typeTag,child->type());
      ret.appendChild(child_);
   }
   return ret;
}






/*!
 * This interface is called whenever a child below this block has changed its name. 
 *
 * @param child Pointer of the child whose name has changed. 
 */
void AbstractBlock::childNameChanged(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 * This interface is called when a new block has been added to this block as a new 
 * child. 
 *
 * @param child Pointer of the new child just added to this block. 
 */
void AbstractBlock::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 * This interface is called when a direct child of this block is removed. The 
 * removed child could possibly be deleted right after this call. 
 *
 * @param child Pointer of child that has been removed from this block. 
 */
void AbstractBlock::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 * This interface is called when a direct child of this block has been moved up or 
 * down. 
 *
 * @param child Pointer of child that has been moved up or down. 
 */
void AbstractBlock::childMoved(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 * Makes a new XML element with the given tag name and text value enclosed within 
 * the new element. 
 *
 * @param document The XML document used to create the new XML element. 
 *
 * @param tagName The tag name for the new XML element. 
 *
 * @param text The text enclosed within the new XML element. 
 *
 * @return New XML element with the given tag name and text. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new XML element with the given tag name. 
 *
 * 2. Append a new text node with the given text to the new XML element as a child 
 *    and then return the new XML element. 
 */
QDomElement AbstractBlock::makeElement(QDomDocument& document, const QString& tagName, const QString& text)
{
   QDomElement ret {document.createElement(tagName)};
   ret.appendChild(document.createTextNode(text));
   return ret;
}






/*!
 * Notifies this block that its name has changed and the block system is notified. 
 */
void AbstractBlock::notifyOfNameChange()
{
   emit nameChanged(this);
}






/*!
 * Called when a child below this block has been modified. 
 */
void AbstractBlock::childModified()
{
   emit modified();
}






/*!
 * Called when a child below this block has modified its name. 
 *
 * @param child Pointer to child block whose name has been modified. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call the interface that notifies a child's name has changed and emit the name 
 *    changed signal. 
 */
void AbstractBlock::childNameModified(AbstractBlock* child)
{
   childNameChanged(child);
   emit nameChanged(child);
}






/*!
 * Copies all children from the given block and appends the copies to the list of 
 * this block's children. This does not remove any previous children already part 
 * of this block. 
 *
 * @param parent Pointer to block whose children are copied. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all the children of the given block. 
 *
 * 2. Make a copy of each child using the interface which does so, appending them 
 *    to this block's list of children. 
 */
void AbstractBlock::copyChildren(const AbstractBlock* parent)
{
   for (auto child : qAsConst(parent->_children))
   {
      child->makeCopy().release()->setParent(this,size());
   }
}






/*!
 * Sets this block's parent to the given block, adding it to its new parent's list 
 * with the given index. If the index is less than 1 it is prepended, else if it is 
 * greater than the size of the list it is appended. This will remove and 
 * disconnect any previous parent this block may have had. The new parent can be a 
 * null pointer which means this block will have no parent. 
 *
 * @param parent Pointer to the new parent for this block. 
 *
 * @param index The index where this block will be added to its new parent's list. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If a previous parent exists then remove this block from its list of children 
 *    and disconnect any signal/slot connections they share. 
 *
 * 2. Set this block's parent to the new parent given. 
 *
 * 3. If the new parent is not null then insert this block into the new parent's 
 *    list with the given index and connect the appropriate signals between the two 
 *    of them. 
 */
void AbstractBlock::setParent(AbstractBlock* parent, int index)
{
   if ( AbstractBlock* oldParent = AbstractBlock::parent() )
   {
      oldParent->_children.removeOne(this);
      disconnect(oldParent);
   }
   QObject::setParent(parent);
   if ( parent )
   {
      parent->_children.insert(index,this);
      connect(this,&AbstractBlock::modified,parent,&AbstractBlock::childModified);
      connect(this,&AbstractBlock::nameChanged,parent,&AbstractBlock::childNameModified);
   }
}






/*!
 * Creates a new child reading it in from the given XML element. The new child is 
 * then appended to this block's list of children. 
 *
 * @param element XML element used to read in the new child. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the type number for this block is out of range of possible types for this 
 *    project type then throw an exception, else go to the next step. 
 *
 * 2. Create a new block with the given type and read it in from the XML element, 
 *    appending it to this blocks list of children. 
 */
void AbstractBlock::readChild(const QDomElement& element)
{
   DomElementReader reader(element);
   int type {reader.attributeToInt(_typeTag)};
   if ( type < 0 || type >= factory().size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(factory().size()));
      throw e;
   }
   unique_ptr<AbstractBlock> child {factory().makeBlock(type)};
   child->read(element);
   child.release()->setParent(this,size());
}
