#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "cppqt_definition.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace std;
using namespace CppQt;






AbstractBlock* Definition::readData(QXmlStreamReader& xml)
{
   enum
   {
      Name = 0
   };
   XMLElementParser parser(xml);
   parser.addKeyword("name",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Name:
         setName(xml.readElementText());
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






const AbstractBlock* Definition::writeData(QXmlStreamWriter& xml) const
{
   xml.writeTextElement("name",_type->name());
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
   return this;
}






unique_ptr<AbstractBlock> Definition::makeCopy() const
{
   unique_ptr<Definition> ret {new Definition(factory(),AbstractBlock::type())};
   ret->_type = _type->makeCopy();
   return ret;
}






Definition& Definition::setName(const QString& name)
{
   if ( !QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*").exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type name '%1'.").arg(name));
      throw e;
   }
   if ( Type::Concrete* type = dynamic_cast<Type::Concrete*>(_type.get()) )
   {
      type->setName(name);
   }
   return *this;
}
