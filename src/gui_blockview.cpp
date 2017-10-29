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
void BlockView::populateAddMenu(QMenu* menu) const
{
   // clear menu and build from list of add actions
   menu->clear();
   for (auto i = _addActions.constBegin(); i != _addActions.constEnd() ;++i)
   {
      menu->addAction(*i);
   }
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
   // clear out previous actions list
   qDeleteAll(_addActions);
   _addActions.clear();

   // get block model, factory, index, block pointer, and build list of selected index
   BlockModel* model {qobject_cast<BlockModel*>(_treeView->model())};
   const AbstractBlockFactory* factory {model->getFactory()};
   QModelIndex index {getSelection()};
   AbstractBlock* pointer {model->getPointer(index)};
   QList<int> list {factory->getBuildList(pointer->getType())};

   // if block view already existed delete it
   if ( _view )
   {
      delete _view;
      _view = nullptr;
   }

   // if index is valid create new view and add it to layout
   if ( index.isValid() )
   {
      _view = factory->makeView(pointer->getType(),pointer);
      _area->setWidget(_view);
   }

   // iterate through build list and create add action for each type index can have as children
   for (auto i = list.constBegin(); i != list.constEnd() ;++i)
   {
      _addActions.append(new QAction(factory->getName(*i),this));
      _addActions.back()->setData(*i);
      connect(_addActions.back(),&QAction::triggered,this,&BlockView::addTriggered);
   }

   // emit selection changed signal
   emit selectionChanged();
}






//@@
void BlockView::modelDestroyed()
{
   // model was destroyed to clear any existing view
   delete _view;
   _view = nullptr;
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
