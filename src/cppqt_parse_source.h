#ifndef CPPQT_PARSE_SOURCE_H
#define CPPQT_PARSE_SOURCE_H
#include "basic_inputparser.h"
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
      class Source : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit Source(bool first = false);
      };
   }
}

#endif
