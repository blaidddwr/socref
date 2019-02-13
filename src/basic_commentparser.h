#ifndef BASIC_COMMENTPARSER_H
#define BASIC_COMMENTPARSER_H
#include "abstract_parser.h"
//



namespace Basic
{
   /*!
    * This implements the abstract parser class. This provides lines representing a 
    * comment block for source code. This does not require any input as it only 
    * outputs the comments it is provided at initialization. This will wrap comments 
    * provided to it to be a given maximum number of columns per line. A header can 
    * also be given that is kept to the left of the provided comment. Lastly empty 
    * lines can be added that will have the basic comment token provided. Begin and 
    * end lines can also be provided that is used to begin and close a comment block 
    * when provided to a comment parser element's parent scanner. 
    * 
    * This parser also provides setting an indentation of spaces before each comment 
    * line of its comment block. This indent is not counted towards the maximum number 
    * of columns per line. 
    */
   class CommentParser : public Abstract::Parser
   {
      Q_OBJECT
   public:
      virtual QStringList output() const override final;
   public:
      explicit CommentParser(const QString& begin, const QString& middle, const QString& end, int maxColumns);
      void setIndent(int value);
      void add(const QString& text);
      void add(const QString& header, const QString& text);
      void add(int count);
   private:
      struct Node { QString header; QString text; int footerLines {0}; };
   private:
      QStringList createComment(QString header, const QString& text) const;
      /*!
       * The number of space characters prepended to each line of comment block output 
       * when this parser generates it for its parent scanner. 
       */
      int _indent {0};
      /*!
       * The beginning line that is provided as the first line for this comment parser 
       * element. 
       */
      const QString _begin;
      /*!
       * The middle token that is prepended to each comment line provided by this comment 
       * parser element. 
       */
      const QString _middle;
      /*!
       * The ending line that is provided as the last line for this comment parser 
       * element. 
       */
      const QString _end;
      /*!
       * The maximum number of columns per comment line provided by this comment parser 
       * element. 
       */
      int _maxColumns;
      /*!
       * The number of blank lines provided before the first comment block by this 
       * comment parser element. 
       */
      int _headerLines {0};
      /*!
       * List of comment blocks that are used for generating the provided lines in 
       * between the optional begin and end lines by this comment parser element. 
       */
      QList<Node> _comments;
   };
}



#endif
