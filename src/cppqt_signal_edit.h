#ifndef CPPQT_SIGNAL_EDIT_H
#define CPPQT_SIGNAL_EDIT_H
#include "cppqt_signal.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit dialog for the signal block type. 
    */
   class Signal::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Signal* block);
   protected:
      virtual QLayout* layout() override;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
