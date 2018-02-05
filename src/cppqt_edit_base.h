#ifndef CPPQT_EDIT_BASE_H
#define CPPQT_EDIT_BASE_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QLineEdit;
class QPlainTextEdit;
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
         QLabel* createTitle(const QString& title);
      protected slots:
         virtual void applyClicked() override;
      private:
         void createNameEdit();
         void createDescriptionEdit();
         CppQt::Base* _block;
         QLineEdit* _nameEdit;
         QPlainTextEdit* _descriptionEdit;
      };
   }
}



#endif
