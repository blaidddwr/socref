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
/*!
 */
const char* AbstractBlock::_typeTag {"type"};






/*!
 */
std::unique_ptr<AbstractBlock> AbstractBlock::makeCopy() const
{
   unique_ptr<AbstractBlock> ret {makeBlank()};
   ret->copyChildren(this);
   ret->copyDataFrom(this);
   return ret;
}






/*!
 */
AbstractBlock* AbstractBlock::root()
{
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
   return root;
}






/*!
 */
const AbstractBlock* AbstractBlock::root() const
{
   const AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
   return root;
}






/*!
 */
AbstractBlock* AbstractBlock::parent() const
{
   return qobject_cast<AbstractBlock*>(QObject::parent());
}






/*!
 */
int AbstractBlock::size() const
{
   return _children.size();
}






/*!
 */
const QList<AbstractBlock*>& AbstractBlock::list() const
{
   return _children;
}






/*!
 *
 * @param pointer  
 */
int AbstractBlock::indexOf(AbstractBlock* pointer) const
{
   return _children.indexOf(pointer);
}






/*!
 *
 * @param index  
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
 *
 * @param type  
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
 *
 * @param types  
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
   readData(data);
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
