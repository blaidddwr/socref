#ifndef CPPQT_PARSE_HEADPARSER_H
#define CPPQT_PARSE_HEADPARSER_H
#include "basic_inputparser.h"
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
       * This specializes the basic input parser class. This will grab all custom
       * lines of code in a C++ header file so they are saved and given back as output
       * lines.
       */
      class HeadParser : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit HeadParser();
      };
   }
}

#endif
