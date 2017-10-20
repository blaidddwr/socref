#include "abstractblock.h"
#include "exception.h"






//@@
void AbstractBlock::initialize(int type, AbstractBlock* parent)
{
   // set type
   _type = type;

   // if parent is not nullptr set it
   if ( parent )
   {
      setParent(parent);
   }
}






//@@
void AbstractBlock::insertChild(int index, AbstractBlock* child)
{
   // make sure the child pointer is valid
   if ( !child )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }

   // set new child's parent and emit modified signal
   child->setParent(this,index);
   emit modified();
}






//@@
AbstractBlock* AbstractBlock::takeChild(int index)
{
}






//@@
void AbstractBlock::removeChild(int index)
{
}






//@@
void AbstractBlock::read(QXmlStreamReader &xml)
{
}






//@@
void AbstractBlock::write(QXmlStreamWriter& xml) const
{
}






//@@
void AbstractBlock::setParent(AbstractBlock* parent, int index)
{
   // if current parent exists remove from children and disconnect signals from it
   if ( QObject::parent() )
   {
      AbstractBlock* parent = qobject_cast<AbstractBlock*>(QObject::parent());
      parent->_children.removeOne(this);
      disconnect(parent);
   }

   // check if new parent exists
   if ( parent )
   {
      // insert into list of children and connect modified signal
      parent->_children.insert(index,this);
      connect(this,&AbstractBlock::modified,parent,&AbstractBlock::childModified);
   }

   // set new parent
   QObject::setParent(parent);
}
