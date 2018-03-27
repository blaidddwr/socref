#ifndef CPPQT_VIEW_OPERATOR_H
#define CPPQT_VIEW_OPERATOR_H
#include "cppqt_view_function.h"



namespace CppQt
{
   namespace View
   {
      class Operator : public Function
      {
         Q_OBJECT
      public:
         using Function::Function;
      };
   }
}



#endif
