#ifndef XMLELEMENTPARSER_H
#define XMLELEMENTPARSER_H
#include <QXmlStreamReader>



//@@
class XMLElementParser
{
public:
   //@@
   XMLElementParser(QXmlStreamReader& xml, int level = 1, int stopLevel = 0):
      _xml(xml),
      _level(level),
      _stopLevel(stopLevel)
   {}
   XMLElementParser& addKeyword(const QString& keyword);
   int operator()();
private:
   QXmlStreamReader& _xml;
   int _level;
   int _stopLevel;
   QList<QString> _keywords;
};



#endif
