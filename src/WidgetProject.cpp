#include "WidgetProject.h"
#include <QtWidgets>
#include "Exceptions.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
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
    updateActions(QModelIndex());
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
        connect(
            listView()->selectionModel()
            ,&QItemSelectionModel::currentChanged
            ,this
            ,&Project::onCurrentIndexChanged
        );
        connect(
            listView()->selectionModel()
            ,&QItemSelectionModel::selectionChanged
            ,this
            ,&Project::onSelectionChanged
        );
    }
    updateActions(QModelIndex());
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


void Project::onCurrentIndexChanged(
    const QModelIndex& current
    ,const QModelIndex& previous
)
{
    Q_UNUSED(previous);
    updateActions(current);
}


void Project::onModelDestroyed(
    QObject* object
)
{
    if (_model == object)
    {
        _model = nullptr;
        updateActions(QModelIndex());
    }
}


void Project::onSelectionChanged(
)
{
    auto itemSelectionModel = listView()->selectionModel();
    G_ASSERT(itemSelectionModel);
    updateActions(itemSelectionModel->currentIndex());
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
    const QModelIndex& index
)
{
    if (_model)
    {
        auto itemSelectionModel = listView()->selectionModel();
        G_ASSERT(itemSelectionModel);
        _copyAction->setDisabled(!itemSelectionModel->hasSelection());
        _cutAction->setDisabled(!itemSelectionModel->hasSelection());
        _pasteAction->setDisabled(!_model->canPaste(index));
        _redoAction->setDisabled(!_model->canRedo());
        _removeAction->setDisabled(!index.isValid());
        _undoAction->setDisabled(!_model->canUndo());
        _moveDownAction->setDisabled(!_model->canMoveDown(index));
        _moveUpAction->setDisabled(!_model->canMoveUp(index));
    }
    else
    {
        _copyAction->setDisabled(true);
        _cutAction->setDisabled(true);
        _moveDownAction->setDisabled(true);
        _moveUpAction->setDisabled(true);
        _pasteAction->setDisabled(true);
        _redoAction->setDisabled(true);
        _removeAction->setDisabled(true);
        _undoAction->setDisabled(true);
    }
    updateAddActions(index);
}


void Project::updateAddActions(
    const QModelIndex& index
)
{
    int blockIndex = -1;
    if (_model)
    {
        blockIndex = _model->blockIndex(index);
    }
    if (_addActionBlockIndex != blockIndex)
    {
        addMenu()->clear();
        if (_model)
        {
            auto language = _model->language();
            for (auto index: language->blockMeta(blockIndex)->allowList())
            {
                auto meta = language->blockMeta(index);
                auto action = new QAction(meta->displayIcon(),meta->label(),addMenu());
                addMenu()->addAction(action);
            }
        }
        _addActionBlockIndex = blockIndex;
    }
    addMenu()->setDisabled(addMenu()->isEmpty());
}
}
