#include <QDomDocument>

#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace std;






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







AbstractBlock* AbstractBlock::insertChild(int index, unique_ptr<AbstractBlock> child)
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
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.").arg(child->type()).arg(type()));
      throw e;
   }
   child.release()->setBlockParent(this,index);
   emit modified();
   return this;
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






AbstractBlock* AbstractBlock::removeChild(int index)
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
   return this;
}






AbstractBlock* AbstractBlock::read(const QDomElement& parent)
{
   enum
   {
      Data = 0
      ,Total
   };
   QStringList tags {"data"};
   QVector<bool> readTags(1,false);
   qDeleteAll(_children);
   _children.clear();
   const QList<int> buildList {factory().buildList(type())};
   QStringList buildNames;
   for (const auto& i : buildList)
   {
      buildNames.push_back(factory().elementName(i));
   }
   QDomNode node {parent.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         if ( element.tagName() == tags.at(Data) )
         {
            readData(element);
            readTags[Data] = true;
         }
         else if ( buildNames.contains(element.tagName()) )
         {
            readChild(element);
         }
      }
      node = node.nextSibling();
   }
   if ( readTags.contains(false) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
   return this;
}






QDomElement AbstractBlock::write(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   QDomElement data {writeData(document)};
   data.setTagName("data");
   ret.appendChild(data);
   for (auto child : _children)
   {
      QDomElement child_ {child->write(document)};
      child_.setTagName(factory().elementName(child->type()));
      child_.setAttribute("type",QString::number(child->type()));
      ret.appendChild(child_);
   }
   return ret;
}






AbstractBlock& AbstractBlock::root()
{
   AbstractBlock* root {this};
   while ( root->_parent )
   {
      root = root->_parent;
   }
   return *root;
}






AbstractBlock* AbstractBlock::copyChildren(const AbstractBlock* block)
{
   for (const auto& child : block->_children)
   {
      child->makeCopy().release()->setBlockParent(this,childrenSize());
   }
   return this;
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
   if ( !child.hasAttribute("type") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Child element missing type attribute."));
      throw e;
   }
   bool ok;
   int type {child.attribute("type").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in type attribute."));
      throw e;
   }
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
      root().notifyOfNameChange(this);
   }
   else
   {
      emit nameChanged(block);
   }
}
