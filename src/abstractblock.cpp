#include <QDomDocument>
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "exception.h"
#include "domelementreader.h"



using namespace std;



const char* AbstractBlock::_dataTag {"data"};
const char* AbstractBlock::_typeTag {"type"};






AbstractBlock*AbstractBlock::parent() const
{
   return _parent;
}






int AbstractBlock::childrenSize() const
{
   return _children.size();
}






AbstractBlock* AbstractBlock::child(int index)
{
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get child %1 when only %2 children exist.").arg(index).arg(_children.size()));
      throw e;
   }
   return _children.at(index);
}





const AbstractBlock* AbstractBlock::child(int index) const
{
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get child %1 when only %2 children exist.").arg(index).arg(_children.size()));
      throw e;
   }
   return _children.at(index);
}






int AbstractBlock::childIndex(AbstractBlock* child) const
{
   return _children.indexOf(child);
}






void AbstractBlock::insertChild(int index, unique_ptr<AbstractBlock>&& child)
{
   if ( !child )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }
   if ( !factory().buildList(type()).contains(child->type()) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.").arg(child->type()).arg(type()));
      throw e;
   }
   child.release()->setBlockParent(this,index);
   emit modified();
}






unique_ptr<AbstractBlock> AbstractBlock::takeChild(int index)
{
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot take child %1 when only %2 children exist.").arg(index).arg(_children.size()));
      throw e;
   }
   AbstractBlock* ret = _children.at(index);
   ret->setBlockParent(nullptr,-1);
   emit modified();
   return unique_ptr<AbstractBlock>(ret);
}






void AbstractBlock::removeChild(int index)
{
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot remove child %1 when only %2 children exist.").arg(index).arg(_children.size()));
      throw e;
   }
   delete _children.takeAt(index);
   emit modified();
}






void AbstractBlock::read(const QDomElement& parent)
{
   qDeleteAll(_children);
   _children.clear();
   QDomElement data;
   QList<QDomElement> children;
   DomElementReader reader(parent);
   reader.set(_dataTag,&data);
   const QList<int> buildList {factory().buildList(type())};
   for (auto type : buildList)
   {
      reader.set(factory().elementName(type),&children,false);
   }
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
   readData(data);
   for (auto child : qAsConst(children))
   {
      readChild(child);
   }
}






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
      child_.setAttribute(_typeTag,QString::number(child->type()));
      ret.appendChild(child_);
   }
   return ret;
}






AbstractBlock* AbstractBlock::root()
{
   AbstractBlock* root {this};
   while ( root->_parent )
   {
      root = root->_parent;
   }
   return root;
}






void AbstractBlock::copyChildren(const AbstractBlock* block)
{
   for (auto child : qAsConst(block->_children))
   {
      child->makeCopy().release()->setBlockParent(this,childrenSize());
   }
}






QList<AbstractBlock*> AbstractBlock::children() const
{
   return _children;
}






void AbstractBlock::notifyOfNameChange()
{
   notifyOfNameChange(nullptr);
   emit nameChanged(this);
}






void AbstractBlock::childModified()
{
   emit modified();
}






void AbstractBlock::setBlockParent(AbstractBlock* parent, int index)
{
   if ( _parent )
   {
      _parent->_children.removeOne(this);
      setParent(nullptr);
      disconnect(_parent);
   }
   if ( parent )
   {
      parent->_children.insert(index,this);
      setParent(parent);
      connect(this,&AbstractBlock::modified,parent,&AbstractBlock::childModified);
   }
   _parent = parent;
   setParent(parent);
}






void AbstractBlock::readChild(const QDomElement& child)
{
   DomElementReader reader(child);
   int type {reader.attributeToInt(_typeTag)};
   if ( type < 0 || type >= factory().size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(factory().size()));
      throw e;
   }
   unique_ptr<AbstractBlock> child_ {factory().makeBlock(type)};
   child_->read(child);
   child_.release()->setBlockParent(this,childrenSize());
}






void AbstractBlock::notifyOfNameChange(AbstractBlock* block)
{
   if ( !block )
   {
      root()->notifyOfNameChange(this);
   }
   else
   {
      emit nameChanged(block);
   }
}
