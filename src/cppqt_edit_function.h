#ifndef CPPQT_EDIT_FUNCTION_H
#define CPPQT_EDIT_FUNCTION_H
#include "cppqt_edit_base.h"
#include "cppqt_gui.h"



class QGroupBox;
class QPlainTextEdit;
class QCheckBox;



namespace CppQt
{
   namespace Edit
   {
      class Function : public Base
      {
         Q_OBJECT
      public:
         Function(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         QGroupBox* createReturnEdit();
         QGroupBox* createPropertiesEdit();
         //QWidget* createOperationsEdit();
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
      private:
         void createReturnWidgets();
         void createPropertiesWidgets();
         CppQt::Function* _block;
         Gui::TypeComboBox* _returnCombo;
         QPlainTextEdit* _returnEdit;
         QCheckBox* _virtualBox;
         QCheckBox* _staticBox;
         QCheckBox* _constBox;
         QCheckBox* _overrideBox;
         QCheckBox* _finalBox;
         QCheckBox* _abstractBox;
      };
   }
}



#endif
