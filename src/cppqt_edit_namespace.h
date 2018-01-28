#ifndef CPPQT_NAMESPACEEDIT_H
#define CPPQT_NAMESPACEEDIT_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QLineEdit;
class QPlainTextEdit;



namespace CppQt
{
   namespace Edit
   {
      class Namespace : public Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         explicit Namespace(AbstractBlock* block, QWidget* parent = nullptr);
      protected slots:
         virtual void okClicked() override final;
         virtual void applyClicked() override final;
         virtual void cancelClicked() override final;
      private slots:
         void editGlobalTypesClicked();
         void editLocalTypesClicked();
      private:
         virtual std::unique_ptr<QLayout> createForm() override final;
         void createNameEdit();
         void createDescriptionEdit();
         QLayout* createTypeButtons();
         CppQt::Namespace* _block;
         QLineEdit* _nameEdit;
         QPlainTextEdit* _descriptionEdit;
      };
   }
}



#endif
