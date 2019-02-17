#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
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
      class Header : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit Header();
      };
   }
}

#endif
