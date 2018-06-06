#ifndef CPPQT_ENUMVALUE_EDIT_H
#define CPPQT_ENUMVALUE_EDIT_H
#include "cppqt_enumvalue.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the enumeration value block. 
    */
   class EnumValue::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(EnumValue* block);
   protected:
      virtual QLayout* layout() override final;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
