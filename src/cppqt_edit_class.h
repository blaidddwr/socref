#ifndef CPPQT_EDIT_CLASS_H
#define CPPQT_EDIT_CLASS_H
#include "cppqt_edit_namespace.h"



class QCheckBox;



namespace CppQt
{
   namespace Edit
   {
      class Class : public Namespace
      {
         Q_OBJECT
      public:
         explicit Class(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      protected slots:
         virtual bool applyClicked() override final;
      private:
         void addInheritance(QFormLayout* layout);
         void addQtObject(QFormLayout* layout);
         CppQt::Class* _block;
         QLineEdit* _inheritanceEdit;
         QCheckBox* _qtObjectBox;
      };
   }
}



#endif
