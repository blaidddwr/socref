#ifndef CPPQT_PARSE_COMMENT_H
#define CPPQT_PARSE_COMMENT_H
#include "basic_commentparser.h"
//



namespace CppQt
{
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
