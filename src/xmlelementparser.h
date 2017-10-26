#ifndef XMLELEMENTPARSER_H
#define XMLELEMENTPARSER_H
#include <QXmlStreamReader>



//@@
class XMLElementParser
{
public:
   enum
   {
      End = -1
   };
   XMLElementParser(QXmlStreamReader& xml, int level = 1, int stopLevel = 0);
   XMLElementParser& addKeyword(const QString& keyword, bool onlyOnce = false
         , bool markRead = false);
   int operator()();
   bool allRead() const;
   bool keyRead(const QString& keyword);
private:
   struct KeywordInfo
   {
      bool read;
      bool once;
   };
   QXmlStreamReader& _xml;
   int _level;
   int _stopLevel;
   QList<QString> _keywords;
   QList<KeywordInfo> _keywordInfos;
};



//@@
inline XMLElementParser::XMLElementParser(QXmlStreamReader &xml, int level, int stopLevel):
   _xml(xml), _level(level), _stopLevel(stopLevel) {}



#endif
