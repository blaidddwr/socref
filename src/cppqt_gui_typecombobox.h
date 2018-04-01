#ifndef CPPQT_GUI_TYPECOMBOBOX_H
#define CPPQT_GUI_TYPECOMBOBOX_H
#include <QComboBox>
#include "cppqt.h"
#include "global.h"



class QMenu;
class QPushButton;



namespace CppQt
{
   namespace Gui
   {
      class TypeComboBox : public QComboBox
      {
         Q_OBJECT
      public:
         explicit TypeComboBox(AbstractBlock* block, QWidget* parent = nullptr);
         QString value() const;
      private:
         void buildTypeList(AbstractBlock* block);
      };
   }
}



#endif
