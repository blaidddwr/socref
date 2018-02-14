#ifndef CPPQT_VIEW_PARENT_H
#define CPPQT_VIEW_PARENT_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Parent : public Base
      {
         Q_OBJECT
      public:
         explicit Parent(const AbstractBlock* block, QWidget* parent = nullptr);
      };
   }
}



#endif
