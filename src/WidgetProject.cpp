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


QAction* Project::clearAction(
)
{
    if (!_clearAction)
    {
        _clearAction = new QAction(QIcon::fromTheme("edit-clear"),tr("Clear"),this);
        _clearAction->setStatusTip(tr("Clear all selected and current blocks."));
        connect(_clearAction,&QAction::triggered,this,&Project::clear);
    }
    return _clearAction;
}


QAction* Project::copyAction(
)
{
    if (!_copyAction)
    {
        _copyAction = new QAction(QIcon::fromTheme("edit-copy"),tr("Copy"),this);
        _copyAction->setStatusTip(tr("Copy the selected block or blocks."));
        _copyAction->setShortcut(QKeySequence::Copy);
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
        _cutAction->setShortcut(QKeySequence::Cut);
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
        _moveDownAction->setShortcut(Qt::CTRL|Qt::Key_Down);
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
        _moveUpAction->setShortcut(Qt::CTRL|Qt::Key_Up);
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
        _pasteAction->setShortcut(QKeySequence::Paste);
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
        _redoAction->setShortcut(QKeySequence::Redo);
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
        _removeAction->setShortcut(QKeySequence::Delete);
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
    treeView()->setModel(model);
    if (_model)
    {
        connect(_model,&QObject::destroyed,this,&Project::onModelDestroyed);
        connect(
            treeView()->selectionModel()
            ,&QItemSelectionModel::currentChanged
            ,this
            ,&Project::onCurrentIndexChanged
        );
        connect(
            treeView()->selectionModel()
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
        _undoAction->setShortcut(QKeySequence::Undo);
        connect(_undoAction,&QAction::triggered,this,&Project::undo);
    }
    return _undoAction;
}


void Project::add(
    int index
)
{
    if (_model)
    {
        auto parent = treeView()->selectionModel()->currentIndex();
        _model->insert(index,0,parent);
        if (!parent.isValid())
        {
            treeView()->setCurrentIndex(parent);
        }
    }
}


void Project::clear(
)
{
    if (_model)
    {
        treeView()->setCurrentIndex(QModelIndex());
    }
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
    move(1);
}


void Project::moveUp(
)
{
    move(-1);
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
    auto itemSelectionModel = treeView()->selectionModel();
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
    auto itemSelectionModel = treeView()->selectionModel();
    G_ASSERT(itemSelectionModel);
    if (
        _model
        && itemSelectionModel->hasSelection()
    )
    {
        _model->remove(itemSelectionModel->selectedIndexes());
    }
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


void Project::move(
    int delta
)
{
    if (_model)
    {
        auto index = treeView()->selectionModel()->currentIndex();
        if (index.isValid())
        {
            auto parent = index.parent();
            auto row = index.row();
            if (_model->move(parent,row,row+delta))
            {
                auto newIndex = _model->index(row+delta,0,parent);
                treeView()->selectionModel()->setCurrentIndex(
                    newIndex
                    ,QItemSelectionModel::SelectCurrent
                );
                if (newIndex == treeView()->currentIndex())
                {
                    updateActions(newIndex);
                }
                else
                {
                    treeView()->setCurrentIndex(newIndex);
                }
            }
        }
    }
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
        _splitter->addWidget(treeView());
        _splitter->addWidget(blankBlockWidget());
    }
    return _splitter;
}


QTreeView* Project::treeView(
)
{
    if (!_treeView)
    {
        _treeView = new QTreeView(this);
        _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    return _treeView;
}


void Project::updateActions(
    const QModelIndex& index
)
{
    if (_model)
    {
        auto itemSelectionModel = treeView()->selectionModel();
        G_ASSERT(itemSelectionModel);
        copyAction()->setDisabled(!itemSelectionModel->hasSelection());
        cutAction()->setDisabled(!itemSelectionModel->hasSelection());
        pasteAction()->setDisabled(!_model->canPaste(index));
        redoAction()->setDisabled(!_model->canRedo());
        removeAction()->setDisabled(!index.isValid());
        undoAction()->setDisabled(!_model->canUndo());
        moveDownAction()->setDisabled(!_model->canMove(index.parent(),index.row(),index.row()+1));
        moveUpAction()->setDisabled(!_model->canMove(index.parent(),index.row(),index.row()-1));
    }
    else
    {
        copyAction()->setDisabled(true);
        cutAction()->setDisabled(true);
        moveDownAction()->setDisabled(true);
        moveUpAction()->setDisabled(true);
        pasteAction()->setDisabled(true);
        redoAction()->setDisabled(true);
        removeAction()->setDisabled(true);
        undoAction()->setDisabled(true);
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
            const auto& allowed = language->blockMeta(blockIndex)->allowList();
            for (int i = 0;i < language->size();i++)
            {
                if (allowed.contains(i))
                {
                    auto meta = language->blockMeta(i);
                    auto action = new QAction(meta->displayIcon(),meta->label(),addMenu());
                    action->setStatusTip(
                        tr("Add %1 block into the current block.").arg(meta->label())
                    );
                    connect(action,&QAction::triggered,this,[this,i](){ add(i); });
                    addMenu()->addAction(action);
                }
            }
        }
        _addActionBlockIndex = blockIndex;
    }
    addMenu()->setDisabled(addMenu()->isEmpty());
}
}
