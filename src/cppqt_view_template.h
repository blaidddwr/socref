#ifndef CPPQT_VIEW_TEMPLATE_H
#define CPPQT_VIEW_TEMPLATE_H
#include "cppqt_view_variable.h"



namespace CppQt
{
   namespace View
   {
      class Template : public Variable
      {
         Q_OBJECT
      public:
         explicit Template(const AbstractBlock* block, QWidget* parent = nullptr);
      };
   }
}



#endif
