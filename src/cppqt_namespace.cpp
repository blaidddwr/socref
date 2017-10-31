#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"



using namespace CppQt;






//@@
void Namespace::readData(QXmlStreamReader& xml)
{
   // enumeration of elements to read
   enum
   {
      Name = 0
      ,Description
   };

   // initialize xml parser
   XMLElementParser parser(xml);
   parser.addKeyword("name",true).addKeyword("description",true);
   int element;

   // run parser until it reaches the end
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      // determine which element was found and set accordingly
      switch (element)
      {
      case Name:
         _name = xml.readElementText();
         break;
      case Description:
         _description = xml.readElementText();
         break;
      }
   }

   // make sure name was read
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
}






//@@
void Namespace::writeData(QXmlStreamWriter& xml) const
{
   // write out name and description
   xml.writeTextElement("name",_name);
   xml.writeTextElement("description",_description);

   // make sure it worked
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
}






//@@
AbstractBlock* Namespace::makeCopy() const
{
   // make copy of namespace
   Namespace* ret {new Namespace(getFactory(),getType())};
   ret->_name = _name;
   ret->_description = _description;

   // copy all children of namespace and return pointer
   ret->copyChildren(this);
   return ret;
}






//@@
void Namespace::setName(const QString &name)
{
   // check if new name is different
   if ( _name != name )
   {
      // set new name, notify of name change, and emit modified signal
      _name = name;
      notifyOfNameChange();
      emit modified();
   }
}






//@@
void CppQt::Namespace::setDescription(const QString& description)
{
   // check if new description is different
   if ( _description != description )
   {
      // set new description and emit modified signal
      _description = description;
      emit modified();
   }
}






//@@
QList<QString> Namespace::getVariableTypes() const
{
   // prepare return list
   QList<QString> ret;

   // if this is the root namespace add default types
   if ( !getParent() )
   {
      ret << "bool" << "char" << "short int" << "int" << "long int" << "long long int"
          << "short unsigned int" << "unsigned int" << "long unsigned int"
          << "long long unsigned int" << "float" << "double" << "qint8" << "qint16" << "qint32"
          << "qint64" << "quint8" << "quint16" << "quint32" << "quint64";
   }

   // iterate through all children
   for (int i = 0; i < getChildrenSize() ;++i)
   {
      // check if child is a namespace
      if ( getChild(i)->getType() == BlockFactory::NamespaceType )
      {
         // get list of variable types for child
         const Namespace* child {qobject_cast<const Namespace*>(getChild(i))};
         QList<QString> childList {child->getVariableTypes()};

         // append name of child to its variable types and add to main list
         for (auto x = childList.begin(); x != childList.end() ;++i)
         {
            (*x).prepend(child->getName() + "::");
         }
         ret << childList;
      }
   }

   // return main list of variable types
   return ret;
}
