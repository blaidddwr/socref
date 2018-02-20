#ifndef CPPQT_EDIT_CONSTRUCTOR_H
#define CPPQT_EDIT_CONSTRUCTOR_H
#include "cppqt_edit_function.h"



class QCheckBox;



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
         virtual bool apply() override final;
      private:
         void addProperties(QFormLayout* layout);
         QLayout* setupProperties();
         QWidget* setupExplicit();
         CppQt::Constructor* _block;
         QCheckBox* _explicitBox;
      };
   }
}



#endif
