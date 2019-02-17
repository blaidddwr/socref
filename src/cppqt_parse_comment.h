#ifndef CPPQT_PARSE_COMMENT_H
#define CPPQT_PARSE_COMMENT_H
#include "basic_commentparser.h"
//



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    */
   namespace Parse
   {
      /*!
       */
      class Comment : public Basic::CommentParser
      {
         Q_OBJECT
      public:
         explicit Comment();
      };
   }
}

#endif
