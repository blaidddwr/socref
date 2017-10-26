#include "xmlelementparser.h"
#include "exception.h"






//@@
XMLElementParser& XMLElementParser::addKeyword(const QString& keyword, bool onlyOnce)
{
   // push new keyword to list and return reference to object
   _keywords.push_back({keyword,false,onlyOnce});
   return *this;
}






//@@
int XMLElementParser::operator()()
{
   // do loop while level is greater than stop level and xml is not at end
   while ( _level > _stopLevel && !_xml.atEnd() )
   {
      // read next element and check to see if it is start element
      _xml.readNext();
      if ( _xml.isStartElement() )
      {
         // check to see if this is first level
         if ( _level == 1 )
         {
            // loop through all keywords
            for (int i = 0; i < _keywords.size() ;++i)
            {
               // check if the element name matches the keyword name
               if ( _xml.name() == _keywords.at(i).text )
               {
                  // read keyword if not marked as read only once or has not been read once yet
                  if ( !_keywords.at(i).once || ( _keywords.at(i).once && !_keywords.at(i).read ) )
                  {
                     // mark keyword as read and return its index
                     _keywords[i].read = true;
                     return i;
                  }
               }
            }
         }

         // increment level
         ++_level;
      }

      // else check if it is end element
      else if ( _xml.isEndElement() )
      {
         // decrement level
         --_level;
      }
   }

   // make sure no xml error has occured
   if ( _xml.hasError() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("XML error encountered: %1").arg(_xml.errorString()));
      throw e;
   }

   // return end of xml element/document indicator
   return End;
}






//@@
bool XMLElementParser::allRead() const
{
   // initialize return value to true and iterate through all keywords
   bool ret {true};
   for (auto i = _keywords.constBegin(); i != _keywords.constEnd() ;++i)
   {
      // if keyword read value is false make return value false
      ret &= (*i).read;
   }

   // return if all keywords read
   return ret;
}
