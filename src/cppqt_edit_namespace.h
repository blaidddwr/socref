#ifndef CPPQT_NAMESPACEEDIT_H
#define CPPQT_NAMESPACEEDIT_H
#include "gui_abstractedit.h"
#include "classes.h"
#include "cppqt_classes.h"



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
         virtual void cancelClicked() override final { emit finished(); }
      private:
         virtual std::unique_ptr<QLayout> createForm() override final;
         void createNameEdit();
         void createDescriptionEdit();
         CppQt::Namespace* _block;
         QLineEdit* _nameEdit;
         QPlainTextEdit* _descriptionEdit;
      };
   }
}



#endif
