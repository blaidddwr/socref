#ifndef CPPQT_ENUMERATION_EDIT_H
#define CPPQT_ENUMERATION_EDIT_H
#include "cppqt_enumeration.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the enumeration block. 
    */
   class Enumeration::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Enumeration* block);
   protected:
      virtual QLayout* layout() override final;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
