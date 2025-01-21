#include "WidgetStringList.h"
#include <QtWidgets>
namespace Widget {


StringList::StringList(
    QWidget* parent
):
    QListView(parent)
{
    setModel(model());
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(selectionModel(),&QItemSelectionModel::currentChanged,this,&StringList::updateActions);
    connect(
        this
        ,&QWidget::customContextMenuRequested
        ,this
        ,[this](const QPoint& pos){ contextMenu()->exec(mapToGlobal(pos)); }
    );
    updateActions();
}


QStringListModel* StringList::model(
)
{
    if (!_model)
    {
        auto redirect = [this](){ emit stringListChanged(); };
        _model = new QStringListModel(this);
        connect(_model,&QAbstractItemModel::modelReset,this,redirect);
        connect(_model,&QAbstractItemModel::dataChanged,this,redirect);
        connect(_model,&QAbstractItemModel::rowsInserted,this,redirect);
        connect(_model,&QAbstractItemModel::rowsRemoved,this,redirect);
        connect(_model,&QAbstractItemModel::rowsMoved,this,redirect);
    }
    return _model;
}


void StringList::add(
)
{
    model()->insertRows(0,1,QModelIndex());
    updateActions();
}


void StringList::moveDown(
)
{
    auto index = currentIndex();
    if (canMoveDown(index))
    {
        model()->moveRow(index.parent(),index.row(),index.parent(),index.row()+2);
        updateActions();
    }
}


void StringList::moveUp(
)
{
    auto index = currentIndex();
    if (canMoveUp(index))
    {
        model()->moveRow(index.parent(),index.row(),index.parent(),index.row()-1);
        updateActions();
    }
}


void StringList::remove(
)
{
    auto index = currentIndex();
    if (index.isValid())
    {
        model()->removeRows(index.row(),1,index.parent());
        updateActions();
    }
}


void StringList::updateActions(
)
{
    auto index = currentIndex();
    removeAction()->setEnabled(index.isValid());
    moveDownAction()->setEnabled(canMoveDown(index));
    moveUpAction()->setEnabled(canMoveUp(index));
}


QAction* StringList::addAction(
)
{
    if (!_addAction)
    {
        _addAction = new QAction(tr("Add"),this);
        connect(_addAction,&QAction::triggered,this,&StringList::add);
    }
    return _addAction;
}


bool StringList::canMoveDown(
    const QModelIndex& index
)
{
    if (!index.isValid())
    {
        return false;
    }
    return index.row() < (model()->rowCount(index.parent())-1);
}


bool StringList::canMoveUp(
    const QModelIndex& index
)
{
    if (!index.isValid())
    {
        return false;
    }
    return index.row() > 0;
}


QMenu* StringList::contextMenu(
)
{
    if (!_contextMenu)
    {
        _contextMenu = new QMenu(this);
        _contextMenu->addAction(addAction());
        _contextMenu->addAction(removeAction());
        _contextMenu->addAction(moveUpAction());
        _contextMenu->addAction(moveDownAction());
    }
    return _contextMenu;
}


QAction* StringList::moveDownAction(
)
{
    if (!_moveDownAction)
    {
        _moveDownAction = new QAction(tr("Move Down"),this);
        connect(_moveDownAction,&QAction::triggered,this,&StringList::moveDown);
    }
    return _moveDownAction;
}


QAction* StringList::moveUpAction(
)
{
    if (!_moveUpAction)
    {
        _moveUpAction = new QAction(tr("Move Up"),this);
        connect(_moveUpAction,&QAction::triggered,this,&StringList::moveUp);
    }
    return _moveUpAction;
}


QAction* StringList::removeAction(
)
{
    if (!_removeAction)
    {
        _removeAction = new QAction(tr("Remove"),this);
        connect(_removeAction,&QAction::triggered,this,&StringList::remove);
    }
    return _removeAction;
}
}
