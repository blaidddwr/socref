#include <QTreeView>
#include <QScrollArea>
#include <QMenu>

#include "gui_blockview.h"
#include "blockmodel.h"
#include "abstractblock.h"
#include "gui_abstractedit.h"
#include "abstractblockfactory.h"



using namespace std;
using namespace Gui;



AbstractBlock* BlockView::_copy {nullptr};






BlockView::BlockView(QWidget* parent):
   QSplitter(parent)
{
   createTreeView();
   createArea();
   addWidget(_treeView);
   addWidget(_area);
   createActions();
   createMenu();
   updateActions();
}






void BlockView::setModel(BlockModel* model)
{
   _treeView->setModel(model);
   _model = model;
   _factory = model->factory();
   _selectionModel = _treeView->selectionModel();
   connect(_selectionModel,&QItemSelectionModel::selectionChanged,this,&BlockView::selectionModelChanged);
   connect(model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);
   selectionModelChanged();
}






bool BlockView::canPaste() const
{
   if ( _model && _copy )
   {
      int a = _factory->type();
      int b = _copy->factory().type();
      if ( a == b )
      {
         AbstractBlock* index {_model->pointer(selection())};
         if ( index && _factory->buildList(index->type()).contains(_copy->type()) )
         {
            return true;
         }
      }
   }
   return false;
}






void BlockView::addTriggered()
{
   QAction* from {qobject_cast<QAction*>(sender())};
   if ( _model )
   {
      _model->insertRow(-1,selection(),_factory->makeBlock(from->data().toInt()));
   }
}






void BlockView::removeTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      _model->removeRow(index.row(),_model->parent(index));
   }
}






void BlockView::editTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      AbstractBlock* pointer {_model->pointer(index)};
      unique_ptr<AbstractEdit> edit {_factory->makeEdit(pointer->type(),pointer)};
      edit->initialize();
      connect(edit.get(),&AbstractEdit::finished,this,&BlockView::editFinished);
      setView(edit.release());
   }
}






void BlockView::cutTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      setCopy(_model->cutRow(index.row(),_model->parent(index)).release());
      updateActions();
   }
}






void BlockView::copyTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      setCopy(_model->copyRow(index.row(),_model->parent(index)).release());
      updateActions();
   }
}






void BlockView::pasteTriggered()
{
   if ( canPaste() )
   {
      _model->insertRow(-1,selection(),_copy->makeCopy());
   }
}






void BlockView::moveUpTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      _model->moveRow(index.row(),index.row() - 1,_model->parent(index));
   }
}






void BlockView::moveDownTriggered()
{
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      _model->moveRow(index.row(),index.row() + 2,_model->parent(index));
   }
}






void BlockView::selectionModelChanged()
{
   unique_ptr<QWidget> view;
   QModelIndex index {selection()};
   if ( index.isValid() )
   {
      AbstractBlock* pointer {_model->pointer(index)};
      view = _factory->makeView(pointer->type(),pointer);;
   }
   setView(view.release());
   updateActions();
   updateMenu();
}






void BlockView::modelDestroyed()
{
   setView(nullptr);
   _model = nullptr;
   _factory = nullptr;
   _selectionModel = nullptr;
   updateActions();
   updateMenu();
}






void BlockView::createTreeView()
{
   _treeView = new QTreeView;
   _treeView->setHeaderHidden(true);
}






void BlockView::createArea()
{
   _area = new QScrollArea;
   _area->setWidgetResizable(true);
}






void BlockView::createActions()
{
   createRemoveAction();
   createEditAction();
   createCutAction();
   createCopyAction();
   createPasteAction();
   createMoveUpAction();
   createMoveDownAction();
}






void BlockView::createRemoveAction()
{
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   connect(_removeAction,&QAction::triggered,this,&BlockView::removeTriggered);
}






void BlockView::createEditAction()
{
   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   connect(_editAction,&QAction::triggered,this,&BlockView::editTriggered);
}






void BlockView::createCutAction()
{
   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   connect(_cutAction,&QAction::triggered,this,&BlockView::cutTriggered);
}






void BlockView::createCopyAction()
{
   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   connect(_copyAction,&QAction::triggered,this,&BlockView::copyTriggered);
}






void BlockView::createPasteAction()
{
   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   connect(_pasteAction,&QAction::triggered,this,&BlockView::pasteTriggered);
}






void BlockView::createMoveUpAction()
{
   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   connect(_moveUpAction,&QAction::triggered,this,&BlockView::moveUpTriggered);
}






void BlockView::createMoveDownAction()
{
   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   connect(_moveDownAction,&QAction::triggered,this,&BlockView::moveDownTriggered);
}






void BlockView::createMenu()
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






void BlockView::updateActions()
{
   updateAddActions();
   bool selected {hasSelection()};
   _removeAction->setDisabled(!selected);
   _editAction->setDisabled(!selected);
   _copyAction->setDisabled(!selected);
   _cutAction->setDisabled(!selected);
   _moveUpAction->setDisabled(!selected);
   _moveDownAction->setDisabled(!selected);
   _pasteAction->setDisabled(!canPaste());
   _addMenu->setDisabled(!_model);
}






void BlockView::updateAddActions()
{
   qDeleteAll(_addActions);
   _addActions.clear();
   if ( _model )
   {
      QList<int> list {_factory->buildList(_model->pointer(selection())->type())};
      for (const auto& i : list)
      {
         _addActions.append(new QAction(_factory->name(i),this));
         _addActions.back()->setData(i);
         connect(_addActions.back(),&QAction::triggered,this,&BlockView::addTriggered);
      }
   }
}






void BlockView::updateMenu()
{
   _addMenu->clear();
   for (const auto& i : _addActions)
   {
      _addMenu->addAction(i);
   }
}






void BlockView::setView(QWidget* view)
{
   if ( _view )
   {
      delete _view;
      _view = nullptr;
   }
   if ( view )
   {
      _view = view;
      _area->setWidget(_view);
   }
}






void BlockView::setCopy(AbstractBlock* copy)
{
   if ( _copy )
   {
      delete _copy;
   }
   _copy = copy;
}







QModelIndex BlockView::selection() const
{
   QModelIndex ret;
   if ( _selectionModel && !_selectionModel->selection().isEmpty() )
   {
      ret = _selectionModel->selection().first().indexes().first();
   }
   return ret;
}
