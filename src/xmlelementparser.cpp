#include "xmlelementparser.h"
#include "exception.h"






XMLElementParser& XMLElementParser::addKeyword(const QString& keyword, bool onlyOnce, bool markRead)
{
   _keywords.push_back(keyword);
   _keywordInfos.push_back({markRead,onlyOnce});
   return *this;
}






int XMLElementParser::operator()()
{
   while ( _level > _stopLevel && !_xml.atEnd() )
   {
      _xml.readNext();
      if ( _xml.isStartElement() )
      {
         int index;
         if ( _level == 1 && ( index = _keywords.indexOf(_xml.name().toString()) ) != -1 )
         {
            KeywordInfo& info {_keywordInfos[index]};
            if ( !info.once || ( info.once && !info.read ) )
            {
               info.read = true;
               return index;
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
      e.setDetails(QObject::tr("XML error encountered: %1").arg(_xml.errorString()));
      throw e;
   }
   return End;
}






bool XMLElementParser::allRead() const
{
   bool ret {true};
   for (const auto& i : _keywordInfos)
   {
      ret &= i.read;
   }
   return ret;
}
