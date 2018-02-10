#ifndef CPPQT_GUI_LISTDIALOG_H
#define CPPQT_GUI_LISTDIALOG_H
#include <QDialog>
#include <QModelIndex>



class QTableView;
class QItemSelection;



namespace CppQt
{
   namespace Gui
   {
      class ListDialog : public QDialog
      {
         Q_OBJECT
      public:
         explicit ListDialog(QWidget* parent = nullptr);
         explicit ListDialog(const QString& listItemTitle, QWidget* parent = nullptr);
         void setListItemTitle(const QString& listItemTitle);
         QStringList value() const;
         void setValue(const QStringList& list);
      protected:
         virtual void resizeEvent(QResizeEvent* event) override final;
      private slots:
         void addClicked();
         void removeClicked();
         void moveUpClicked();
         void moveDownClicked();
         void doubleClicked(const QModelIndex& index);
         void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
      private:
         void autoFitText();
         void autoFitText(int row);
         void setupGui();
         QLayout* setupTop();
         QLayout* setupSideButtons();
         QLayout* setupBottomButtons();
         class Model;
         QString _listItemTitle {"Step"};
         QTableView* _view;
         Model* _model;
         QModelIndex _current;
         QStringList _list;
      };
   }
}



#endif
