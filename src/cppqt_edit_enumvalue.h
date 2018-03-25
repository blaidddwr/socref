#ifndef CPPQT_EDIT_ENUMVALUE_H
#define CPPQT_EDIT_ENUMVALUE_H
#include "cppqt_edit_base.h"



namespace CppQt
{
   namespace Edit
   {
      class EnumValue : public Base
      {
         Q_OBJECT
      public:
         explicit EnumValue(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual void apply() override final;
      private:
         void addValue(QFormLayout* layout);
         CppQt::EnumValue* _block;
         QLineEdit* _valueEdit;
      };
   }
}



#endif
