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
   XMLElementParser& addKeyword(const QString& keyword, bool onlyOnce = false);
   int operator()();
   bool allRead() const;
private:
   struct Keyword
   {
      QString text;
      bool read;
      bool once;
   };
   QXmlStreamReader& _xml;
   int _level;
   int _stopLevel;
   QList<Keyword> _keywords;
};



//@@
inline XMLElementParser::XMLElementParser(QXmlStreamReader &xml, int level, int stopLevel):
   _xml(xml), _level(level), _stopLevel(stopLevel) {}



#endif
