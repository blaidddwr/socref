#ifndef CPPQT_NAMESPACEEDIT_H
#define CPPQT_NAMESPACEEDIT_H
#include "cppqt_edit_base.h"



namespace CppQt
{
   namespace Edit
   {
      class Namespace : public Base
      {
         Q_OBJECT
      public:
         explicit Namespace(CppQt::Namespace* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
      };
   }
}



#endif
