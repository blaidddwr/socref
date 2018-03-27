#ifndef CPPQT_VIEW_SLOT_H
#define CPPQT_VIEW_SLOT_H
#include "cppqt_view_function.h"



namespace CppQt
{
   namespace View
   {
      class Slot : public Function
      {
         Q_OBJECT
      public:
         using Function::Function;
      };
   }
}



#endif
