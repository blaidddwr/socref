#include "WidgetProject.h"
#include <QtWidgets>
#include "Exceptions.h"
#include "ModelProject.h"
namespace Widget {


Project::Project(
    QWidget* parent
):
    QWidget(parent)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(splitter());
    setLayout(layout);
}


QMenu* Project::addMenu(
)
{
    if (!_addMenu)
    {
        _addMenu = new QMenu(tr("Add"),this);
    }
    return _addMenu;
}


QAction* Project::copyAction(
)
{
    if (!_copyAction)
    {
        _copyAction = new QAction(QIcon::fromTheme("edit-copy"),tr("Copy"),this);
        _copyAction->setStatusTip(tr("Copy the selected block or blocks."));
        connect(_copyAction,&QAction::triggered,this,&Project::copy);
    }
    return _copyAction;
}


QAction* Project::cutAction(
)
{
    if (!_cutAction)
    {
        _cutAction = new QAction(QIcon::fromTheme("edit-cut"),tr("Cut"),this);
        _cutAction->setStatusTip(tr("Cut the selected block or blocks."));
        connect(_cutAction,&QAction::triggered,this,&Project::cut);
    }
    return _cutAction;
}


QAction* Project::moveDownAction(
)
{
    if (!_moveDownAction)
    {
        _moveDownAction = new QAction(QIcon::fromTheme("go-down"),tr("Move Down"),this);
        _moveDownAction->setStatusTip(tr("Move the currently selected block down by one."));
        connect(_moveDownAction,&QAction::triggered,this,&Project::moveDown);
    }
    return _moveDownAction;
}


QAction* Project::moveUpAction(
)
{
    if (!_moveUpAction)
    {
        _moveUpAction = new QAction(QIcon::fromTheme("go-up"),tr("Move Up"),this);
        _moveUpAction->setStatusTip(tr("Move the currently selected block up by one."));
        connect(_moveUpAction,&QAction::triggered,this,&Project::moveUp);
    }
    return _moveUpAction;
}


QAction* Project::pasteAction(
)
{
    if (!_pasteAction)
    {
        _pasteAction = new QAction(QIcon::fromTheme("edit-paste"),tr("Paste"),this);
        _pasteAction->setStatusTip(tr("Paste copied blocks into the currently selected block."));
        connect(_pasteAction,&QAction::triggered,this,&Project::paste);
    }
    return _pasteAction;
}


QAction* Project::redoAction(
)
{
    if (!_redoAction)
    {
        _redoAction = new QAction(QIcon::fromTheme("edit-redo"),tr("Redo"),this);
        _redoAction->setStatusTip(tr("Redo the previously undone command."));
        connect(_redoAction,&QAction::triggered,this,&Project::redo);
    }
    return _redoAction;
}


QAction* Project::removeAction(
)
{
    if (!_removeAction)
    {
        _removeAction = new QAction(QIcon::fromTheme("list-remove"),tr("Remove"),this);
        _removeAction->setStatusTip(tr("Remove the selected block or blocks."));
        connect(_removeAction,&QAction::triggered,this,&Project::remove);
    }
    return _removeAction;
}


void Project::setModel(
    Model::Project* model
)
{
    if (_model)
    {
        disconnect(_model,&QObject::destroyed,this,&Project::onModelDestroyed);
    }
    _model = model;
    listView()->setModel(model);
    if (_model)
    {
        connect(_model,&QObject::destroyed,this,&Project::onModelDestroyed);
    }
    updateActions();
}


QAction* Project::undoAction(
)
{
    if (!_undoAction)
    {
        _undoAction = new QAction(QIcon::fromTheme("edit-undo"),tr("Undo"),this);
        _undoAction->setStatusTip(tr("Undo the previously done command."));
        connect(_undoAction,&QAction::triggered,this,&Project::undo);
    }
    return _undoAction;
}


void Project::add(
    int index
)
{
    Q_UNUSED(index);
    //TODO
}


void Project::copy(
)
{
    //TODO
}


void Project::cut(
)
{
    //TODO
}


void Project::moveDown(
)
{
    //TODO
}


void Project::moveUp(
)
{
    //TODO
}


void Project::onModelDestroyed(
    QObject* object
)
{
    if (_model == object)
    {
        _model = nullptr;
        updateActions();
    }
}


void Project::paste(
)
{
    //TODO
}


void Project::redo(
)
{
    //TODO
}


void Project::remove(
)
{
    //TODO
}


void Project::undo(
)
{
    //TODO
}


QWidget* Project::blankBlockWidget(
)
{
    if (!_blankBlockWidget)
    {
        _blankBlockWidget = new QWidget(this);
    }
    return _blankBlockWidget;
}


QListView* Project::listView(
)
{
    if (!_listView)
    {
        _listView = new QListView(this);
    }
    return _listView;
}


void Project::setBlockWidget(
    QWidget* widget
)
{
    G_ASSERT(widget);
    splitter()->replaceWidget(1,widget);
}


QSplitter* Project::splitter(
)
{
    if (!_splitter)
    {
        _splitter = new QSplitter(this);
        _splitter->setChildrenCollapsible(false);
        _splitter->addWidget(listView());
        _splitter->addWidget(blankBlockWidget());
    }
    return _splitter;
}


void Project::updateActions(
)
{
    //TODO
}


void Project::updateAddActions(
)
{
    //TODO
}
}
