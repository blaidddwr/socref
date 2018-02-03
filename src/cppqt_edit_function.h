#ifndef CPPQT_EDIT_FUNCTION_H
#define CPPQT_EDIT_FUNCTION_H
#include "cppqt_edit_base.h"
#include "cppqt_gui.h"



class QGroupBox;
class QPlainTextEdit;
class QLineEdit;



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
         QGroupBox* createFlagsEdit();
         //QWidget* createOperationsEdit();
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
      private:
         void createReturnWidgets();
         void createFlagWidgets();
         CppQt::Function* _block;
         Gui::TypeComboBox* _returnCombo;
         QPlainTextEdit* _returnEdit;
         QLineEdit* _preFlags;
         QLineEdit* _postFlags;
      };
   }
}



#endif
