#ifndef CPPQT_EDIT_OPERATOR_H
#define CPPQT_EDIT_OPERATOR_H
#include "cppqt_edit_function.h"



namespace CppQt
{
   namespace Edit
   {
      class Operator : public Function
      {
         Q_OBJECT
      public:
         explicit Operator(CppQt::Operator* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual void apply() override final;
      private:
         void addOperation(QFormLayout* layout);
         void setupOperation();
         void addProperties(QFormLayout* layout);
         QGridLayout* setupProperties();
         QLineEdit* _operationEdit;
         CppQt::Operator* _block;
      };
   }
}



#endif
