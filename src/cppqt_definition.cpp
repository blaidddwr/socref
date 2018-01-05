#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "cppqt_definition.h"
#include "exception.h"
#include "xmlelementparser.h"
#include "cppqt_typefactory.h"



using namespace std;
using namespace CppQt;






unique_ptr<AbstractBlock> Definition::makeCopy() const
{
   unique_ptr<Definition> ret {new Definition(factory(),AbstractBlock::type())};
   ret->_type = _type->makeCopy();
   return ret;
}






Definition& Definition::setType(unique_ptr<AbstractType>&& type)
{
   if ( *type != *_type )
   {
      _type = std::move(type);
      notifyOfNameChange();
      emit modified();
   }
   return *this;
}






AbstractBlock* Definition::readData(QXmlStreamReader& xml)
{
   enum
   {
      Type = 0
   };
   XMLElementParser parser(xml);
   parser.addKeyword("type",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Type:
         _type = TypeFactory::instance().read(xml);
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
   _type->write("type",xml);
   return this;
}
