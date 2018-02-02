#ifndef CPPQT_EDIT_VARIABLE_H
#define CPPQT_EDIT_VARIABLE_H
#include "cppqt_edit_base.h"
#include "cppqt_gui.h"



namespace CppQt
{
   namespace Edit
   {
      class Variable : public Base
      {
         Q_OBJECT
      public:
         Variable(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
      private:
         QLayout* createTypeCombo();
         CppQt::Variable* _block;
         Gui::TypeComboBox* _type;
      };
   }
}



#endif
