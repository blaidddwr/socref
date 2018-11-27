#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include "basicblock.h"
//



namespace CppQt
{
   /*!
    * This is the type block. This is used to define a specific C++ type, either 
    * literal or template. 
    */
   class Type : public BasicBlock
   {
      Q_OBJECT
   public:
      /*!
       * Constructs a new type block. 
       */
      explicit Type() = default;
   };
}



#endif
