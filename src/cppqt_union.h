#ifndef CPPQT_UNION_H
#define CPPQT_UNION_H
#include "cppqt_base.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the union block. This represents a union definition for C++ and can
    * only hold variables.
    */
   class Union : public Base
   {
      Q_OBJECT
   public:
      /*!
       * Constructs a new union block.
       */
      Q_INVOKABLE explicit Union() = default;
   };
}

#endif
