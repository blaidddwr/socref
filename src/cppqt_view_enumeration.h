#ifndef CPPQT_VIEW_ENUMERATION_H
#define CPPQT_VIEW_ENUMERATION_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Enumeration : public Base
      {
         Q_OBJECT
      public:
         explicit Enumeration(const AbstractBlock* block, QWidget* parent = nullptr);
      };
   }
}



#endif
