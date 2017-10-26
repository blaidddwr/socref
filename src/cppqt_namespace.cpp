#include "cppqt_namespace.h"
#include "xmlelementparser.h"
#include "exception.h"



using namespace CppQt;






//@@
void Namespace::readData(QXmlStreamReader& xml)
{
   // initialize xml parser
   XMLElementParser parser(xml);
   parser.addKeyword("name",true);

   // run parser once and see if it finds name element
   if ( parser() != XMLElementParser::End )
   {
      _name = xml.readElementText();
   }

   // else name element was not found
   else
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
   // write out name and make sure it worked
   xml.writeTextElement("name",_name);
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
