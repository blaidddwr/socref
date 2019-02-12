#ifndef BASIC_LINEPARSER_H
#define BASIC_LINEPARSER_H
#include "abstract_parser.h"
//



namespace Basic
{
   /*!
    * This implements the abstract parser class. This is a very simple parser that 
    * requires no input and simply provides the lines of text given to it along with 
    * an indent of spaces prepended to each line provided to its parent scanner. 
    */
   class LineParser : public Abstract::Parser
   {
      Q_OBJECT
   public:
      virtual QStringList output() const override final;
   public:
      /*!
       * Constructs a new line parser. 
       */
      LineParser() = default;
      LineParser(int count);
      LineParser(int indent, const QString& line);
      void setIndent(int indent);
      void add(const QString& line);
      void add(const QStringList& lines);
      void add(int count);
   private:
      /*!
       * The lines used to generate the static output of this line parser to its parent 
       * scanner when parsing a source file. 
       */
      QStringList _lines;
      /*!
       * The number of space characters prepended to each line of output when this parser 
       * generates it for its parent scanner. 
       */
      int _indent {0};
   };
}



#endif
