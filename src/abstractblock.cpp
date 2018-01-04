#include "abstractblock.h"
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
   if ( !_factory.buildList(_type).contains(child->_type) )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.").arg(child->_type).arg(_type));
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






AbstractBlock* AbstractBlock::read(QXmlStreamReader &xml)
{
   enum
   {
      Data
      ,Child
   };
   XMLElementParser parser(xml);
   parser.addKeyword("data",true);
   const QList<int> buildList {_factory.buildList(_type)};
   for (const auto& i : buildList)
   {
      parser.addKeyword(_factory.elementName(i),false,true);
   }
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      if ( element == Data )
      {
         readData(xml);
      }
      else
      {
         readChild(xml);
      }
   }
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
   return this;
}






const AbstractBlock* AbstractBlock::write(QXmlStreamWriter& xml) const
{
   xml.writeStartElement("data");
   writeData(xml);
   xml.writeEndElement();
   for (const auto& i : _children)
   {
      xml.writeStartElement(_factory.elementName(i->_type));
      xml.writeAttribute("type",QString::number(i->_type));
      i->write(xml);
      xml.writeEndElement();
   }
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
   return this;
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






void AbstractBlock::readChild(QXmlStreamReader& xml)
{
   if ( !xml.attributes().hasAttribute("type") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Child element missing type attribute."));
      throw e;
   }
   bool ok;
   int type {xml.attributes().value("type").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in type attribute."));
      throw e;
   }
   if ( type < 0 || type >= _factory.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(_factory.size()));
      throw e;
   }
   unique_ptr<AbstractBlock> child {_factory.makeBlock(type)};
   child->read(xml);
   child.release()->setBlockParent(this,childrenSize());
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
