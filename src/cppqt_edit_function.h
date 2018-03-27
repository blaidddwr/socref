#ifndef CPPQT_EDIT_FUNCTION_H
#define CPPQT_EDIT_FUNCTION_H
#include "cppqt_edit_variable.h"
#include "cppqt_gui.h"
#include "gui.h"



class QCheckBox;
class QGridLayout;



namespace CppQt
{
   namespace Edit
   {
      class Function : public Variable
      {
         Q_OBJECT
      public:
         explicit Function(CppQt::Function* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         virtual void apply() override;
         virtual bool isConstExprCheckable() const override final;
         virtual bool isStaticCheckable() const override final;
         virtual bool isDefaultCheckable();
         virtual void updateProperties() override final;
         bool isDefaultChecked() const;
         bool isConstChecked() const;
         bool isNoExceptChecked() const;
         bool isVirtualChecked() const;
         bool isOverrideChecked() const;
         bool isFinalChecked() const;
         bool isAbstractChecked() const;
         void addReturn(QFormLayout* layout);
         void addProperties(QFormLayout* layout);
         void addOperations(QFormLayout* layout);
         QGridLayout* setupProperties();
         QWidget* setupDefault();
         QWidget* setupConst();
         QWidget* setupNoExcept();
         QWidget* setupVirtual();
         QWidget* setupOverride();
         QWidget* setupFinal();
         QWidget* setupAbstract();
      protected slots:
         virtual void checkBoxChanged(int state) override final;
      private slots:
         void operationsClicked();
      private:
         void setupReturn();
         QPushButton* setupOperations();
         CppQt::Function* _block;
         ::Gui::TextEdit* _returnEdit {nullptr};
         QCheckBox* _defaultBox {nullptr};
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
