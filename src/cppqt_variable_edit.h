#ifndef CPPQT_VARIABLE_EDIT_H
#define CPPQT_VARIABLE_EDIT_H
#include "cppqt_variable.h"
#include "gui_abstractedit.h"
#include "cppqt_gui.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the variable block. 
    */
   class Variable::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Variable* block);
   protected:
      virtual QLayout* layout() override final;
      virtual void apply() override final;
      virtual QString fieldTitle(int index) const override final;
   private:
      void addTypeEdit(QFormLayout* form);
      /*!
       * The variable block that this edit dialog is editing. 
       */
      Variable* _block;
      /*!
       * This object's type edit widget for editing the type field. 
       */
      Gui::TypeSelection* _typeEdit;
   };
}



#endif
