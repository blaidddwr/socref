#ifndef CPPQT_PARSE_SOURCEPARSER_H
#define CPPQT_PARSE_SOURCEPARSER_H
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
       * lines of code in a C++ source file so they are saved and given back as output
       * lines. Because of the special main source file this can include the very
       * first line of the source file.
       */
      class SourceParser : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit SourceParser(bool first = false);
      };
   }
}

#endif
