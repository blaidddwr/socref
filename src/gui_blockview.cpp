#include <QTreeView>
#include <QScrollArea>
#include <QMenu>

#include "gui_blockview.h"
#include "blockmodel.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"



using namespace Gui;






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
   addWidget(_area);
}






//@@
void BlockView::setModel(BlockModel* model)
{
   // set tree view model
   _treeView->setModel(model);

   // connect selection changed and model destroyed signals
   connect(_treeView->selectionModel(),&QItemSelectionModel::selectionChanged,this
           ,&BlockView::selectionModelChanged);
   connect(model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);

   // manually call model changed to update menu items
   selectionModelChanged();
}






//@@
bool BlockView::canPaste() const
{
   // get block model, make sure it and copy exists
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   if ( model && _copy )
   {
      // get model's block factory and make sure it is same type as copy
      const AbstractBlockFactory* factory {model->getFactory()};
      if ( factory && factory->getType() == _copy->getFactory().getType() )
      {
         // get block pointer from current selection and make sure copy's type is in current
         // selection's build list
         AbstractBlock* index {model->getPointer(getSelection())};
         if ( index && factory->getBuildList(index->getType()).contains(_copy->getType()) )
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
   // get action and block model pointers, make sure model exists
   QAction* from {qobject_cast<QAction*>(sender())};
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   if ( model )
   {
      // get block factory and insert new block of given type
      const AbstractBlockFactory* factory {model->getFactory()};
      model->insertRow(-1,getSelection(),factory->makeBlock(from->data().toInt()));
   }
}






//@@
void BlockView::removeTriggered()
{
   // get block model and make sure it exists
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   if ( model )
   {
      // get selection index and make sure it is valid
      QModelIndex index {getSelection()};
      if ( index.isValid() )
      {
         // remove index from model
         model->removeRow(index.row(),model->parent(index));
      }
   }
}






//@@
void BlockView::editTriggered()
{
}






//@@
void BlockView::cutTriggered()
{
}






//@@
void BlockView::copyTriggered()
{
}






//@@
void BlockView::pasteTriggered()
{
}






//@@
void BlockView::moveUpTriggered()
{
}






//@@
void BlockView::moveDownTriggered()
{
}






//@@
void BlockView::selectionModelChanged()
{
   // get block model, factory, and index
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   const AbstractBlockFactory* factory {model->getFactory()};
   QModelIndex index {getSelection()};

   // if block view already existed delete it
   if ( _view )
   {
      delete _view;
      _view = nullptr;
   }

   // if index is valid create new view and add it to layout
   if ( index.isValid() )
   {
      AbstractBlock* pointer {model->getPointer(index)};
      _view = factory->makeView(pointer->getType(),pointer);
      _area->setWidget(_view);
   }

   // update actions and menu
   updateActions();
   updateMenu();
}






//@@
void BlockView::modelDestroyed()
{
   // model was destroyed to clear any existing view
   delete _view;
   _view = nullptr;

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
   connect(_removeAction,&QAction::triggered,this,&BlockView::editTriggered);

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

   // get model and make sure it exists
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   if ( model )
   {
      // get factory and build list
      const AbstractBlockFactory* factory {model->getFactory()};
      QList<int> list {factory->getBuildList(model->getPointer(getSelection())->getType())};

      // iterate through build list and add action for each type
      for (auto i = list.constBegin(); i != list.constEnd() ;++i)
      {
         _addActions.append(new QAction(factory->getName(*i),this));
         _addActions.back()->setData(*i);
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
   _addMenu->setDisabled(!model);
}






//@@
void BlockView::updateMenu()
{
   // clear menu and build from list of add actions
   _addMenu->clear();
   for (auto i = _addActions.constBegin(); i != _addActions.constEnd() ;++i)
   {
      _addMenu->addAction(*i);
   }
}







//@@
QModelIndex BlockView::getSelection() const
{
   // if selection model exists and it is not empty get first index it has
   QModelIndex ret;
   const QItemSelectionModel* model {_treeView->selectionModel()};
   if ( model && !model->selection().isEmpty() )
   {
      ret = model->selection().first().indexes().first();
   }

   // return selected index or invalid index for root
   return ret;
}
