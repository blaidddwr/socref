#ifndef BASIC_INPUTPARSER_H
#define BASIC_INPUTPARSER_H
#include "abstract_parser.h"
//



namespace Basic
{
   /*!
    * This implements the abstract parser class. This is a basic input parser that 
    * grabs input from its parent scanner's source file based off a begin begin 
    * expression and end expression. Each line in the source file that matches the 
    * begin regular expression is added to this parser element's output lines and 
    * stops once it encounters a line that matches the end regular expression. The 
    * begin and end lines matched are not added as output lines. 
    */
   class InputParser : public Abstract::Parser
   {
      Q_OBJECT
   public:
      virtual QStringList output() const override final;
      virtual bool needsInput() const override final;
      virtual QString headerExpression() const override final;
      virtual bool input(const QString& line) override final;
      virtual void reset() override final;
   public:
      explicit InputParser(const QString& beginExp, const QString& endExp, bool first = false);
   private:
      /*!
       * The regular expression used to match the beginning line that causes this parser 
       * to start taking input from its parent scanner's source file. 
       */
      const QString _beginExp;
      /*!
       * The regular expression used to match the ending line that causes this parser to 
       * end taking input from its parent scanner's source file. 
       */
      const QString _endExp;
      /*!
       * The output lines of this parser grabbed from input between matching beginning 
       * and ending lines. 
       */
      QStringList _lines;
      /*!
       * True if the first line input has not been given by this parent's scanner. False 
       * if the first line has been read or this line parser is set to read the first 
       * line of input. 
       */
      bool _first {true};
   };
}



#endif
