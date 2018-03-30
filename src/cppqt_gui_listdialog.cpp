#include "cppqt_gui_listdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QItemSelection>
#include <QTableView>
#include <QHeaderView>
#include <QSettings>
#include "cppqt_gui_listdialog_model.h"
#include "gui_textdialog.h"
#include "application.h"



using namespace CppQt::Gui;
const char* ListDialog::_geometryKey {"cppqt.gui.listdialog.geometry"};






ListDialog::ListDialog(QWidget* parent):
   ::Gui::PersistentDialog(_geometryKey,parent)
{
   setupGui();
}






ListDialog::ListDialog(const QString& listItemTitle, QWidget* parent):
   ::Gui::PersistentDialog(_geometryKey,parent),
   _listItemTitle(listItemTitle)
{
   setupGui();
}






void ListDialog::setListItemTitle(const QString& listItemTitle)
{
   _listItemTitle = listItemTitle;
}






QStringList ListDialog::value() const
{
   return _list;
}






void ListDialog::setValue(const QStringList& list)
{
   _list = list;
   _model->setStringList(&_list);
   autoFitText();
   _current = QModelIndex();
}






void ListDialog::resizeEvent(QResizeEvent* event)
{
   QDialog::resizeEvent(event);
   autoFitText();
}






void ListDialog::addClicked()
{
   _model->addAfter(_current);
}






void ListDialog::removeClicked()
{
   if ( _current.isValid() )
   {
      _model->remove(_current);
      _current = QModelIndex();
   }
}






void ListDialog::moveUpClicked()
{
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveUp(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






void ListDialog::moveDownClicked()
{
   if ( _current.isValid() )
   {
      int oldRow {_current.row()};
      _current = _model->moveDown(_current);
      autoFitText(oldRow);
      autoFitText(_current.row());
   }
}






void ListDialog::doubleClicked(const QModelIndex& index)
{
   if ( index.row() < _list.size() )
   {
      ::Gui::TextDialog dialog;
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






void ListDialog::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   Q_UNUSED(deselected)
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();
}






void ListDialog::autoFitText()
{
   for (int i = 0; i < _list.size() ;++i) autoFitText(i);
}






void ListDialog::autoFitText(int row)
{
   QFontMetricsF metrics {_view->fontMetrics()};
   int width {_view->columnWidth(0)};
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
   _view->setRowHeight(row,metrics.boundingRect(text).height()*times - times + 12);
}






void ListDialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupBottomButtons());
   setLayout(layout);
}






QLayout* ListDialog::setupTop()
{
   _model = new Model(&_list,this);
   _view = new QTableView;
   _view->setModel(_model);
   _view->horizontalHeader()->setStretchLastSection(true);
   connect(_view,&QTableView::doubleClicked,this,&ListDialog::doubleClicked);
   connect(_view->selectionModel()
           ,&QItemSelectionModel::selectionChanged
           ,this
           ,&ListDialog::selectionChanged);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_view);
   ret->addLayout(setupSideButtons());
   return ret;
}






QLayout* ListDialog::setupSideButtons()
{
   QPushButton* add {new QPushButton(tr("&Add"))};
   QPushButton* remove {new QPushButton(tr("&Remove"))};
   QPushButton* moveUp {new QPushButton(tr("Mov&e Up"))};
   QPushButton* moveDown {new QPushButton(tr("Move &Down"))};
   connect(add,&QPushButton::clicked,this,&ListDialog::addClicked);
   connect(remove,&QPushButton::clicked,this,&ListDialog::removeClicked);
   connect(moveUp,&QPushButton::clicked,this,&ListDialog::moveUpClicked);
   connect(moveDown,&QPushButton::clicked,this,&ListDialog::moveDownClicked);
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(add);
   ret->addWidget(remove);
   ret->addWidget(moveUp);
   ret->addWidget(moveDown);
   ret->addStretch();
   return ret;
}






QLayout* ListDialog::setupBottomButtons()
{
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(ok,&QPushButton::clicked,this,&QDialog::accept);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addStretch();
   ret->addWidget(cancel);
   return ret;
}
