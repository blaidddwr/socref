#ifndef GLSL_PARSE_SOURCEPARSER_H
#define GLSL_PARSE_SOURCEPARSER_H
#include "basic_inputparser.h"



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
       * This specializes the basic input parser class. This will grab all custom
       * lines of code in a GLSL shader source file so they are saved and given back
       * as output lines. The first line is also grabbed and saved because there is no
       * default header for source files.
       */
      class SourceParser : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit SourceParser();
      };
   }
}

#endif
