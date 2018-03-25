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
         explicit Variable(AbstractBlock* block, QWidget* parent = nullptr, bool isFinal = true);
      protected:
         virtual QLayout* layout() override;
         virtual void apply() override;
         virtual bool isConstExprCheckable() const;
         virtual bool isStaticCheckable() const;
         virtual bool isMutableCheckable() const;
         virtual void updateProperties();
         void addCombo(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         void addInitializer(QFormLayout* layout);
         bool isConstExprChecked() const;
         bool isStaticChecked() const;
         bool isMutableChecked() const;
         QWidget* setupConstExpr();
         QWidget* setupStatic();
         QWidget* setupMutable();
      protected slots:
         virtual void checkBoxChanged(int state);
      private:
         void setupCombo();
         QLayout* setupProperties();
         void setupInitializer();
         CppQt::Variable* _block;
         Gui::TypeComboBox* _type {nullptr};
         QCheckBox* _constExprBox {nullptr};
         QCheckBox* _staticBox {nullptr};
         QCheckBox* _mutableBox {nullptr};
         QLineEdit* _initializerEdit {nullptr};
      };
   }
}



#endif
