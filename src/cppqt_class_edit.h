#ifndef CPPQT_CLASS_EDIT_H
#define CPPQT_CLASS_EDIT_H
#include "cppqt_class.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the class block. 
    */
   class Class::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Class* block);
   protected:
      virtual QLayout* layout() override;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
