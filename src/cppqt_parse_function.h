#ifndef CPPQT_PARSE_FUNCTION_H
#define CPPQT_PARSE_FUNCTION_H
#include "basic_functionparser.h"
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
       * This specializes the basic function parser class. This provides the in token,
       * out token, escape tokens, begin inline comment string, and maximum columns
       * C++/Qt project setting to its base class. The header and operations are
       * passed along as arguments since they are unique to each function.
       */
      class Function : public Basic::FunctionParser
      {
         Q_OBJECT
      public:
         explicit Function(const QString& header, const QStringList& operations);
      };
   }
}

#endif
