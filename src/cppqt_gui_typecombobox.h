#ifndef CPPQT_GUI_TYPECOMBOBOX_H
#define CPPQT_GUI_TYPECOMBOBOX_H
#include <QWidget>
#include "global.h"
#include "cppqt.h"



class QComboBox;
class QMenu;
class QPushButton;



namespace CppQt
{
   namespace Gui
   {
      class TypeComboBox : public QWidget
      {
         Q_OBJECT
      public:
         explicit TypeComboBox(AbstractBlock* block, QWidget* parent = nullptr);
         void setCurrentIndex(const QString& type);
         QString value() const;
      private slots:
         void editGlobalTriggered();
         void editLocalTriggered();
         void typeListChanged();
      private:
         QPushButton* createButton();
         QMenu* createMenu();
         QAction* createGlobalAction();
         QAction* createLocalAction();
         void findNamespaces(AbstractBlock* block);
         void buildComboList();
         Namespace* _local;
         Namespace* _global;
         QComboBox* _box;
      };
   }
}



#endif
