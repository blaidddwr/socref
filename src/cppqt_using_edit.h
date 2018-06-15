#ifndef CPPQT_USING_EDIT_H
#define CPPQT_USING_EDIT_H
#include "cppqt_using.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    */
   class Using::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Using* block);
   protected:
      virtual QLayout* layout() override;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
