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
    */
   namespace Parse
   {
      /*!
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
