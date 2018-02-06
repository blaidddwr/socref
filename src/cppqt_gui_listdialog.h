#ifndef CPPQT_GUI_LISTDIALOG_H
#define CPPQT_GUI_LISTDIALOG_H
#include <QDialog>



namespace CppQt
{
   namespace Gui
   {
      class ListDialog : public QDialog
      {
      public:
         explicit ListDialog(QWidget* parent = nullptr);
         explicit ListDialog(const QString& listItemTitle, QWidget* parent = nullptr);
         void setListItemTitle(const QString& listItemTitle);
         QStringList value() const;
      };
   }
}



#endif
