#ifndef CPPQT_EDIT_BASE_H
#define CPPQT_EDIT_BASE_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"
#include "gui.h"



class QLineEdit;
class QLabel;
class QFormLayout;



namespace CppQt
{
   namespace Edit
   {
      class Base : public Gui::AbstractEdit
      {
      public:
         Q_OBJECT
      public:
         explicit Base(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         void addFields(QFormLayout* layout);
      protected slots:
         virtual void applyClicked() override;
      private:
         void setupNameEdit();
         void setupDescriptionEdit();
         CppQt::Base* _block;
         QLineEdit* _nameEdit;
         Gui::TextEdit* _descriptionEdit;
      };
   }
}



#endif
