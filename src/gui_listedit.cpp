#include "gui_listedit.h"
#include <QHeaderView>
#include <QAction>
#include "gui_listedit_model.h"
#include "gui_textdialog.h"



using namespace Gui;
//






/*!
 * Constructs a new list edit widget with the given optional parent. 
 *
 * @param parent Optional parent for this new list edit widget. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this new object's view and actions. 
 */
ListEdit::ListEdit(QWidget* parent):
   QTableView(parent)
{
   // 1
   setupView();
   setupActions();
}






/*!
 * Constructs a new list edit widget with the given list item title and optional 
 * parent. 
 *
 * @param listItemTitle The list item title this new list edit widget will use as 
 *                      the title for each string row in its list. 
 *
 * @param parent Optional parent for this new list edit widget. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this new object's view and actions. 
 */
ListEdit::ListEdit(const QString& listItemTitle, QWidget* parent):
   QTableView(parent),
   _listItemTitle(listItemTitle)
{
   // 1
   setupView();
   setupActions();
}






/*!
 * Returns the current string list for this widget. 
 *
 * @return Current string list for this widget. 
 */
QStringList ListEdit::value() const
{
   return _list;
}






/*!
 * Sets the string list for this edit widget to the given list. 
 *
 * @param list New string list that this edit widget is set to by copying it. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's string list to the new one given by copying it, then reset 
 *    this object's string list model to the new model, then auto fit all new rows 
 *    of text, and then reset this object's current model index to invalid. 
 */
void ListEdit::setValue(const QStringList& list)
{
   // 1
   _list = list;
   _model->setList(&_list);
   autoFitText();
   _current = QModelIndex();
}






/*!
 * Implements the qt interface called whenever this widget is resized. This 
 * implementation simply auto fits all its text after calling its parent's resize 
 * event interface. 
 *
 * @param event Pointer to the qt resize event. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call qt table view's resize event interface and then auto fit all text rows. 
 */
void ListEdit::resizeEvent(QResizeEvent* event)
{
   QTableView::resizeEvent(event);
   autoFitText();
}






/*!
 * Called when the add action is triggered, adding a new string to this edit widget 
 * after the current index. 
 */
void ListEdit::addTriggered()
{
   _model->addAfter(_current);
   autoFitText();
}






/*!
 * Called when the remove action is triggered, removing this object's current model 
 * index if it is valid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current model index is valid then remove it from its model 
 *    and reset the current model index to invalid. 
 */
void ListEdit::removeTriggered()
{
   // 1
   if ( _current.isValid() )
   {
      _model->remove(_current);
      _current = QModelIndex();
   }
}






/*!
 * Called when the move up action is triggered, moving this object's current model 
 * index up by one if it is valid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current model index is valid then move it up by one in its 
 *    string list using the model and setting the current index to the new one 
 *    returned. Auto fit the new text for the two rows that have changed as a 
 *    result of the move. 
 */
