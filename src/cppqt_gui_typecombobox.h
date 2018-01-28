#ifndef CPPQT_GUI_TYPECOMBOBOX_H
#define CPPQT_GUI_TYPECOMBOBOX_H
#include <QComboBox>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Gui
   {
      class TypeComboBox : public QComboBox
      {
         Q_OBJECT
      public:
         explicit TypeComboBox(AbstractBlock* block, QWidget* parent = nullptr);
         void setCurrentIndex(const QString& type);
         QString value() const;
      private slots:
         void comboActivated(int index);
         void typeListChanged();
      private:
         void findNamespaces(AbstractBlock* block);
         void buildComboList();
         Namespace* _local;
         Namespace* _global;
         int _localIndex {-2};
         int _globalIndex {-2};
      };
   }
}



#endif
