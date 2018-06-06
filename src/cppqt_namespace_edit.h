#ifndef CPPQT_NAMESPACE_EDIT_H
#define CPPQT_NAMESPACE_EDIT_H
#include "cppqt_namespace.h"
#include "gui_abstractedit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the namespace block. 
    */
   class Namespace::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Namespace* block);
   protected:
      virtual QLayout* layout() override final;
      virtual QString fieldTitle(int index) const override final;
   };
}



#endif
