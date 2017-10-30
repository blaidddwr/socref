#include <memory>

#include "abstractblockfactory.h"
#include "abstractblock.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace std;






//@@
AbstractBlock* AbstractBlock::getChild(int index) const
{
   // make sure index given is valid
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get child %1 when only %2 children exist.").arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // return child document pointer
   return _children.at(index);
}






//@@
void AbstractBlock::insertChild(int index, AbstractBlock* o_child)
{
   // make sure the child pointer is valid
   if ( !o_child )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }

   // make sure block type can be added as child
   if ( !_factory.getBuildList(_type).contains(o_child->_type) )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.")
                   .arg(o_child->_type).arg(_type));
      throw e;
   }

   // set new child's parent and emit modified signal
   o_child->setBlockParent(this,index);
   emit modified();
}






//@@
AbstractBlock* AbstractBlock::takeChild(int index)
{
   // make sure index given is valid
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot take child %1 when only %2 children exist.").arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // clear orphaned child's parent
   AbstractBlock* ret = _children.at(index);
   ret->setBlockParent(nullptr,-1);

   // emit modified signal and return orphaned child pointer
   emit modified();
   return ret;
}






//@@
void AbstractBlock::removeChild(int index)
{
   // make sure index given is valid
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot remove child %1 when only %2 children exist.").arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // remove child from list and emit modified signal
   delete _children.takeAt(index);
   emit modified();
}






//@@
void AbstractBlock::read(QXmlStreamReader &xml)
{
   // enumeration for different elements
   enum
   {
      Data
      ,Child
   };

   // initialize xml element parser
   XMLElementParser parser(xml);
   parser.addKeyword("data",true);

   // add keywords for all block types this block can have as children
   const QList<int> buildList {_factory.getBuildList(_type)};
   for (auto i = buildList.constBegin(); i != buildList.constEnd() ;++i)
   {
      parser.addKeyword(_factory.getElementName(*i),false,true);
   }

   // parse xml until end of nested element is reached
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      // if element is data read it in
      if ( element == Data )
      {
         readData(xml);
      }

      // else element is child so read that in
      else
      {
         readChild(xml);
      }
   }

   // make sure all required elements were read in
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
}






//@@
void AbstractBlock::write(QXmlStreamWriter& xml) const
{
   // write data
   xml.writeStartElement("data");
   writeData(xml);
   xml.writeEndElement();

   // write out list of all children
   for (auto i = _children.constBegin(); i != _children.constEnd() ;++i)
   {
      // write start of child element and its type attribute
      xml.writeStartElement(_factory.getElementName((*i)->_type));
      xml.writeAttribute("type",QString::number((*i)->_type));

      // write out child and end element
      (*i)->write(xml);
      xml.writeEndElement();
   }

   // make sure all writing worked
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
}






//@@
void AbstractBlock::setBlockParent(AbstractBlock* parent, int index)
{
   // if current parent exists remove from children and disconnect signals from it
   if ( _parent )
   {
      _parent->_children.removeOne(this);
      disconnect(_parent);
   }

   // if new parent exists insert into list of children and connect modified signal
   if ( parent )
   {
      parent->_children.insert(index,this);
      connect(this,&AbstractBlock::modified,parent,&AbstractBlock::childModified);
   }

   // set new parent
   _parent = parent;
   setParent(parent);
}






//@@
void AbstractBlock::readChild(QXmlStreamReader& xml)
{
   // make sure type attribute exists
   if ( !xml.attributes().hasAttribute("type") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Child element missing type attribute."));
      throw e;
   }

   // read in type attribute and make sure it worked
   bool ok;
   int type {xml.attributes().value("type").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in type attribute."));
      throw e;
   }

   // make sure type is valid
   if ( type < 0 || type >= _factory.getSize() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(_factory.getSize()));
      throw e;
   }

   // create new block, read in data, and add it as child
   unique_ptr<AbstractBlock> child {_factory.makeBlock(type)};
   child->read(xml);
   child.release()->setBlockParent(this,getChildrenSize());
}






//@@
void AbstractBlock::notifyOfNameChange(AbstractBlock* block)
{
   // check if block pointer is null
   if ( !block )
   {
      // iterate up chain of parents until root is reached
      AbstractBlock* object {this};
      while ( object->_parent )
      {
         object = object->_parent;
      }

      // call this function on root block
      object->notifyOfNameChange(this);
   }

   // else this is root block so emit name changed signal
   else
   {
      emit nameChanged(block);
   }
}
