#include <memory>

#include "abstractblockfactory.h"
#include "abstractblock.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace std;






//@@
void AbstractBlock::initialize(int type, AbstractBlock* parent)
{
   // set type
   _type = type;

   // if parent is not nullptr set it
   if ( parent )
   {
      setBlockParent(parent,parent->getChildrenSize());
   }
}






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
      Type
      ,Data
      ,Children
   };

   // initialize xml element parser
   XMLElementParser parser(xml);
   parser.addKeyword("type").addKeyword("data").addKeyword("children");
   int element;

   // parse xml until end of nested element is reached
   while ( ( element = parser() ) != -1 )
   {
      // determine which element has been found
      switch (element)
      {
      case Type:
         {
            // read in type integer
            bool ok;
            int type {xml.readElementText().toInt(&ok)};

            // make sure reading worked
            if ( !ok )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Failed reading in type integer."));
               throw e;
            }

            // make sure the type is correct
            if ( _type != type )
            {
               Exception::ReadError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Read in type %1 for object of type %2.").arg(type).arg(_type));
               throw e;
            }
            break;
         }
      case Data:
         // read in data
         readData(xml);
         break;
      case Children:
         // read in children
         readChildren(xml);
         break;
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
   // write type and data
   xml.writeTextElement("type",QString::number(_type));
   xml.writeStartElement("data");
   writeData(xml);
   xml.writeEndElement();

   // write out list of all children
   xml.writeStartElement("children");
   for (auto i = _children.constBegin(); i != _children.constEnd() ;++i)
   {
      // write start of child element and its type
      xml.writeStartElement("child");
      xml.writeTextElement("type",QString::number((*i)->_type));

      // write out egress element
      xml.writeStartElement("egress");
      (*i)->write(xml);

      // write out end of elements
      xml.writeEndElement();
      xml.writeEndElement();
   }

   // write out end of children list
   xml.writeEndElement();
}






//@@
void AbstractBlock::notifyOfNameChange()
{
   // iterate backwards until root is found
   AbstractBlock* object {this};
   while ( object->_parent )
   {
      object = object->_parent;
   }

   // emit name changed signal on root object
   QMetaObject::invokeMethod(object,"nameChanged",Qt::DirectConnection,Q_ARG(AbstractBlock*,this));
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






void AbstractBlock::readChildren(QXmlStreamReader& xml)
{
   // initialize xml parser
   XMLElementParser parser(xml);
   parser.addKeyword("child");
   int element;

   // read in any and all child elements found
   while ( ( element = parser() ) != -1 )
   {
      readChild(xml);
   }
}






void AbstractBlock::readChild(QXmlStreamReader& xml)
{
   // enumeration for different elements
   enum
   {
      Type
      ,Egress
   };

   // initialize new child pointer and xml parser
   unique_ptr<AbstractBlock> child;
   XMLElementParser parser(xml);
   parser.addKeyword("type").addKeyword("egress");
   int element;

   // parse xml until end of nested element is reached
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      // determine which element is found
      switch (element)
      {
      case Type:
         {
            // make sure a new child has not already been created
            if ( !child )
            {
               // read in type integer
               bool ok;
               int type {xml.readElementText().toInt(&ok)};

               // make sure reading worked
               if ( !ok )
               {
                  Exception::ReadError e;
                  MARK_EXCEPTION(e);
                  e.setDetails(tr("Failed reading in type integer."));
                  throw e;
               }

               // make sure type integer is valid
               if ( type < 0 || type >= _factory.getSize() )
               {
                  Exception::ReadError e;
                  MARK_EXCEPTION(e);
                  e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type)
                               .arg(_factory.getSize()));
                  throw e;
               }

               // make new child block and initialize
               child.reset(_factory.makeBlock(type));
               child->initialize(type,this);
            }
            break;
         }
      case Egress:
         // make sure type has been read in first
         if ( !child )
         {
            Exception::ReadError e;
            MARK_EXCEPTION(e);
            e.setTitle(tr("Read in egress element before type element."));
            throw e;
         }

         // have child block read in its xml
         child->read(xml);
         break;
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
