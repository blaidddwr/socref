#ifndef CPPQT_EDIT_FUNCTION_H
#define CPPQT_EDIT_FUNCTION_H
#include "cppqt_edit_variable.h"
#include "cppqt_gui.h"
#include "gui.h"



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
         virtual QLayout* layout() override final;
         void addReturn(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         void addOperations(QFormLayout* layout);
      protected slots:
         virtual void okClicked() override final;
         virtual void applyClicked() override final;
         virtual void cancelClicked() override final;
         virtual void checkBoxChanged(int state) override final;
      private slots:
         void operationsClicked();
      private:
         void setupProperties();
         void connectProperties();
         void fillProperties();
         void updateProperties();
         CppQt::Function* _block;
         ::Gui::TextEdit* _returnEdit;
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
