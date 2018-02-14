#ifndef CPPQT_EDIT_PARENT_H
#define CPPQT_EDIT_PARENT_H
#include "cppqt_edit_base.h"



class QComboBox;



namespace CppQt
{
   namespace Edit
   {
      class Parent : public Base
      {
         Q_OBJECT
      public:
         explicit Parent(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual bool apply() override final;
      private slots:
         void templateChanged(const QString& text);
      private:
         bool isValidTemplate();
         void addAccess(QFormLayout* layout);
         void addTemplate(QFormLayout* layout);
         void setupAccess();
         void setupTemplate();
         CppQt::Parent* _block;
         QComboBox* _accessBox;
         QLineEdit* _templateEdit;
      };
   }
}



#endif
