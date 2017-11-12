#ifndef CPPQT_NAMESPACEEDIT_H
#define CPPQT_NAMESPACEEDIT_H
#include "gui_abstractedit.h"



class AbstractBlock;
class QLineEdit;
class QPlainTextEdit;



namespace CppQt
{
   class Namespace;



   namespace Edit
   {
      class Namespace : public Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         explicit Namespace(AbstractBlock* block, QWidget* parent = nullptr);
         virtual QLayout* createForm() override final;
      protected slots:
         virtual void okClicked() override final;
         virtual void applyClicked() override final;
         virtual void cancelClicked() override final { emit finished(); }
      private:
         void createNameEdit();
         void createDescriptionEdit();
         CppQt::Namespace* _block;
         QLineEdit* _nameEdit;
         QPlainTextEdit* _descriptionEdit;
      };
   }
}



#endif
