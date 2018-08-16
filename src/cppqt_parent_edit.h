#ifndef CPPQT_PARENT_EDIT_H
#define CPPQT_PARENT_EDIT_H
#include "cppqt_parent.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit dialog for the parent block type. 
    */
   class Parent::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Parent* block);
   protected:
      virtual QLayout* layout() override;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
