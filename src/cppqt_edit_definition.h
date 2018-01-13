#ifndef CPPQT_EDIT_DEFINITION_H
#define CPPQT_EDIT_DEFINITION_H
#include "gui_abstractedit.h"
#include "classes.h"
#include "cppqt_classes.h"
#include "cppqt_type_classes.h"



namespace CppQt
{
   namespace Edit
   {
      class Definition : public Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         explicit Definition(AbstractBlock* block, QWidget* parent = nullptr);
      protected slots:
         virtual void okClicked() override final;
         virtual void applyClicked() override final { apply(); }
         virtual void cancelClicked() override final { emit finished(); }
      private:
         virtual std::unique_ptr<QLayout> createForm() override final;
         bool apply();
         CppQt::Definition* _block;
         Type::Edit* _typeEdit;
      };
   }
}



#endif
