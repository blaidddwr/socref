#ifndef CPPQT_VIEW_SIGNAL_H
#define CPPQT_VIEW_SIGNAL_H
#include "cppqt_view_function.h"



namespace CppQt
{
   namespace View
   {
      class Signal : public Function
      {
         Q_OBJECT
      public:
         explicit Signal(const AbstractBlock* block, QWidget* parent = nullptr);
      };
   }
}



#endif
