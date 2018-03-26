#ifndef CPPQT_EDIT_PARENT_H
#define CPPQT_EDIT_PARENT_H
#include "gui_abstractedit.h"
#include "cppqt.h"



class QComboBox;
class QLineEdit;
class QFormLayout;



namespace CppQt
{
   namespace Edit
   {
      class Parent : public ::Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         explicit Parent(CppQt::Parent* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual void apply() override final;
      private slots:
         void classChanged(const QString& text);
      private:
         void addAccess(QFormLayout* layout);
         void addClass(QFormLayout* layout);
         void setupAccess();
         void setupClass();
         CppQt::Parent* _block;
         QComboBox* _accessBox;
         QLineEdit* _classEdit;
      };
   }
}



#endif
