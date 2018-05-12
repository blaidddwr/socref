#ifndef CPPQT_GUI_TYPESELECTION_H
#define CPPQT_GUI_TYPESELECTION_H
#include <QPushButton>
#include "cppqt.h"
#include "global.h"



namespace CppQt
{
   namespace Gui
   {
      class TypeSelection : public QPushButton
      {
         Q_OBJECT
      public:
         explicit TypeSelection(AbstractBlock* block, QWidget* parent = nullptr);
         QString value() const;
         void setValue(const QString& value);
      private slots:
         void selectionTriggered(int index);
      private:
         QMenu* buildRootMenu(AbstractBlock* block);
         QMenu* buildMenu(TypeList* block);
         void addType(QMenu* menu, Type* type);
         QList<QString> _types;
         QString _value;
      };
   }
}



#endif
