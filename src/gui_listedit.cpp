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
 * @param block Pointer to the block that is contextually being used in text dialog 
 *              objects this widget opens. 
 *
 * @param parent Optional parent for this new list edit widget. 
 */
ListEdit::ListEdit(Abstract::Block* block, QWidget* parent):
   QTableView(parent),
   _block(block)
{
   // Setup this new object's view and actions. 
   setupView();
   setupActions();
}






/*!
 * Constructs a new list edit widget with the given list item title and optional 
 * parent. 
 *
 * @param block Pointer to the block that is contextually being used in text dialog 
 *              objects this widget opens. 
 *
 * @param listItemTitle The list item title this new list edit widget will use as 
 *                      the title for each string row in its list. 
 *
 * @param parent Optional parent for this new list edit widget. 
 */
ListEdit::ListEdit(Abstract::Block* block, const QString& listItemTitle, QWidget* parent):
   QTableView(parent),
   _listItemTitle(listItemTitle),
   _block(block)
{
   // Setup this new object's view and actions. 
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
 */
void ListEdit::setValue(const QStringList& list)
{
   // Set the new value of this object's string list and update its model. 
   _list = list;
   _model->setList(&_list);

   // Auto fit all new rows of text and reset this object's current model index to 
   // invalid. 
   autoFitText();
   _current = QModelIndex();
}






/*!
 * Implements _QWidget_ interface. This implementation simply auto fits all its 
 * text after calling its parent's resize event interface. 
 *
 * @param event See Qt docs. 
 */
void ListEdit::resizeEvent(QResizeEvent* event)
{
   // Call parent interface and then auto fit all text rows. 
   QTableView::resizeEvent(event);
   autoFitText();
}






/*!
 * Called when the add action is triggered, adding a new string to this edit widget 
 * after the current index. 
 */
void ListEdit::addTriggered()
{
   // Add a new string after this object's current model index and then auto fit the 
   // text of all rows. 
   _model->addAfter(_current);
   autoFitText();
}






/*!
 * Called when the remove action is triggered, removing this object's current model 
 * index if it is valid. 
 */
void ListEdit::removeTriggered()
{
   // Make sure this object's current model index is valid. 
   if ( _current.isValid() )
   {
      // Remove the current model index from the model and reset it to invalid. 
      _model->remove(_current);
      _current = QModelIndex();
   }
}






/*!
 * Called when the move up action is triggered, moving this object's current model 
 * index up by one if it is valid. 
 */
void ListEdit::moveUpTriggered()
{
   // Make sure this object's current model index is valid. 
   if ( _current.isValid() )
   {
      // Move this object's current model index up by one, updating the index and auto 
      // fitting all text rows. 
      _current = _model->moveUp(_current);
      autoFitText();
   }
}






/*!
 * Called when the move down action is triggered, moving this object's current 
 * model index down by one if it is valid. 
 */
void ListEdit::moveDownTriggered()
{
   // Make sure this object's current model index is valid. 
   if ( _current.isValid() )
   {
      // Move this object's current model index down by one, updating the index and auto 
      // fitting all text rows. 
      _current = _model->moveDown(_current);
      autoFitText();
   }
}






/*!
 * Called when a cell in this qt table view has been double clicked by the mouse. 
 * This opens a text dialog for the string of the given model index double clicked 
 * if it is valid and within range. 
 *
 * @param index  
 */
void ListEdit::doubleClicked(const QModelIndex& index)
{
   // Make sure the given index is valid and is within range of this object's string 
   // list. 
   if ( !index.isValid() || index.row() < 0 || index.row() >= _list.size() ) return;

   // Create a new text dialog, setting its title to the item title and its text to 
   // the string with the given model index. 
   TextDialog dialog(_block);
   dialog.setWindowTitle(QString(_listItemTitle)
                         .append(" #")
                         .append(QString::number(index.row() + 1)));
   dialog.setText(_model->data(index,Qt::EditRole).toString());

   // Execute the text dialog and make sure it returned with accept. 
   if ( dialog.exec() )
   {
      // Set the value of the string with the given index to the text of the text dialog 
      // and auto fit the new text at the given row. 
      _model->setData(index,dialog.text(),Qt::EditRole);
      autoFitText(index.row());
   }
}






/*!
 * Implements _QAbstractItemView_ interface. 
 *
 * @param selected See Qt docs. 
 *
 * @param deselected See Qt docs. 
 */
void ListEdit::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   // This does not care what was deselected. 
   Q_UNUSED(deselected)

   // If the given list of selected indexes is empty then set this object's current 
   // model index to invalid, else set it to the first model index in the given list 
   // of selected indexes. 
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();
}






