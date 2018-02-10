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
         void addTypeButtons(QFormLayout* layout);
      protected slots:
         virtual void applyClicked() override;
      private slots:
         void editGlobalTypesClicked();
         void editLocalTypesClicked();
      private:
         CppQt::Namespace* _block;
      };
   }
}



#endif
