#include "abstractblock.h"
#include <QDomDocument>
#include <exception.h>
#include "abstractblockfactory.h"
#include "domelementreader.h"



using namespace std;
//



/*!
 */
const char* AbstractBlock::_dataTag {"data"};
const char* AbstractBlock::_versionTag {"version"};
/*!
 */
const char* AbstractBlock::_typeTag {"type"};






/*!
 * This must make an exact copy of this block. A default implementation of this 
 * block is given that uses other virtual functions to create a copy of this 
 * block. The new block returned has no parent. 
 *
 * @return Exact copy of this block's data as a new block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Make a null instance of this block type as the return variable. 
 *
 * 2. Copy the children of this block to the new block instance return variable. 
 *
 * 3. Copy all data from this block to the new block instance return variable. 
 *
 * 4. Return the new block return variable. 
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
 * 1. Initialize the return pointer with the pointer to this block. 
 *
 * 2. Iterate the return pointer, setting it to its parent, until the return 
 *    pointer does not have a parent. 
 *
 * 3. Return the return pointer. 
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
 * 1. Initialize the return pointer with the pointer to this block. 
 *
 * 2. Iterate the return pointer, setting it to its parent, until the return 
 *    pointer does not have a parent. 
 *
 * 3. Return the return pointer. 
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
 * @return Pointer to parent block, if any. 
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
 * This returns a read only reference to this block's list of children blocks. 
 * The list is a list of pointers to all child blocks. 
 *
 * @return Reference to this block's list of children. 
 */
const QList<AbstractBlock*>& AbstractBlock::list() const
{
   return _children;
}






/*!
 * This gets the index where the child with the given pointer it stored in this 
 * node's list of children, if any. If no child with the given pointer is found 
 * then -1 is returned. 
 *
 * @param pointer Pointer of child block to match in this node's list and return 
 *                its index. 
 *
 * @return Index of child with given pointer, if any. 
 */
int AbstractBlock::indexOf(AbstractBlock* pointer) const
{
   return _children.indexOf(pointer);
}






/*!
 * This returns a pointer to this node's child with the given index. If the 
 * index is out of range an exception is thrown. 
 *
 * @param index  
 *
 * @return Pointer to this node's child with given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception, else go to 
 *    next step. 
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
 * @param type The block type to search for within this block's list of 
 *             children. 
 *
 * @return Returns true if this node contains any children of the given type, 
 *         else returns false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this node's children. If a child that matches the 
 *    given type is found then return true. 
 *
 * 2. No match found after reaching end of list so return false. 
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
 * @return Returns true if this node contains any children of any type given, 
 *         else returns false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this node's children. If a child that matches any 
 *    of the given types is found then return true. 
 *
 * 2. No match found after reaching end of list so return false. 
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
 *
 * @param index  
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
 *
 * @param index  
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
 *
 * @param index  
 *
 * @param child  
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
 *
 * @param index  
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
 *
 * @param index  
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
 *
 * @param element  
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
 *
 * @param document  
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
 *
 * @param child  
 */
void AbstractBlock::childNameChanged(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 *
 * @param child  
 */
void AbstractBlock::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 *
 * @param child  
 */
void AbstractBlock::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 *
 * @param child  
 */
void AbstractBlock::childMoved(AbstractBlock* child)
{
   Q_UNUSED(child)
}






/*!
 */
void AbstractBlock::notifyOfNameChange()
{
   notifyOfNameChange(nullptr);
}






/*!
 */
void AbstractBlock::childModified()
{
   emit modified();
}






/*!
 *
 * @param parent  
 */
void AbstractBlock::copyChildren(const AbstractBlock* parent)
{
   for (auto child : qAsConst(parent->_children))
   {
      child->makeCopy().release()->setParent(this,size());
   }
}






/*!
 *
 * @param parent  
 *
 * @param index  
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
   }
}






/*!
 *
 * @param element  
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






/*!
 *
 * @param changed  
 */
void AbstractBlock::notifyOfNameChange(AbstractBlock* changed)
{
   if ( !changed )
   {
      AbstractBlock* root {this};
      while ( root->parent() )
      {
         root = root->parent();
         root->childNameChanged(this);
      }
      root->notifyOfNameChange(this);
   }
   else emit nameChanged(changed);
}
