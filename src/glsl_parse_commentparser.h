#ifndef GLSL_PARSE_COMMENTPARSER_H
#define GLSL_PARSE_COMMENTPARSER_H
#include "basic_commentparser.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    */
   namespace Parse
   {
      /*!
       * This specializes the basic comment parser class. This provides the begin,
       * middle, and ending strings for GLSL code to its basic class. This also
       * provides the max columns GLSL project setting to the base class.
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
