#ifndef CPPQT_EDIT_VARIABLE_H
#define CPPQT_EDIT_VARIABLE_H
#include "cppqt_edit_base.h"
#include "cppqt_gui.h"



class QCheckBox;



namespace CppQt
{
   namespace Edit
   {
      class Variable : public Base
      {
         Q_OBJECT
      public:
         explicit Variable(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         virtual bool isConstExprCheckable() const;
         virtual bool isStaticCheckable() const;
         virtual void updateProperties();
         void addCombo(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         void addConstExpr(QFormLayout* layout);
         void addStatic(QFormLayout* layout);
         void addInitializer(QFormLayout* layout);
         bool isConstExprChecked() const;
         bool isStaticChecked() const;
      protected slots:
         virtual bool applyClicked() override;
         virtual void checkBoxChanged(int state);
      private:
         CppQt::Variable* _block;
         Gui::TypeComboBox* _type {nullptr};
         QCheckBox* _constExprBox {nullptr};
         QCheckBox* _staticBox {nullptr};
         QLineEdit* _initializerEdit {nullptr};
      };
   }
}



#endif
