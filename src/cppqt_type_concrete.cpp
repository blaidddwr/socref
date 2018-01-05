#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "cppqt_type_concrete.h"
#include "exception.h"
#include "xmlelementparser.h"
#include "cppqt_typefactory.h"



using namespace CppQt;
using namespace CppQt::Type;
using namespace std;






bool Concrete::isEquivalent(const AbstractType* type) const
{
   if ( const Concrete* type_ = dynamic_cast<const Concrete*>(type) )
   {
      return name() == type_->name();
   }
   return false;
}






AbstractType* Concrete::read(QXmlStreamReader& xml)
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
      e.setDetails(QObject::tr("Failed reading in all required elements."));
      throw e;
   }
   return this;
}






int Concrete::type() const
{
   return TypeFactory::ConcreteType;
}






void Concrete::writeData(QXmlStreamWriter& xml) const
{
   xml.writeTextElement("name",name());
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Xml Error writing to file."));
      throw e;
   }
}
