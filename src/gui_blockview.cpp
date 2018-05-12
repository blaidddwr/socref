#include "gui_blockview.h"
#include <QTreeView>
#include <QScrollArea>
#include <QMenu>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <exception.h>
#include "gui_abstractedit.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "blockmodel.h"
#include "application.h"



using namespace std;
using namespace Gui;
//



/*!
 */
const char* BlockView::_stateKey {"gui.blockview.state"};
/*!
 */
AbstractBlock* BlockView::_copy;






/*!
 *
 * @param parent  
 */
BlockView::BlockView(QWidget* parent):
   QSplitter(parent)
{
   setupGui();
   restoreSettings();
   updateActions();
}






/*!
 */
BlockView::~BlockView()
{
   saveSettings();
}






/*!
 */
QMenu* BlockView::contextMenu() const
{
   return _contextMenu;
}






/*!
 */
bool BlockView::canPaste() const
{
   if ( _model && _copy && _factory->type() == _copy->factory().type() )
   {
      AbstractBlock* index {_model->pointer(_current)};
      if ( index && index->buildList().contains(_copy->type()) ) return true;
   }
   return false;
}






/*!
 *
 * @param model  
 */
void BlockView::setModel(BlockModel* model)
{
   _treeView->setModel(model);
   _model = model;
   _factory = model->factory();
   _selectionModel = _treeView->selectionModel();
   _current = QModelIndex();
   connect(_selectionModel
           ,&QItemSelectionModel::selectionChanged
           ,this
           ,&BlockView::selectionModelChanged);
   connect(_model,&BlockModel::dataChanged,this,&BlockView::modelDataChanged);
   connect(_model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);
}






/*!
 */
void BlockView::updateView()
{
   delete _view;
   if ( _current.isValid() )
   {
      AbstractBlock* pointer {_model->pointer(_current)};
      _view = pointer->makeView().release();
      if ( !_view )
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Got unexpected nullptr when creating view widget for block."));
         throw e;
      }
      _area->setWidget(_view);
      updateTitle(pointer);
   }
   else _view = nullptr;
}






/*!
 *
 * @param type  
 */
void BlockView::addTriggered(int type)
{
   if ( _model )
   {
      _model->insert(_current,_factory->makeBlock(type));
   }
}






/*!
 */
void BlockView::removeTriggered()
{
   if ( _current.isValid() )
   {
      _model->remove(_current);
      QModelIndexList list {_selectionModel->selection().indexes()};
      if ( list.isEmpty() ) _current = QModelIndex();
      else _current = list.first();
   }
}






/*!
 */
void BlockView::editTriggered()
{
   if ( _current.isValid() )
   {
      AbstractBlock* pointer {_model->pointer(_current)};
      unique_ptr<AbstractEdit> edit {pointer->makeEdit()};
      if ( edit )
      {
         edit->initialize(this);
         edit->setWindowIcon(pointer->icon());
         edit->setWindowTitle(tr("Edit %1").arg(_factory->name(pointer->type())));
         edit->exec();
         updateView();
      }
   }
}






/*!
 */
void BlockView::cutTriggered()
{
   if ( _current.isValid() )
   {
      setCopy(_model->cut(_current).release());
      updateActions();
   }
}






/*!
 */
void BlockView::copyTriggered()
{
   if ( _current.isValid() )
   {
      setCopy(_model->copy(_current).release());
      updateActions();
   }
}






/*!
 */
void BlockView::pasteTriggered()
{
   if ( canPaste() ) _model->insert(_current,_copy->makeCopy());
}






/*!
 */
void BlockView::moveUpTriggered()
{
   if ( _current.isValid() ) _current = _model->moveUp(_current);
}






/*!
 */
void BlockView::moveDownTriggered()
{
   if ( _current.isValid() ) _current = _model->moveDown(_current);
}






/*!
 *
 * @param selected  
 *
 * @param deselected  
 */
void BlockView::selectionModelChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   Q_UNUSED(deselected)
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();
   updateView();
   updateActions();
   updateMenu();
}






/*!
 */
void BlockView::modelDestroyed()
{
   _titleIcon->clear();
   _titleText->clear();
   _model = nullptr;
   _factory = nullptr;
   _selectionModel = nullptr;
   _current = QModelIndex();
   updateView();
   updateActions();
   updateMenu();
}






/*!
 *
 * @param topLeft  
 *
 * @param bottomRight  
 *
 * @param roles  
 */
void BlockView::modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
   Q_UNUSED(bottomRight)
   Q_UNUSED(roles)
   if ( topLeft == _current && _current.isValid() )
   {
      updateTitle(_model->pointer(_current));
      updateActions();
      updateMenu();
   }
}






/*!
 *
 * @param position  
 */
