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
         explicit Operator(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual bool apply() override final;
      private:
         void addOperation(QFormLayout* layout);
         QLineEdit* _operationEdit;
         CppQt::Operator* _block;
      };
   }
}



#endif