void ListEdit::moveUpTriggered()
{
   // 1
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveUp(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






/*!
 * Called when the move down action is triggered, moving this object's current 
 * model index down by one if it is valid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current model index is valid then move it down by one in its 
 *    string list using the model and setting the current index to the new one 
 *    returned. Auto fit the new text for the two rows that have changed as a 
 *    result of the move. 
 */
void ListEdit::moveDownTriggered()
{
   // 1
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveDown(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






/*!
 * Called when a cell in this qt table view has been double clicked by the mouse. 
 * This opens a text dialog for the string of the given model index double clicked 
 * if it is valid and within range. 
 *
 * @param index  
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is not valid or is not within range of this object's 
 *    string list then return. 
 *
 * 2. Create a new text dialog _dialog_, setting its title and its text to the 
 *    string with the given model index, and then execute it in modal. If execution 
 *    fails then return. 
 *
 * 3. Set the value of the string with the given index to the text of the text 
 *    dialog _dialog_ and auto fit the new text at the given row. 
 */
void ListEdit::doubleClicked(const QModelIndex& index)
{
   // 1
   if ( !index.isValid() || index.row() < 0 || index.row() >= _list.size() ) return;

   // 2
   TextDialog dialog;
   dialog.setWindowTitle(QString(_listItemTitle)
                         .append(" #")
                         .append(QString::number(index.row() + 1)));
   dialog.setText(_model->data(index,Qt::EditRole).toString());
   if ( !dialog.exec() ) return;

   // 3
   _model->setData(index,dialog.text(),Qt::EditRole);
   autoFitText(index.row());
}






/*!
 * Called when the selection of this object's qt table view's selection model has 
 * changed. This implementation updates its object's current model index with the 
 * first in the given selected list. 
 *
 * @param selected List of model indexes that have been selected. 
 *
 * @param deselected List of model indexes that have been deselected. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given list of selected indexes is empty then set this object's current 
 *    model index to invalid, else set it to the first model index in the given 
 *    list of selected indexes. 
 */
void ListEdit::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   Q_UNUSED(deselected)

   // 1
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();
}






/*!
 * Constructs this new object's model and initializes the model and this qt table 
 * view. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create this object's model with this object's list and set this qt table 
 *    view's model to the new model. 
 *
 * 2. Initialize this qt table view and connect its relevant signals. 
 */
void ListEdit::setupView()
{
   // 1
   _model = new Model(&_list,this);
   setModel(_model);

   // 2
   horizontalHeader()->setStretchLastSection(true);
   horizontalHeader()->setHidden(true);
   connect(this,&QTableView::doubleClicked,this,&ListEdit::doubleClicked);
   connect(selectionModel()
           ,&QItemSelectionModel::selectionChanged
           ,this
           ,&ListEdit::selectionChanged);
}






/*!
 * Constructs and initializes all shortcut actions for this new list edit widget. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Add the new action. 
 *
 * 2. Add the remove action. 
 *
 * 3. Add the move up action. 
 *
 * 4. Add the move down action. 
 *
 * 5. Add all shortcut actions to this new widget. 
 */
void ListEdit::setupActions()
{
   // 1
   QAction* add {new QAction(this)};
   add->setShortcutContext(Qt::WidgetShortcut);
   add->setShortcut(Qt::CTRL + Qt::Key_A);
   connect(add,&QAction::triggered,this,&ListEdit::addTriggered);

   // 2
   QAction* remove {new QAction(this)};
   remove->setShortcutContext(Qt::WidgetShortcut);
   remove->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(remove,&QAction::triggered,this,&ListEdit::removeTriggered);

   // 3
   QAction* moveUp {new QAction(this)};
   moveUp->setShortcutContext(Qt::WidgetShortcut);
   moveUp->setShortcut(Qt::CTRL + Qt::Key_Up);
   connect(moveUp,&QAction::triggered,this,&ListEdit::moveUpTriggered);

   // 4
   QAction* moveDown {new QAction(this)};
   moveDown->setShortcutContext(Qt::WidgetShortcut);
   moveDown->setShortcut(Qt::CTRL + Qt::Key_Down);
   connect(moveDown,&QAction::triggered,this,&ListEdit::moveDownTriggered);

   // 5
   addAction(add);
   addAction(remove);
   addAction(moveUp);
   addAction(moveDown);
}






/*!
 * Updates the row height of all text cells for this list edit widget so all the 
 * text fits within the current width of this widget. 
 */
void ListEdit::autoFitText()
{
   for (int i = 0; i < _list.size() ;++i) autoFitText(i);
}






/*!
 * Updates the height of a given row of this list edit widget so all the text fits 
 * within the current width of its cell. 
 *
 * @param row The row whose height is updated so all text fits with the current 
 *            width of this list edit widget. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create font metrics, get the cell width _width_, and get the text of the 
 *    string at the given row _text_. 
 *
 * 2. Create and initialize _lines_, _start_, and _lastSpace_, all used for 
 *    determining the number of lines _text_ requires to fit within the _width_ 
 *    constraint. 
 *
 * 3. Iterate through _text_ to determine the number of lines needed to constrain 
 *    the text within the constraint _width_. Save the number of lines needed to 
 *    _lines_. 
 *
 * 4. Set the row height of the cell at the given row based off the calculated 
 *    lines needed to git within the cell's width. 
 */
void ListEdit::autoFitText(int row)
{
   static qreal wiggle {1.1};
   static qreal bias {0.5};
   // 1
   QFontMetricsF metrics {fontMetrics()};
   qreal width {static_cast<qreal>(columnWidth(0))};
   const QString text {_list.at(row)};

   // 2
   int lines {1};
   int start {0};
   int lastSpace {0};

   // 3
   for (int end = 0; end < text.size() ;++end)
   {
      if ( text.at(end) == QChar('\n') )
      {
         ++lines;
         start = lastSpace = end + 1;
      }
      else if ( text.at(end).isSpace() )
      {
         lastSpace = end;
      }
      else if ( metrics.width(text.mid(start,end - start + 1)) > width )
      {
         ++lines;
         start = lastSpace + 1;
      }
   }

   // 4
   setRowHeight(row
                ,static_cast<int>(metrics.lineSpacing()*wiggle*(static_cast<qreal>(lines) + bias)));
}
