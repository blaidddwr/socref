#ifndef BASIC_FUNCTIONPARSER_H
#define BASIC_FUNCTIONPARSER_H
#include "abstract_parser.h"
#include <QRegularExpression>
//



namespace Basic
{
   /*!
    * This implements the abstract parser class. This provides lines representing a 
    * single function with this parser element's parent scanner's source file. This 
    * will grab input from the source file if the matching function is found, taking 
    * all lines of that function. These input lines are then parsed, cutting off any 
    * additional indent spacing and adding inline comments as needed. Steps of 
    * operation are provided for filling in the inline comments. 
    * 
    * The function is matched in the source file based off a header regular expression 
    * that is built with provided methods of this class. Once matched lines will be 
    * read as input until the last out token is found signaling the end of the 
    * function. The first header line defining the function and last line with the 
    * final out token will be included in input. 
    * 
    * Additional parsing is done with the input lines by looking for marker lines of 
    * inline comments. The provided comment begin string is matched with the beginning 
    * of the string, excluding any preceding white space. Each consecutive lines of 
    * inline comment matches will be treated as a single inline comment and filled in 
    * with the specific operation in the provided steps of operation. The order of 
    * inline comments will be matched with the order provided in the steps of 
    * operation. 
    */
   class FunctionParser : public Abstract::Parser
   {
      Q_OBJECT
   public:
      virtual QStringList output() const override final;
      virtual bool needsInput() const override final;
      virtual QString headerExpression() const override final;
      virtual void setIndent(int value) override final;
      virtual bool input(const QString& line) override final;
      virtual void reset() override final;
   public:
      explicit FunctionParser(QChar inToken, QChar outToken, const QString& commentBegin, int maxColumns, const QString& header, const QStringList& operations);
      void add(const QString& text);
      void addExp(const QString& expression);
   private:
      void processInlineComment(const QString& line);
      void insertInlineComment(int index, int spacing);
      /*!
       * The character used to denote the beginning of a block of execution. This is used 
       * to determine the end of a function with this parser element's parent scanner's 
       * source file. 
       */
      const QChar _inToken;
      /*!
       * The character used to denote the end of a block of execution. This is used to 
       * determine the end of a function with this parser element's parent scanner's 
       * source file. 
       */
      const QChar _outToken;
      /*!
       * The string used at the beginning of an input line, excluding any preceding white 
       * space, to denote that input line is an inline comment. 
       */
      const QString _commentBegin;
      /*!
       * The maximum number of columns allowed for a single line of an inline comment. 
       */
      const int _maxColumns;
      /*!
       * The header or full name of this function that will be used as the first line of 
       * output for this function parser. 
       */
      const QString _header;
      /*!
       * Steps of operation for this function. This is used as inline comments, following 
       * the same order as inline comments are found when parsing the input lines. 
       */
      const QStringList _operations;
      /*!
       * The regular expression used to match the first input line of this parser 
       * element's function within its parent scanner's source file. 
       */
      QString _headerExp;
      /*!
       * The number of indent spaces prepended to each output line of this function 
       * parser element. 
       */
      int _indent {0};
      /*!
       * True if the first line has not been read or false otherwise. This is used to 
       * determine the number of spaces on the first line. 
       */
      bool _first {true};
      /*!
       * The maximum number of spaces that are cut off from the beginning of each input 
       * line for this function parser. Only white space is cut off the beginning of 
       * input lines. 
       */
      int _cutOff {0};
      /*!
       * The current execution block depth as this function parser is reading in input 
       * lines. This is used to determine when the function ends in this parent scanner's 
       * source file. 
       */
      int _depth {0};
      /*!
       * True when the first input line of a single inline comment block has been read or 
       * false otherwise. This is used to add only one step of operation per block of 
       * inline comment input lines. 
       */
      bool _edgePast {false};
      /*!
       * Index to the next step of operation to be used to generate an inline comment for 
       * this function parser. 
       */
      int _nextOperation {0};
      /*!
       * The lines of output for this function parser generated by reading the input 
       * lines if found in its parent scanner's source file and used to provide its 
       * output lines with indent spaces prepended to each line. 
       */
      QStringList _lines;
   };
}



#endif
