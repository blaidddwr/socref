#include "xmlelementparser.h"
#include "exception.h"






//@@
XMLElementParser& XMLElementParser::addKeyword(const QString& keyword, bool onlyOnce, bool markRead)
{
   // push new keyword to list and return reference to object
   _keywords.push_back(keyword);
   _keywordInfos.push_back({markRead,onlyOnce});
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
         // check to see if this is first level and name matches a keyword
         int index;
         if ( _level == 1 && ( index = _keywords.indexOf(_xml.name().toString()) ) != -1 )
         {
            // read keyword if not marked as read only once or has not been read once yet
            KeywordInfo& info {_keywordInfos[index]};
            if ( !info.once || ( info.once && !info.read ) )
            {
               // mark keyword as read and return its index
               info.read = true;
               return index;
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
   for (const auto& i : _keywordInfos)
   {
      // if keyword read value is false make return value false
      ret &= i.read;
   }

   // return if all keywords read
   return ret;
}
