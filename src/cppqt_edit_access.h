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
         virtual QLayout* layout() override;
      protected slots:
         virtual void applyClicked() override;
      private:
         void addComboBox(QFormLayout* layout);
         static const QStringList _options;
         CppQt::Access* _block;
         QComboBox* _box;
      };
   }
}



#endif
