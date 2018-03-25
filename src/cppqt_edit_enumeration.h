#ifndef CPPQT_EDIT_ENUMERATION_H
#define CPPQT_EDIT_ENUMERATION_H
#include "cppqt_edit_base.h"



class QCheckBox;



namespace CppQt
{
   namespace Edit
   {
      class Enumeration : public Base
      {
         Q_OBJECT
      public:
         explicit Enumeration(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual void apply() override final;
      private:
         void addClass(QFormLayout* layout);
         QCheckBox* setupClass();
         CppQt::Enumeration* _block;
         QCheckBox* _classBox;
      };
   }
}



#endif
