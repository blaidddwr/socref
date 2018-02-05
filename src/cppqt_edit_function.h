#ifndef CPPQT_EDIT_FUNCTION_H
#define CPPQT_EDIT_FUNCTION_H
#include "cppqt_edit_variable.h"
#include "cppqt_gui.h"



class QPlainTextEdit;
class QCheckBox;



namespace CppQt
{
   namespace Edit
   {
      class Function : public Variable
      {
         Q_OBJECT
      public:
         Function(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         void addReturn(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
      private slots:
         void checkBoxChanged(int state);
      private:
         void createReturnWidgets();
         void createPropertiesWidgets();
         void connectProperties();
         void fillProperties();
         void updateProperties();
         CppQt::Function* _block;
         QPlainTextEdit* _returnEdit;
         QCheckBox* _virtualBox;
         QCheckBox* _constBox;
         QCheckBox* _noExceptBox;
         QCheckBox* _overrideBox;
         QCheckBox* _finalBox;
         QCheckBox* _abstractBox;
      };
   }
}



#endif
