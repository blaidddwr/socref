#ifndef GLSL_STRUCT_H
#define GLSL_STRUCT_H
#include "glsl_variable.h"
//



namespace GLSL
{
   /*!
    * This is the struct block. This represents variable structures that can be 
    * defined in GLSL shader programs which is a collection of individual variables. 
    * Because this is nearly identical to the variable block type in GLSL this simply 
    * inherits from that class. 
    */
   class Struct : public Variable
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
