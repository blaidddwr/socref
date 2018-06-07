#include "gui_listedit.h"
#include <QHeaderView>
#include <QAction>
#include "gui_listedit_model.h"
#include "gui_textdialog.h"



using namespace Gui;
//






/*!
 *
 * @param parent  
 */
ListEdit::ListEdit(QWidget* parent):
   QTableView(parent)
{
   setupView();
   setupActions();
}






/*!
 *
 * @param listItemTitle  
 *
 * @param parent  
 */
ListEdit::ListEdit(const QString& listItemTitle, QWidget* parent):
   QTableView(parent),
   _listItemTitle(listItemTitle)
{
   setupView();
   setupActions();
}






/*!
 */
QStringList ListEdit::value() const
{
   return _list;
}






/*!
 *
 * @param list  
 */
void ListEdit::setValue(const QStringList& list)
{
   _list = list;
   _model->setList(&_list);
   autoFitText();
   _current = QModelIndex();
}






/*!
 *
 * @param event  
 */
void ListEdit::resizeEvent(QResizeEvent* event)
{
   QTableView::resizeEvent(event);
   autoFitText();
}






/*!
 */
void ListEdit::addTriggered()
{
   _model->addAfter(_current);
}






/*!
 */
void ListEdit::removeTriggered()
{
   if ( _current.isValid() )
   {
      _model->remove(_current);
      _current = QModelIndex();
   }
}






/*!
 */
void ListEdit::moveUpTriggered()
{
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveUp(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






/*!
 */
void ListEdit::moveDownTriggered()
{
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveDown(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






/*!
 *
 * @param index  
 */
void ListEdit::doubleClicked(const QModelIndex& index)
{
   if ( index.row() < _list.size() )
   {
      TextDialog dialog;
      dialog.setWindowTitle(QString(_listItemTitle)
                            .append(" #")
                            .append(QString::number(index.row() + 1)));
      dialog.setText(_model->data(index,Qt::EditRole).toString());
      if ( dialog.exec() && dialog.text() != _list.at(index.row()) )
      {
         _model->setData(index,dialog.text(),Qt::EditRole);
         autoFitText(index.row());
      }
   }
}






/*!
 *
 * @param selected  
 *
 * @param deselected  
 */
void ListEdit::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   Q_UNUSED(deselected)
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();
}






/*!
 */
void ListEdit::setupView()
{
   _model = new Model(&_list,this);
   setModel(_model);
   horizontalHeader()->setStretchLastSection(true);
   connect(this,&QTableView::doubleClicked,this,&ListEdit::doubleClicked);
   connect(selectionModel()
           ,&QItemSelectionModel::selectionChanged
           ,this
           ,&ListEdit::selectionChanged);
}






/*!
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
   moveUp->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(moveUp,&QAction::triggered,this,&ListEdit::moveUpTriggered);

   // 4
   QAction* moveDown {new QAction(this)};
   moveDown->setShortcutContext(Qt::WidgetShortcut);
   moveDown->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(moveDown,&QAction::triggered,this,&ListEdit::moveDownTriggered);

   // 5
   addAction(add);
   addAction(remove);
   addAction(moveUp);
   addAction(moveDown);
}






/*!
 */
void ListEdit::autoFitText()
{
   for (int i = 0; i < _list.size() ;++i) autoFitText(i);
}






/*!
 *
 * @param row  
 */
void ListEdit::autoFitText(int row)
{
   QFontMetricsF metrics {fontMetrics()};
   int width {columnWidth(0)};
   const QString text {_list.at(row)};
   QStringList words {text.split(' ')};
   int times {0};
   while ( !words.isEmpty() )
   {
      times++;
      qreal lineWidth {metrics.width(words.first()) + metrics.width(' ')};
      words.removeFirst();
      while ( !words.isEmpty() )
      {
         qreal wordWidth {metrics.width(words.first())};
         if ( (wordWidth + lineWidth) < width )
         {
            lineWidth += wordWidth + metrics.width(' ');
            words.removeFirst();
         }
         else break;
      }
   }
   for (auto ch : text)
   {
      if ( ch == QChar('\n') ) times++;
   }
   setRowHeight(row,metrics.boundingRect(text).height()*times - times + 12);
}
