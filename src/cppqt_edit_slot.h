#ifndef CPPQT_EDIT_SLOT_H
#define CPPQT_EDIT_SLOT_H
#include "cppqt_edit_function.h"



namespace CppQt
{
   namespace Edit
   {
      class Slot : public Function
      {
         Q_OBJECT
      public:
         explicit Slot(AbstractBlock* block, QWidget* parent = nullptr, bool isFinal = true);
      protected:
         virtual QLayout* layout() override;
      };
   }
}



#endif
