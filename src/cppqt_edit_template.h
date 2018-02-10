#ifndef CPPQT_EDIT_TEMPLATE_H
#define CPPQT_EDIT_TEMPLATE_H
#include "cppqt_edit_variable.h"



namespace CppQt
{
   namespace Edit
   {
      class Template : public Variable
      {
         Q_OBJECT
      public:
         Template(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      };
   }
}



#endif
