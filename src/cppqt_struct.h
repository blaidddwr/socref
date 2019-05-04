#ifndef CPPQT_STRUCT_H
#define CPPQT_STRUCT_H
#include "cppqt_base.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the struct block. This represents a structure definition for C++ and
    * can only hold variables.
    */
   class Struct : public Base
   {
      Q_OBJECT
   public:
      /*!
       * Constructs a new struct block.
       */
      Q_INVOKABLE explicit Struct() = default;
   };
}

#endif