/*!
 * Initializes the table view along with creating its model for this new list edit 
 * widget. 
 */
void ListEdit::setupView()
{
   // Create this object's model set it to this table view. 
   _model = new Model(&_list,this);
   setModel(_model);

   // Configure this table view's horizontal header. 
   horizontalHeader()->setStretchLastSection(true);
   horizontalHeader()->setHidden(true);

   // Connect the double clicked signal of this table view. 
   connect(this,&QTableView::doubleClicked,this,&ListEdit::doubleClicked);
}






/*!
 * Constructs and initializes all shortcut actions for this new list edit widget. 
 */
void ListEdit::setupActions()
{
   // Create and initialize this object's add action, connecting its triggered 
   // signal. 
   QAction* add {new QAction(this)};
   add->setShortcutContext(Qt::WidgetShortcut);
   add->setShortcut(Qt::CTRL + Qt::Key_A);
   connect(add,&QAction::triggered,this,&ListEdit::addTriggered);

   // Create and initialize this object's remove action, connecting its triggered 
   // signal. 
   QAction* remove {new QAction(this)};
   remove->setShortcutContext(Qt::WidgetShortcut);
   remove->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(remove,&QAction::triggered,this,&ListEdit::removeTriggered);

   // Create and initialize this object's move up action, connecting its triggered 
   // signal. 
   QAction* moveUp {new QAction(this)};
   moveUp->setShortcutContext(Qt::WidgetShortcut);
   moveUp->setShortcut(Qt::CTRL + Qt::Key_Up);
   connect(moveUp,&QAction::triggered,this,&ListEdit::moveUpTriggered);

   // Create and initialize this object's move down action, connecting its triggered 
   // signal. 
   QAction* moveDown {new QAction(this)};
   moveDown->setShortcutContext(Qt::WidgetShortcut);
   moveDown->setShortcut(Qt::CTRL + Qt::Key_Down);
   connect(moveDown,&QAction::triggered,this,&ListEdit::moveDownTriggered);

   // Add all shortcut actions to this new list edit widget. 
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
   // Iterate through all text rows and update their height to fit their text. 
   for (int i = 0; i < _list.size() ;++i) autoFitText(i);
}






/*!
 * Updates the height of a given row of this list edit widget so all the text fits 
 * within the current width of its cell. 
 *
 * @param row The row whose height is updated so all text fits with the current 
 *            width of this list edit widget. 
 */
void ListEdit::autoFitText(int row)
{
   // Initialize wiggle and bias used to help determine the row height at the end. 
   static qreal wiggle {1.1};
   static qreal bias {0.5};

   // Create font metrics, get the cell width, and get the text of the string at the 
   // given row. 
   QFontMetricsF metrics {fontMetrics()};
   qreal width {static_cast<qreal>(columnWidth(0))};
   const QString text {_list.at(row)};

   // Create and initialize all variables used for determining the number of lines 
   // required to fit the given row's text. 
   int lines {1};
   int start {0};
   int lastSpace {0};

   // Iterate through all characters of the given row's text. 
   for (int end = 0; end < text.size() ;++end)
   {
      // Check to see if this is a new line character. 
      if ( text.at(end) == QChar('\n') )
      {
         // Add a line and reset the start and last space markers to the next character. 
         ++lines;
         start = lastSpace = end + 1;
      }

      // Else if this character is a space then update the last space marker. 
      else if ( text.at(end).isSpace() ) lastSpace = end;

      // Else check to see if the current line is too long for the limiting width 
      // factor. 
      else if ( metrics.width(text.mid(start,end - start + 1)) > width )
      {
         // Add a line and reset the start marker to the character after the last space 
         // marker. 
         ++lines;
         start = lastSpace + 1;
      }
   }

   // Set the row height of the cell at the given row based off the calculated lines 
   // needed along with the wiggle and bias to fit within the cell's width. 
   setRowHeight(row
                ,static_cast<int>(metrics.lineSpacing()*wiggle*(static_cast<qreal>(lines) + bias)));
}
