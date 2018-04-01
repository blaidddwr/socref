#ifndef CPPQT_EDIT_TYPE_H
#define CPPQT_EDIT_TYPE_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QLineEdit;
class QFormLayout;
//



namespace CppQt
{
   namespace Edit
   {
      class Type : public ::Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         Type(CppQt::Type* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      protected:
         virtual void apply() override final;
      private slots:
         void textChanged(const QString& text);
      private:
         void addType(QFormLayout* layout);
         CppQt::Type* _block;
         QLineEdit* _typeEdit;
      };
   }
}



#endif
