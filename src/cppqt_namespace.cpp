#include "cppqt_namespace.h"
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
void Namespace::setName(const QString &name)
{
   // if new name is different update and notify of change
   if ( _name != name )
   {
      _name = name;
      notifyOfNameChange();
   }
}
