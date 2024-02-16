#include "WidgetProject.h"
#include <QtWidgets>
#include "BlockAbstract.h"
#include "Exceptions.h"
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "ModelProject.h"
#include "WidgetBlockAbstract.h"
namespace Widget {


Project::Project(
    QWidget* parent
):
    QWidget(parent)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(splitter());
    setLayout(layout);
    updateAddGlobalActions();
    updateActions(QModelIndex());
}


QMenu* Project::addGlobalMenu(
)
{
    if (!_addGlobalMenu)
    {
        _addGlobalMenu = new QMenu(tr("Add to Global"),this);
    }
    return _addGlobalMenu;
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


QAction* Project::deselectAction(
)
{
    if (!_deselectAction)
    {
        _deselectAction = new QAction(QIcon::fromTheme("edit-select-none"),tr("Deselect"),this);
        _deselectAction->setStatusTip(tr("Clear all selected blocks."));
        _deselectAction->setShortcut(QKeySequence::Deselect);
        connect(_deselectAction,&QAction::triggered,this,&Project::deselect);
    }
    return _deselectAction;
}


QAction* Project::moveDownAction(
)
{
    if (!_moveDownAction)
    {
        _moveDownAction = new QAction(QIcon::fromTheme("go-down"),tr("Move Down"),this);
        _moveDownAction->setStatusTip(tr("Move the current block down by one."));
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
        _moveUpAction->setStatusTip(tr("Move the current block up by one."));
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
        _pasteAction->setStatusTip(tr("Paste copied blocks into the current block."));
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
    updateAddGlobalActions();
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
        auto parent = treeView()->currentIndex();
        _model->insert(index,0,parent);
        if (!parent.isValid())
        {
            treeView()->setCurrentIndex(parent);
        }
    }
}


void Project::addGlobal(
    int index
)
{
    if (_model)
    {
        auto parent = QModelIndex();
        _model->insert(index,0,parent);
        if (!parent.isValid())
        {
            treeView()->setCurrentIndex(parent);
        }
    }
}


void Project::deselect(
)
{
    if (_model)
    {
        auto tv = treeView();
        tv->selectionModel()->setCurrentIndex(tv->currentIndex(),QItemSelectionModel::Clear);
    }
}


void Project::copy(
)
{
    auto selectionModel = treeView()->selectionModel();
    if (
        _model
        && selectionModel->hasSelection()
    )
    {
        _model->copy(selectionModel->selectedIndexes());
    }
}


void Project::cut(
)
{
    auto selectionModel = treeView()->selectionModel();
    if (
        _model
        && selectionModel->hasSelection()
        )
    {
        if (_model->cut(selectionModel->selectedIndexes()) > 0)
        {
            updateActions(treeView()->currentIndex());
        }
    }
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
    if (current.isValid())
    {
        G_ASSERT(_model);
        setBlockWidget(_model->constBlock(current)->createWidget());
    }
    else
    {
        setBlockWidget(new QWidget);
    }
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
    updateActions(treeView()->selectionModel()->currentIndex());
}


void Project::paste(
)
{
    if (_model)
    {
        if (_model->paste(treeView()->currentIndex(),0) > 0)
        {
            updateActions(treeView()->currentIndex());
        }
    }
}


void Project::redo(
)
{
    if (_model)
    {
        if (_model->redo())
        {
            updateActions(treeView()->currentIndex());
        }
    }
}


void Project::remove(
)
{
    auto selectionModel = treeView()->selectionModel();
    if (
        _model
        && selectionModel->hasSelection()
    )
    {
        _model->remove(selectionModel->selectedIndexes());
    }
}


void Project::undo(
)
{
    if (_model)
    {
        if (_model->undo())
        {
            updateActions(treeView()->currentIndex());
        }
    }
}


QPushButton* Project::blockApplyButton(
)
{
    if (!_blockApplyButton)
    {
        _blockApplyButton = new QPushButton(tr("Apply"));
        _blockApplyButton->setEnabled(false);
    }
    return _blockApplyButton;
}


QHBoxLayout* Project::blockButtonsLayout(
)
{
    if (!_blockButtonsLayout)
    {
        _blockButtonsLayout = new QHBoxLayout;
        _blockButtonsLayout->addWidget(blockApplyButton());
        _blockButtonsLayout->addStretch();
    }
    return _blockButtonsLayout;
}


QGroupBox* Project::blockGroupBox(
)
{
    if (!_blockGroupBox)
    {
        _blockGroupBox = new QGroupBox;
        auto layout = new QVBoxLayout;
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(blockScrollArea());
        _blockGroupBox->setLayout(layout);
    }
    return _blockGroupBox;
}


QScrollArea* Project::blockScrollArea(
)
{
    if (!_blockScrollArea)
    {
        _blockScrollArea = new QScrollArea;
        _blockScrollArea->setWidget(new QWidget);
    }
    return _blockScrollArea;
}


QWidget* Project::blockView(
)
{
    if (!_blockView)
    {
        _blockView = new QWidget;
        auto layout = new QVBoxLayout;
        layout->setContentsMargins(2,0,0,0);
        layout->addWidget(blockGroupBox());
        layout->addLayout(blockButtonsLayout());
        _blockView->setLayout(layout);
    }
    return _blockView;
}


void Project::move(
    int delta
)
{
    if (_model)
    {
        auto index = treeView()->currentIndex();
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
    blockScrollArea()->setWidget(widget);
    if (auto block = qobject_cast<Block::Abstract*>(widget))
    {
        auto button = blockApplyButton();
        connect(button,&QPushButton::clicked,block,&Block::Abstract::apply);
        connect(block,&Block::Abstract::modifiedChanged,button,&QWidget::setEnabled);
        button->setEnabled(block->modified());
    }
}


QSplitter* Project::splitter(
)
{
    if (!_splitter)
    {
        _splitter = new QSplitter;
        _splitter->setChildrenCollapsible(false);
        _splitter->addWidget(treeView());
        _splitter->addWidget(blockView());
    }
    return _splitter;
}


QTreeView* Project::treeView(
)
{
    if (!_treeView)
    {
        _treeView = new QTreeView;
        _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _treeView->setFont(QFont());
    }
    return _treeView;
}


void Project::updateActions(
    const QModelIndex& index
)
{
    if (_model)
    {
        auto selectionModel = treeView()->selectionModel();
        copyAction()->setEnabled(selectionModel->hasSelection());
        cutAction()->setEnabled(selectionModel->hasSelection());
        pasteAction()->setEnabled(_model->canPaste(index));
        redoAction()->setEnabled(_model->canRedo());
        removeAction()->setEnabled(index.isValid());
        undoAction()->setEnabled(_model->canUndo());
        moveDownAction()->setEnabled(_model->canMove(index.parent(),index.row(),index.row()+1));
        moveUpAction()->setEnabled(_model->canMove(index.parent(),index.row(),index.row()-1));
        deselectAction()->setEnabled(treeView()->selectionModel()->hasSelection());
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
        deselectAction()->setDisabled(true);
    }
    updateAddActions(index);
}


void Project::updateAddActions(
    const QModelIndex& index
)
{
    int blockIndex = -1;
    if (
        _model
        && index.isValid()
    )
    {
        blockIndex = _model->blockIndex(index);
    }
    if (_addActionBlockIndex != blockIndex)
    {
        addMenu()->clear();
        if (
            _model
            && blockIndex != -1
        )
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
                        tr("Add new %1 block into the current block.").arg(meta->label())
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


void Project::updateAddGlobalActions(
)
{
    addGlobalMenu()->clear();
    if (_model)
    {
        auto language = _model->language();
        const auto& allowed = language->blockMeta(_model->blockIndex(QModelIndex()))->allowList();
        for (int i = 0;i < language->size();i++)
        {
            if (allowed.contains(i))
            {
                auto meta = language->blockMeta(i);
                auto action = new QAction(meta->displayIcon(),meta->label(),addGlobalMenu());
                action->setStatusTip(
                    tr("Add new %1 block into the global(root) block.").arg(meta->label())
                );
                connect(action,&QAction::triggered,this,[this,i](){ addGlobal(i); });
                addGlobalMenu()->addAction(action);
            }
        }
    }
    addGlobalMenu()->setDisabled(addGlobalMenu()->isEmpty());
}
}
