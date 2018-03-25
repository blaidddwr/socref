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
         Q_OBJECT
      public:
         explicit Base(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override;
         void addFields(QFormLayout* layout);
         void addName(QFormLayout* layout);
         void addDescription(QFormLayout* layout);
      protected:
         virtual void apply() override;
      private:
         void setupName();
         void setupDescription();
         CppQt::Base* _block;
         QLineEdit* _nameEdit {nullptr};
         Gui::TextEdit* _descriptionEdit {nullptr};
      };
   }
}



#endif
