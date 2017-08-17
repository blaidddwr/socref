#ifndef XMLELEMENTPARSER_H
#define XMLELEMENTPARSER_H
#include <QXmlStreamReader>



// @@ XMLElementParser
class XMLElementParser
{
public:
   XMLElementParser(QXmlStreamReader& xml, int level = 1, int stopLevel = 0);
   XMLElementParser& addKeyword(const QString& keyword);
   int operator()();
private:
   QXmlStreamReader& _xml;
   int _level;
   int _stopLevel;
   QList<QString> _keywords;
};






// @@ XMLElementParser::XMLElementParser(QXmlStreamReader&,int,int)
inline XMLElementParser::XMLElementParser(QXmlStreamReader& xml, int level, int stopLevel):
   _xml(xml),
   _level(level),
   _stopLevel(stopLevel)
{}



#endif
