#ifndef CPPQT_SLOT_EDIT_H
#define CPPQT_SLOT_EDIT_H
#include "cppqt_slot.h"
#include "cppqt_function_edit.h"
//



namespace CppQt
{
   /*!
    * This is the edit dialog for the slot block type. 
    */
   class Slot::Edit : public Function::Edit
   {
      Q_OBJECT
   public:
      explicit Edit(Slot* block);
   protected:
      virtual QLayout* leftLayout();
   };
}



#endif
