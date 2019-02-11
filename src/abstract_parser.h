#ifndef ABSTRACT_PARSER_H
#define ABSTRACT_PARSER_H
#include <QObject>
//



namespace Abstract
{
   /*!
    * This represents parser elements that are used in a scanner to parse a source 
    * file. A parser element can read the input of the source file based off a header 
    * that must be matched. A parser element also provides output lines that are used 
    * to write output to the scanner source file along with all other parser elements 
    * part of a scanner. 
    * 
    * A scanner queries all of its parser elements that take input for a regular 
    * expression header that is matched with each input line read from the source 
    * file. If the regular expression matches the line then each following line, 
    * including the header line matched, is provided to the element parser until it is 
    * finished. 
    */
   class Parser : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * This interface returns the output lines for this parser element used by its 
       * parent scanner to write output to its source file. 
       *
       * @return Output lines for this parser element. 
       */
      virtual QStringList output() const = 0;
      virtual bool hasInput() const;
      virtual QString headerExpression() const;
      virtual bool input(const QString& line);
      virtual void reset();
   };
}



#endif
