#ifndef SCANNER_H
#define SCANNER_H
#include <QObject>
#include <socutil/soc_ut_qptr.h>
#include "abstract.h"
//



/*!
 * This is a scanner that is responsible for parsing an entire source file using a 
 * list of parser elements added to it. The parser elements must be added before 
 * parsing a source file. A scanner can parse as many times as needed but it should 
 * only be used once on the specific file it is meant to be parsing. 
 */
class Scanner : public QObject
{
   Q_OBJECT
public:
   void parse(const QString& path);
   void addParser(Soc::Ut::QPtr<Abstract::Parser>&& parser);
private:
   void parseInput();
   QString createOutput();
   Abstract::Parser* findParser(const QString& line);
   /*!
    * List of parser elements this scanner uses for parsing any source file it is 
    * given to parse. 
    */
   QList<Abstract::Parser*> _parsers;
   /*!
    * List of parser elements that require reading input lines this scanner uses for 
    * parsing any source file it is given to parse. 
    */
   QList<Abstract::Parser*> _inputParsers;
   /*!
    * Contains the entire contents of the source file this scanner is parsing before 
    * it is overwritten. This is used to process any input lines required by parser 
    * elements that need input. 
    */
   QString _input;
};



#endif
