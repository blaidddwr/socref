#ifndef CPPQT_EDIT_DECLARATION_H
#define CPPQT_EDIT_DECLARATION_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QComboBox;
class QLineEdit;
class QFormLayout;
//



namespace CppQt
{
   namespace Edit
   {
      class Declaration : public ::Gui::AbstractEdit
      {
         Q_OBJECT
      public:
         Declaration(CppQt::Declaration* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
      protected:
         virtual void apply() override final;
      private:
         void addType(QFormLayout* layout);
         void addField(QFormLayout* layout);
         CppQt::Declaration* _block;
         QComboBox* _typeBox;
         QLineEdit* _fieldEdit;
      };
   }
}



#endif
