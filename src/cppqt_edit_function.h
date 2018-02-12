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
         explicit Function(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         virtual bool isConstExprCheckable() const override final;
         virtual bool isStaticCheckable() const override final;
         virtual void updateProperties() override final;
         bool isConstChecked() const;
         bool isNoExceptChecked() const;
         bool isVirtualChecked() const;
         bool isOverrideChecked() const;
         bool isFinalChecked() const;
         bool isAbstractChecked() const;
         void addReturn(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         void addConst(QFormLayout* layout);
         void addNoExcept(QFormLayout* layout);
         void addVirtual(QFormLayout* layout);
         void addOverride(QFormLayout* layout);
         void addFinal(QFormLayout* layout);
         void addAbstract(QFormLayout* layout);
         void addOperations(QFormLayout* layout);
         virtual bool apply() override;
      protected slots:
         virtual void checkBoxChanged(int state) override final;
      private slots:
         void operationsClicked();
      private:
         CppQt::Function* _block;
         ::Gui::TextEdit* _returnEdit {nullptr};
         QCheckBox* _virtualBox {nullptr};
         QCheckBox* _constBox {nullptr};
         QCheckBox* _noExceptBox {nullptr};
         QCheckBox* _overrideBox {nullptr};
         QCheckBox* _finalBox {nullptr};
         QCheckBox* _abstractBox {nullptr};
      };
   }
}



#endif
