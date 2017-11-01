#include <QTreeView>
#include <QScrollArea>
#include <QMenu>

#include "gui_blockview.h"
#include "blockmodel.h"
#include "abstractblock.h"
#include "gui_abstractedit.h"
#include "abstractblockfactory.h"



using namespace Gui;
AbstractBlock* BlockView::_copy {nullptr};






//@@
BlockView::BlockView(QWidget* parent):
   QSplitter(parent)
{
   // create tree view and add to splitter
   _treeView = new QTreeView;
   _treeView->setHeaderHidden(true);
   addWidget(_treeView);

   // create actions and menu
   createActions();
   createMenu();
   updateActions();

   // create scroll area and add to splitter
   _area = new QScrollArea;
   _area->setWidgetResizable(true);
   addWidget(_area);
}






//@@
void BlockView::setModel(BlockModel* model)
{
   // set tree view model
   _treeView->setModel(model);

   // set model, factory, and selection model
   _model = model;
   _factory = model->getFactory();
   _selectionModel = _treeView->selectionModel();

   // connect selection changed and model destroyed signals
   connect(_selectionModel,&QItemSelectionModel::selectionChanged,this
           ,&BlockView::selectionModelChanged);
   connect(model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);

   // manually call model changed to update menu items
   selectionModelChanged();
}






//@@
bool BlockView::canPaste() const
{
   // make sure model and copy exists
   if ( _model && _copy )
   {
      // make sure factory is same type as copy
      int a = _factory->getType();
      int b = _copy->getFactory().getType();
      if ( a == b )
      {
         // get block pointer from current selection and make sure copy's type is in current
         // selection's build list
         AbstractBlock* index {_model->getPointer(getSelection())};
         if ( index && _factory->getBuildList(index->getType()).contains(_copy->getType()) )
         {
            // all tests pass so paste can be done
            return true;
         }
      }
   }

   // one of tests failed and paste cannot be done
   return false;
}






//@@
void BlockView::addTriggered()
{
   // get action pointer and make sure model exists
   QAction* from {qobject_cast<QAction*>(sender())};
   if ( _model )
   {
      // insert new block of given type
      _model->insertRow(-1,getSelection(),_factory->makeBlock(from->data().toInt()));
   }
}






//@@
void BlockView::removeTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // remove index from model
      _model->removeRow(index.row(),_model->parent(index));
   }
}






//@@
void BlockView::editTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // create new edit widget for index
      AbstractBlock* pointer {_model->getPointer(index)};
      AbstractEdit* edit {_factory->makeEdit(pointer->getType(),pointer)};

      // initialize edit widget, connect signals, and set as view
      edit->initialize();
      connect(edit,&AbstractEdit::finished,this,&BlockView::editFinished);
      setView(edit);
   }
}






//@@
void BlockView::cutTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // cut block from given index and update actions
      setCopy(_model->cutRow(index.row(),_model->parent(index)));
      updateActions();
   }
}






//@@
void BlockView::copyTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // make new copy of block from index and update actions
      setCopy(_model->copyRow(index.row(),_model->parent(index)));
      updateActions();
   }
}






//@@
void BlockView::pasteTriggered()
{
   // if paste can happen insert new row in selection index
   if ( canPaste() )
   {
      _model->insertRow(-1,getSelection(),_copy->makeCopy());
   }
}






//@@
void BlockView::moveUpTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // move index up by one
      _model->moveRow(index.row(),index.row()-1,_model->parent(index));
   }
}






//@@
void BlockView::moveDownTriggered()
{
   // get selection index and make sure it is valid
   QModelIndex index {getSelection()};
   if ( index.isValid() )
   {
      // move index down by one
      _model->moveRow(index.row(),index.row()+2,_model->parent(index));
   }
}






//@@
void BlockView::selectionModelChanged()
{
   // initialize view to null and get selection
   QWidget* view {nullptr};
   QModelIndex index {getSelection()};

   // if index is valid set view pointer to view widget
   if ( index.isValid() )
   {
      AbstractBlock* pointer {_model->getPointer(index)};
      view = _factory->makeView(pointer->getType(),pointer);;
   }

   // set view, update actions and menu
   setView(view);
   updateActions();
   updateMenu();
}






//@@
void BlockView::modelDestroyed()
{
   // clear all data related to model
   setView(nullptr);
   _model = nullptr;
   _factory = nullptr;
   _selectionModel = nullptr;

   // update actions and menu
   updateActions();
   updateMenu();
}






//@@
void BlockView::createActions()
{
   // create remove action
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   connect(_removeAction,&QAction::triggered,this,&BlockView::removeTriggered);

   // create edit action
   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   connect(_editAction,&QAction::triggered,this,&BlockView::editTriggered);

   // create cut action
   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   connect(_cutAction,&QAction::triggered,this,&BlockView::cutTriggered);

   // create copy action
   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   connect(_copyAction,&QAction::triggered,this,&BlockView::copyTriggered);

   // create paste action
   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   connect(_pasteAction,&QAction::triggered,this,&BlockView::pasteTriggered);

   // create move up action
   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   connect(_moveUpAction,&QAction::triggered,this,&BlockView::moveUpTriggered);

   // create move down action
   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   connect(_moveDownAction,&QAction::triggered,this,&BlockView::moveDownTriggered);
}






//@@
void BlockView::createMenu()
{
   // create context menu
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






//@@
void BlockView::updateActions()
{
   // clear out previous actions list
   qDeleteAll(_addActions);
   _addActions.clear();

   // make sure model exists
   if ( _model )
   {
      // get build list
      QList<int> list {_factory->getBuildList(_model->getPointer(getSelection())->getType())};

      // iterate through build list and add action for each type
      for (const auto& i : list)
      {
         _addActions.append(new QAction(_factory->getName(i),this));
         _addActions.back()->setData(i);
         connect(_addActions.back(),&QAction::triggered,this,&BlockView::addTriggered);
      }
   }
   // determine if view has selection
   bool selected {hasSelection()};

   // enable or disable actions
   _removeAction->setDisabled(!selected);
   _editAction->setDisabled(!selected);
   _copyAction->setDisabled(!selected);
   _cutAction->setDisabled(!selected);
   _moveUpAction->setDisabled(!selected);
   _moveDownAction->setDisabled(!selected);
   _pasteAction->setDisabled(!canPaste());

   // enable or disable add menu
   _addMenu->setDisabled(!_model);
}






//@@
void BlockView::updateMenu()
{
   // clear menu and build from list of add actions
   _addMenu->clear();
   for (const auto& i : _addActions)
   {
      _addMenu->addAction(i);
   }
}






//@@
void BlockView::setView(QWidget* view)
{
   // if view already existed delete it
   if ( _view )
   {
      delete _view;
      _view = nullptr;
   }

   // if new view is not null set it as view
   if ( view )
   {
      _view = view;
      _area->setWidget(_view);
   }
}






//@@
void BlockView::setCopy(AbstractBlock* copy)
{
   // if copy already exists delete it
   if ( _copy )
   {
      delete _copy;
   }

   // set new copy pointer
   _copy = copy;
}







//@@
QModelIndex BlockView::getSelection() const
{
   // if selection model exists and it is not empty get first index it has
   QModelIndex ret;
   if ( _selectionModel && !_selectionModel->selection().isEmpty() )
   {
      ret = _selectionModel->selection().first().indexes().first();
   }

   // return selected index or invalid index for root
   return ret;
}
