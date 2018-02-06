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
         Variable(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         virtual bool isConstExprCheckable() const;
         virtual bool isStaticCheckable() const;
         void addCombo(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         const QCheckBox* constExprBox() const;
         const QCheckBox* staticBox() const;
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
         virtual void checkBoxChanged(int state);
      private:
         QLayout* createCombo();
         CppQt::Variable* _block;
         Gui::TypeComboBox* _type {nullptr};
         QCheckBox* _constExprBox {nullptr};
         QCheckBox* _staticBox {nullptr};
      };
   }
}



#endif
