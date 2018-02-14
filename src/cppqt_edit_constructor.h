#ifndef CPPQT_EDIT_CONSTRUCTOR_H
#define CPPQT_EDIT_CONSTRUCTOR_H
#include "cppqt_edit_function.h"



namespace CppQt
{
   namespace Edit
   {
      class Constructor : public Function
      {
         Q_OBJECT
      public:
         explicit Constructor(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      private:
         void addProperties(QFormLayout* layout);
      };
   }
}



#endif