void BlockView::contextMenuRequested(const QPoint& position)
{
   _current = _treeView->indexAt(position);
   updateView();
   updateActions();
   updateMenu();
   _contextMenu->exec(QCursor::pos());
}






/*!
 */
void BlockView::updateActions()
{
   updateAddActions();
   bool selected {_current.isValid()};
   _removeAction->setDisabled(!selected);
   _editAction->setDisabled(!selected);
   _copyAction->setDisabled(!selected);
   _cutAction->setDisabled(!selected);
   _moveUpAction->setDisabled(!selected);
   _moveDownAction->setDisabled(!selected);
   _pasteAction->setDisabled(!canPaste());
   _addMenu->setDisabled(!_model);
}






/*!
 */
void BlockView::updateAddActions()
{
   qDeleteAll(_addActions);
   _addActions.clear();
   if ( _model )
   {
      for (auto type : _model->pointer(_current)->buildList())
      {
         _addActions.append(new QAction(_factory->name(type),this));
         connect(_addActions.back(),&QAction::triggered,[this,type]{ addTriggered(type); });
      }
   }
}






/*!
 */
void BlockView::updateMenu()
{
   _addMenu->clear();
   for (auto action : qAsConst(_addActions)) _addMenu->addAction(action);
   _addMenu->setDisabled(_addMenu->isEmpty());
}






/*!
 *
 * @param copy  
 */
void BlockView::setCopy(AbstractBlock* copy)
{
   delete _copy;
   _copy = copy;
}






/*!
 *
 * @param block  
 */
void BlockView::updateTitle(AbstractBlock* block)
{
   _titleIcon->clear();
   _titleText->clear();
   _titleIcon->setPixmap(block->icon().pixmap(_titleIconSize,_titleIconSize));
   _titleText->setText(block->name());
}






/*!
 */
void BlockView::restoreSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreState(settings.value(_stateKey).toByteArray());
}






/*!
 */
void BlockView::saveSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_stateKey,saveState());
}






/*!
 */
void BlockView::setupGui()
{
   setupTreeView();
   setupArea();
   setupActions();
   setupMenu();
}






/*!
 */
void BlockView::setupTreeView()
{
   _treeView = new QTreeView;
   _treeView->setHeaderHidden(true);
   _treeView->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(_treeView,&QTreeView::customContextMenuRequested,this,&BlockView::contextMenuRequested);
   addWidget(_treeView);
}






/*!
 */
void BlockView::setupArea()
{
   QVBoxLayout* layout {new QVBoxLayout};
   _area = new QScrollArea;
   _area->setWidgetResizable(true);
   layout->addLayout(setupTitleBar());
   layout->addSpacing(8);
   layout->addWidget(_area);
   QWidget* widget {new QWidget};
   widget->setLayout(layout);
   addWidget(widget);
}






/*!
 */
QLayout* BlockView::setupTitleBar()
{
   QHBoxLayout* ret {new QHBoxLayout};
   _titleIcon = new QLabel;
   _titleText = new QLabel;
   QFont font;
   font.setBold(true);
   _titleText->setFont(font);
   _titleText->setTextFormat(Qt::PlainText);
   ret->addWidget(_titleIcon);
   ret->addWidget(_titleText);
   ret->addStretch();
   return ret;
}






/*!
 */
void BlockView::setupActions()
{
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   _removeAction->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(_removeAction,&QAction::triggered,this,&BlockView::removeTriggered);

   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   _editAction->setShortcut(Qt::CTRL + Qt::Key_E);
   connect(_editAction,&QAction::triggered,this,&BlockView::editTriggered);

   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   _cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
   connect(_cutAction,&QAction::triggered,this,&BlockView::cutTriggered);

   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   _copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
   connect(_copyAction,&QAction::triggered,this,&BlockView::copyTriggered);

   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   _pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
   connect(_pasteAction,&QAction::triggered,this,&BlockView::pasteTriggered);

   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   _moveUpAction->setShortcut(Qt::CTRL + Qt::Key_Up);
   connect(_moveUpAction,&QAction::triggered,this,&BlockView::moveUpTriggered);

   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   _moveDownAction->setShortcut(Qt::CTRL + Qt::Key_Down);
   connect(_moveDownAction,&QAction::triggered,this,&BlockView::moveDownTriggered);
}






/*!
 */
void BlockView::setupMenu()
{
   _contextMenu = new QMenu(tr("&Edit"),this);
   _addMenu = _contextMenu->addMenu(tr("&Add"));
   _contextMenu->addAction(_removeAction);
   _contextMenu->addAction(_editAction);
   _contextMenu->addAction(_cutAction);
   _contextMenu->addAction(_copyAction);
   _contextMenu->addAction(_pasteAction);
   _contextMenu->addAction(_moveUpAction);
   _contextMenu->addAction(_moveDownAction);
}
