#ifndef CPPQT_EDIT_BASE_H
#define CPPQT_EDIT_BASE_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QLineEdit;
class QPlainTextEdit;
class QLabel;



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
         QLabel* createLabel(const QString& name, Qt::Alignment vertical = Qt::AlignTop);
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
