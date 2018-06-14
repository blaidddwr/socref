#ifndef CPPQT_VARIABLE_EDIT_H
#define CPPQT_VARIABLE_EDIT_H
#include "cppqt_variable.h"
#include "gui_abstractedit.h"
#include "cppqt_gui.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the variable block. Because this is inherited by 
    * other classes the adding of the type edit widget is provided. Adding it is 
    * optional and if it is not added then it will never be applied to the variable 
    * block edited. 
    */
   class Variable::Edit : public ::Gui::AbstractEdit
   {
      Q_OBJECT
   public:
      explicit Edit(Variable* block);
   protected:
      virtual QLayout* layout() override;
      virtual void apply() override final;
      virtual QString fieldTitle(int index) const override final;
   protected:
      void addTypeEdit(QFormLayout* form);
   private:
      /*!
       * The variable block that this edit dialog is editing. 
       */
      Variable* _block;
      /*!
       * This object's type edit widget for editing the type field or a null pointer if 
       * the type edit widget is never added. 
       */
      Gui::TypeSelection* _typeEdit {nullptr};
   };
}



#endif
