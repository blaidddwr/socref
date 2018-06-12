#ifndef GUI_LISTEDIT_H
#define GUI_LISTEDIT_H
#include <QTableView>
//



namespace Gui
{
   /*!
    * This is a list edit widget that allows the user to edit a list of strings. 
    * Keyboard shortcuts for adding, removing, and moving items within the list are 
    * provided to the user. This widget displays each string in its list as a table 
    * with a single column. A title for each string is also displayed and given a 
    * number, the first string being number 1 and incremented from there. The height 
    * of each row is auto adjusted to make it so all the text fits within the 
    * horizontal limits of this widget. 
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
       * The title text used for the title of each text row in this list edit widget. 
       * This title is appended with the row number. 
       */
      QString _listItemTitle {"Step"};
      /*!
       * Pointer to the string list model for this list edit widget. 
       */
      Model* _model;
      /*!
       * The currently selected model index for this list edit widget. 
       */
      QModelIndex _current;
      /*!
       * The string list for this list edit widget. 
       */
      QStringList _list;
   };
}



#endif
