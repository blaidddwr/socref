#ifndef CPPQT_NAMESPACEEDIT_H
#define CPPQT_NAMESPACEEDIT_H
#include "cppqt_edit_base.h"



namespace CppQt
{
   namespace Edit
   {
      class Namespace : public Base
      {
         Q_OBJECT
      public:
         explicit Namespace(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
      protected slots:
         virtual void okClicked() override;
         virtual void applyClicked() override;
         virtual void cancelClicked() override;
      private slots:
         void editGlobalTypesClicked();
         void editLocalTypesClicked();
      private:
         void addTypeButtons(QFormLayout* layout);
         CppQt::Namespace* _block;
      };
   }
}



#endif
