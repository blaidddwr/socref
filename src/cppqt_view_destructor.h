#ifndef CPPQT_VIEW_DESTRUCTOR_H
#define CPPQT_VIEW_DESTRUCTOR_H
#include "cppqt_view_function.h"



namespace CppQt
{
   namespace View
   {
      class Destructor : public Function
      {
         Q_OBJECT
      public:
         using Function::Function;
      };
   }
}



#endif
