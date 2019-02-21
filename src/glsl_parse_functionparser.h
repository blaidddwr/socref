#ifndef GLSL_PARSE_FUNCTIONPARSER_H
#define GLSL_PARSE_FUNCTIONPARSER_H
#include "basic_functionparser.h"



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
       * This specializes the basic function parser class. This provides the in token,
       * out token, escape tokens, begin inline comment string, and maximum columns
       * GLSL project setting to its base class. The header and operations are passed
       * along as arguments since they are unique to each function. For GLSL there are
       * no escape tokens so that argument is an empty string to its base class.
       */
      class FunctionParser : public Basic::FunctionParser
      {
         Q_OBJECT
      public:
         explicit FunctionParser(const QString& header, const QStringList& operations);
      };
   }
}

#endif
