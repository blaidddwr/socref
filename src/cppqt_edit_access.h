#ifndef CPPQT_EDIT_ACCESS_H
#define CPPQT_EDIT_ACCESS_H
#include "gui_abstractedit.h"
#include "global.h"
#include "cppqt.h"



class QComboBox;
class QFormLayout;



namespace CppQt
{
   namespace Edit
   {
      class Access : public Gui::AbstractEdit
      {
      public:
         Q_OBJECT
      public:
         explicit Access(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         virtual QLayout* layout() override final;
         virtual void apply() override final;
      private:
         void addCombo(QFormLayout* layout);
         void setupCombo();
         CppQt::Access* _block;
         QComboBox* _box;
      };
   }
}



#endif
