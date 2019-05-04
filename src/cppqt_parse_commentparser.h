#ifndef CPPQT_PARSE_COMMENTPARSER_H
#define CPPQT_PARSE_COMMENTPARSER_H
#include "basic_commentparser.h"
//



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * Contains all custom parser elements and parsing functions for the C++/Qt
    * project type.
    */
   namespace Parse
   {
      /*!
       * This specializes the basic comment parser class. This provides the begin,
       * middle, and ending strings for C++ code to its basic class. This also
       * provides the max columns C++/Qt project setting to the base class.
       */
      class CommentParser : public Basic::CommentParser
      {
         Q_OBJECT
      public:
         explicit CommentParser();
      };
   }
}

#endif
