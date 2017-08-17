#include "xmlelementparser.h"
#include "exception.h"






// @@ XMLElementParser::addKeyword(const QString&)
XMLElementParser& XMLElementParser::addKeyword(const QString& keyword)
{
   _keywords.push_back(keyword);
   return *this;
}






// @@ XMLElementParser::operator()()
int XMLElementParser::operator()()
{
   while ( _level > _stopLevel && !_xml.atEnd() )
   {
      _xml.readNext();
      if ( _xml.isStartElement() )
      {
         if ( _level == 1 )
         {
            for (int i = 0; i < _keywords.size() ;++i)
            {
               if ( _xml.name() == _keywords.at(i) )
               {
                  return i;
               }
            }
         }
         ++_level;
      }
      else if ( _xml.isEndElement() )
      {
         --_level;
      }
   }
   if ( _xml.hasError() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setTitle(QObject::tr("XML Read Error"));
      e.setDetails(QObject::tr("XML error encountered: %1").arg(_xml.errorString()));
      throw e;
   }
   return -1;
}
