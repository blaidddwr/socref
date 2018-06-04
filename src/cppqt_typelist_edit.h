#ifndef CPPQT_TYPELIST_EDIT_H
#define CPPQT_TYPELIST_EDIT_H
#include "cppqt_typelist.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the type list block. 
    */
   class TypeList::Edit : public Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(TypeList* block);
   protected:
      virtual QLayout* layout() override final;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
