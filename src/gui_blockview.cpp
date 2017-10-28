#include <QVBoxLayout>
#include <QTreeView>
#include <QMenu>

#include "gui_blockview.h"
#include "blockmodel.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"



using namespace Gui;






//@@
BlockView::BlockView(QWidget* parent):
   QWidget(parent)
{
   // create tree view
   _view = new QTreeView;
   _view->setHeaderHidden(true);

   // create layout and set as widget's layout
   _layout = new QVBoxLayout;
   _layout->addWidget(_view);
   setLayout(_layout);
}






//@@
void BlockView::setModel(BlockModel* model)
{
   // set tree view model, connect selection changed signal, and call selection changed
   _view->setModel(model);
   connect(_view->selectionModel(),&QItemSelectionModel::selectionChanged,this
           ,&BlockView::selectionChanged);
   selectionChanged();
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
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
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
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
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
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
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

   // get block model, factory, and build list of selected index
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
   const AbstractBlockFactory* factory {model->getFactory()};
   QList<int> list {factory->getBuildList(model->getPointer(getSelection())->getType())};

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
QModelIndex BlockView::getSelection() const
{
   // if selection model exists and it is not empty get first index it has
   QModelIndex ret;
   const QItemSelectionModel* model {_view->selectionModel()};
   if ( model && !model->selection().isEmpty() )
   {
      ret = model->selection().first().indexes().first();
   }

   // return selected index or invalid index for root
   return ret;
}
