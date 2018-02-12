#ifndef CPPQT_EDIT_SIGNAL_H
#define CPPQT_EDIT_SIGNAL_H
#include "cppqt_edit_slot.h"



namespace CppQt
{
   namespace Edit
   {
      class Signal : public Slot
      {
         Q_OBJECT
      public:
         explicit Signal(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      };
   }
}



#endif
