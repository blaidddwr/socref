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
   _layout = new QVBoxLayout;
   _view = new QTreeView;
   _layout->addWidget(_view);
   setLayout(_layout);
}






//@@
void BlockView::setModel(BlockModel* model)
{
   _view->setModel(model);
   connect(_view->selectionModel(),&QItemSelectionModel::currentChanged,this
           ,&BlockView::currentSelectionChanged);
   currentSelectionChanged(QModelIndex(),QModelIndex());
   emit selectionChanged();
}






//@@
void BlockView::populateAddMenu(QMenu* menu) const
{
   menu->clear();
   for (auto i = _addActions.constBegin(); i != _addActions.constEnd() ;++i)
   {
      menu->addAction(*i);
   }
}






//@@
bool BlockView::hasSelection() const
{
   QItemSelectionModel* model {_view->selectionModel()};
   if ( model )
   {
      return model->currentIndex().isValid();
   }
   return false;
}






//@@
bool BlockView::canPaste() const
{
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
   if ( model && _copy )
   {
      const AbstractBlockFactory* factory {model->getFactory()};
      if ( factory && factory->getType() == _copy->getFactory().getType() )
      {
         AbstractBlock* selection {model->getPointer(_view->selectionModel()->currentIndex())};
         if ( selection && factory->getBuildList(selection->getType()).contains(_copy->getType()) )
         {
            return true;
         }
      }
   }
   return false;
}






//@@
void BlockView::addTriggered()
{
   QAction* from {qobject_cast<QAction*>(sender())};
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
   const AbstractBlockFactory* factory {model->getFactory()};
   model->insertRow(-1,_view->selectionModel()->currentIndex()
                    ,factory->makeBlock(from->data().toInt()));
}






//@@
void BlockView::removeTriggered()
{
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
void BlockView::currentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);
   qDeleteAll(_addActions);
   _addActions.clear();
   BlockModel* model {qobject_cast<BlockModel*>(_view->model())};
   const AbstractBlockFactory* factory {model->getFactory()};
   QList<int> list {factory->getBuildList(model->getPointer(current)->getType())};
   for (auto i = list.constBegin(); i != list.constEnd() ;++i)
   {
      _addActions.append(new QAction(factory->getName(*i),this));
      _addActions.back()->setData(*i);
      connect(_addActions.back(),&QAction::triggered,this,&BlockView::addTriggered);
   }
   emit selectionChanged();
}
