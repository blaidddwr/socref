#ifndef GUI_LISTEDIT_H
#define GUI_LISTEDIT_H
#include <QTableView>
//



namespace Gui
{
   /*!
    */
   class ListEdit : public QTableView
   {
      Q_OBJECT
   public:
      explicit ListEdit(QWidget* parent = nullptr);
      explicit ListEdit(const QString& listItemTitle, QWidget* parent = nullptr);
      QStringList value() const;
      void setValue(const QStringList& list);
   protected:
      virtual void resizeEvent(QResizeEvent* event) override final;
   private slots:
      void addTriggered();
      void removeTriggered();
      void moveUpTriggered();
      void moveDownTriggered();
      void doubleClicked(const QModelIndex& index);
      void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
   private:
      class Model;
      void setupView();
      void setupActions();
      void autoFitText();
      void autoFitText(int row);
      /*!
       */
      QString _listItemTitle {"Step"};
      /*!
       */
      Model* _model;
      /*!
       */
      QModelIndex _current;
      /*!
       */
      QStringList _list;
   };
}



#endif
