#ifndef CPPQT_EDIT_DESTRUCTOR_H
#define CPPQT_EDIT_DESTRUCTOR_H
#include "cppqt_edit_function.h"



namespace CppQt
{
   namespace Edit
   {
      class Destructor : public Function
      {
         Q_OBJECT
      public:
         explicit Destructor(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      };
   }
}



#endif
