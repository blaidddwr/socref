#ifndef CPPQT_GUI_TYPELISTDIALOG_H
#define CPPQT_GUI_TYPELISTDIALOG_H
#include <QDialog>
#include "cppqt.h"



class QListWidget;
class QListWidgetItem;



namespace CppQt
{
   namespace Gui
   {
      class TypeListDialog : public QDialog
      {
         Q_OBJECT
      public:
         explicit TypeListDialog(Namespace* block, QWidget* parent = nullptr);
         virtual ~TypeListDialog() override final;
      private slots:
         void itemDoubleClicked(QListWidgetItem* item);
         void addClicked();
         void removeClicked();
         void okClicked();
         void applyClicked();
      private:
         bool isDuplicate(const QString& name);
         void restoreSettings();
         void saveSettings();
         void setupGui();
         QLayout* setupTop();
         QLayout* setupTopButtons();
         QLayout* setupBottomButtons();
         static const char* _geometryKey;
         Namespace* _block;
         QListWidget* _list;
      };
   }
}



#endif
