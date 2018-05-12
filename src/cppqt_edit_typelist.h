#ifndef CPPQT_EDIT_TYPELIST_H
#define CPPQT_EDIT_TYPELIST_H
#include "cppqt_edit_base.h"



namespace CppQt
{
   namespace Edit
   {
      class TypeList : public Base
      {
         Q_OBJECT
      public:
         TypeList(CppQt::TypeList* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
      };
   }
}



#endif
