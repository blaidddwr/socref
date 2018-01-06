#include <memory>

#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






unique_ptr<AbstractBlock> Namespace::makeCopy() const
{
   unique_ptr<Namespace> ret {new Namespace};
   ret->_name = _name;
   ret->_description = _description;
   ret->copyChildren(this);
   return ret;
}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






const AbstractBlockFactory&Namespace::factory() const
{
   return BlockFactory::instance();
}






Namespace& Namespace::setName(const QString& name)
{
   if ( !QRegExp("[a-zA-Z_]*[a-zA-Z0-9_]*").exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid namespace '%1'.").arg(name));
      throw e;
   }
   if ( _name != name )
   {
      _name = name;
      notifyOfNameChange();
      emit modified();
   }
   return *this;
}






Namespace& CppQt::Namespace::setDescription(const QString& description)
{
   if ( _description != description )
   {
      _description = description;
      emit modified();
   }
   return *this;
}






AbstractBlock* Namespace::readData(QXmlStreamReader& xml)
{
   enum
   {
      Name = 0
      ,Description
   };
   XMLElementParser parser(xml);
   parser.addKeyword("name",true).addKeyword("description",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
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
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
   return this;
}






const AbstractBlock* Namespace::writeData(QXmlStreamWriter& xml) const
{
   xml.writeTextElement("name",_name);
   xml.writeTextElement("description",_description);
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
   return this;
}
