#ifndef CPPQT_ACCESS_EDIT_H
#define CPPQT_ACCESS_EDIT_H
#include "cppqt_access.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    */
   class Access::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Access* block);
   protected:
      virtual QLayout* layout() override;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
