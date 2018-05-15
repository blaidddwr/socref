#include "gui_blockview.h"
#include <QTreeView>
#include <QScrollArea>
#include <QMenu>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <exception.h>
#include "gui_abstractedit.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "blockmodel.h"
#include "application.h"



using namespace std;
using namespace Gui;
//



/*!
 * The state key used by this object to save/restore qt settings of its state. 
 */
const char* BlockView::_stateKey {"gui.blockview.state"};
/*!
 * Pointer to a copied block that any instance of this class can use to paste into 
 * their current block model if possible. 
 */
AbstractBlock* BlockView::_copy;






/*!
 * Constructs a new block view with no model and the optional given parent. 
 *
 * @param parent  
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this object's GUI, restore the state of this qt splitter widget, and 
 *    update this object's context actions. 
 */
BlockView::BlockView(QWidget* parent):
   QSplitter(parent)
{
   // 1
   setupGui();
   restoreSettings();
   updateContextMenu();
}






/*!
 * Saves the state of this widget. 
 */
BlockView::~BlockView()
{
   saveSettings();
}






/*!
 * Returns a pointer to this block view object's internal context menu. 
 *
 * @return Pointer to this block view object's internal context menu. 
 */
QMenu* BlockView::contextMenu() const
{
   return _contextMenu;
}






/*!
 * Tests whether the currently copied block can be pasted into the currently 
 * selected block. Returns true if it can be pasted or false otherwise. 
 *
 * @return True if the currently copied block can be pasted into the currently 
 *         selected block or false if it cannot. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object has a model, a copy of a block, it's project factory type is 
 *    equal to the copied block's project type, and the copied block's type is 
 *    contained within the selected block's build list then return true, else 
 *    return false. 
 */
bool BlockView::canPaste() const
{
   // 1
   if ( _model && _copy && _factory->type() == _copy->factory().type() )
   {
      AbstractBlock* index {_model->pointer(_current)};
      if ( index && index->buildList().contains(_copy->type()) ) return true;
   }
   return false;
}






/*!
 * Sets a new block model for this block view, releasing but not deleting any 
 * previous model set to it. This block view does not take ownership of the given 
 * model. 
 *
 * @param model Pointer to the new block model this block view will display for 
 *              editing. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's tree view model to the given block model. Save the given 
 *    model pointer, a pointer of the given model's block factory, and the new 
 *    selection model of this object's tree view to this object. Then reset this 
 *    object's currently selected index to none. 
 *
 * 2. Connect all selection model and model signals to this object. 
 */
void BlockView::setModel(BlockModel* model)
{
   // 1
   _treeView->setModel(model);
   _model = model;
   _factory = model->factory();
   _selectionModel = _treeView->selectionModel();
   _current = QModelIndex();

   // 2
   connect(_selectionModel
           ,&QItemSelectionModel::selectionChanged
           ,this
           ,&BlockView::selectionModelChanged);
   connect(_model,&BlockModel::dataChanged,this,&BlockView::modelDataChanged);
   connect(_model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);
}






/*!
 * Called when an add action is triggered. If this object has a model then this 
 * inserts a new block of the given type in the currently selected block. 
 *
 * @param type The block type created and inserted into this object's model if it 
 *             has one. 
 */
void BlockView::addTriggered(int type)
{
   if ( _model ) _model->insert(_current,_factory->makeBlock(type));
}






/*!
 * Called when the remove action is triggered. If this object has a valid current 
 * index then it is removed from its model. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's currently selected index is not valid then return. 
 *
 * 2. Remove this object's currently selected index from its model and then update 
 *    the currently selected index. 
 */
void BlockView::removeTriggered()
{
   // 1
   if ( !_current.isValid() ) return;

   // 2
   _model->remove(_current);
   updateIndex();
}






/*!
 * Called when the edit action is triggered. If this object has a valid current 
 * index then this will open an edit dialog of the index's block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current index is not valid then return. 
 *
 * 2. Create an abstract block pointer _pointer_, set it to this object's current 
 *    index block, and then create a new edit dialog _edit_ from _pointer_. If the 
 *    returned edit dialog is null then return. 
 *
 * 3. Initialize _edit_, set its icon to the block icon of _pointer_, set its title 
 *    using the block type name of _pointer_, modal execute the edit dialog, and 
 *    then update this object's current detailed view of the block that was just 
 *    edited. 
 */
void BlockView::editTriggered()
{
   // 1
   if ( !_current.isValid() ) return;

   // 2
   AbstractBlock* pointer {_model->pointer(_current)};
   unique_ptr<AbstractEdit> edit {pointer->makeEdit()};
   if ( !edit ) return;

   // 3
   edit->initialize();
   edit->setWindowIcon(pointer->icon());
   edit->setWindowTitle(tr("Edit %1").arg(_factory->name(pointer->type())));
   edit->exec();
   updateView();
}






/*!
 * Called when the cut action is triggered. If this object has a valid current 
 * index then this will cut it from this object's model. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current index is valid then delete any current block copy, 
 *    cut the current index's block from the model and set it as this object's new 
 *    copy, update the current index, and update this object's context menu. 
 */
void BlockView::cutTriggered()
{
   // 1
   if ( _current.isValid() )
   {
      delete _copy;
      _copy = _model->cut(_current).release();
      updateIndex();
      updateContextMenu();
   }
}






/*!
 * Called when the copy action is triggered. If this object has a valid current 
 * index then this will copy it from this object's model. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's current index is valid then delete any current block copy, 
 *    copy the current index's block from the model and set it as this object's new 
 *    copy, and update this object's actions. 
 */
void BlockView::copyTriggered()
{
   // 1
   if ( _current.isValid() )
   {
      delete _copy;
      _copy = _model->copy(_current).release();
      updateActions();
   }
}






/*!
 * Called when the paste action is triggered. If this object can paste its copied 
 * block into its model then it does so. 
 */
void BlockView::pasteTriggered()
{
   if ( canPaste() ) _model->insert(_current,_copy->makeCopy());
}






/*!
 * Called when the move up action is triggered. If this object has a valid current 
 * index then this will move it up by one. 
 */
void BlockView::moveUpTriggered()
{
   if ( _current.isValid() ) _current = _model->moveUp(_current);
}






/*!
 * Called when the move down action is triggered. If this object has a valid 
 * current index then this will move it down by one. 
 */
void BlockView::moveDownTriggered()
{
   if ( _current.isValid() ) _current = _model->moveDown(_current);
}






/*!
 * Called when this object's selection model has changed. This determines its 
 * object's new current index, updating its view and context menu afterwords. 
 *
 * @param selected List of indexes that have been selected by the user. 
 *
 * @param deselected Unused list of indexes that have been deselected by the user. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Update this object's current index based off its model's new selection, using 
 *    the first index as its new current index. If there are no selected indexes 
 *    then set the current index as invalid. 
 *
 * 2. Update this object's view and context menu. 
 */
void BlockView::selectionModelChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   Q_UNUSED(deselected)

   // 1
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();

   // 2
   updateView();
   updateContextMenu();
}






/*!
 * Called when this object's block model is about to be deleted. This clears its 
 * object's view to a null state and all internal pointers. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Clear this object's title icon and text. 
 *
 * 2. Set this object's model, factory, and selection models to null. Set this 
 *    object's current index to an invalid one. 
 *
 * 3. Update this object's detailed view and context menu. 
 */
void BlockView::modelDestroyed()
{
   // 1
   _titleIcon->clear();
   _titleText->clear();

   // 2
   _model = nullptr;
   _factory = nullptr;
   _selectionModel = nullptr;
   _current = QModelIndex();

   // 3
   updateView();
   updateContextMenu();
}






/*!
 * Called when data in this object's block model has changed. If this object's 
 * current index is the one updated then this updates its object's view depending 
 * on the given roles. 
 *
 * @param topLeft The top left index of the rectangle of indexes whose data has 
 *                changed. 
 *
 * @param bottomRight The bottom right index of the rectangle of indexes whose data 
 *                    has changed. This is not used in this view because only one 
 *                    index can change at a time. 
 *
 * @param roles The list of data roles that has changed. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given top left index is equal to this object's current index and the 
 *    current index is not valid then update this object's title if needed, 
 *    detailed view if needed, and context menu. 
 */
void BlockView::modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
   Q_UNUSED(bottomRight)

   // 1
   if ( topLeft == _current && _current.isValid() )
   {
      if ( roles.contains(BlockModel::Name) ) updateTitle(_model->pointer(_current));
      if ( roles.contains(BlockModel::Body) ) updateView();
      updateContextMenu();
   }
}






/*!
 * Called when this object's tree view is requesting a context menu because of a 
 * right mouse click. This updates its object's current index based off the mouse 
 * click position and executes its object's context menu. 
 *
 * @param position The relative position within this object's tree view where the 
 *                 mouse event occurred. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Update this object's current index to the one located where the mouse event 
 *    occurred, updating the view and context menu afterwords. 
 *
 * 2. Modal execute this object's context menu. 
 */
void BlockView::contextMenuRequested(const QPoint& position)
{
   // 1
   _current = _treeView->indexAt(position);
   updateView();
   updateContextMenu();

   // 2
   _contextMenu->exec(QCursor::pos());
}






/*!
 * Updates this object's detailed view using its current index. This does not 
 * update this object's title. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete the current view. If there is no currently selected block then set the 
 *    detailed view pointer to null and return. 
 *
 * 2. Create an abstract block pointer _pointer_, setting it to this object's 
 *    currently selected block. Call the abstract block's make view interface, 
 *    saving the returned detailed view to this object. If the detailed view 
 *    returned is a null pointer then throw an exception. 
 *
 * 3. Set this object's scroll area widget to this object's new detailed view 
 *    widget and update this object's title with the abstract block _pointer_. 
 */
void BlockView::updateView()
{
   // 1
   delete _view;
   if ( !_current.isValid() )
   {
      _view = nullptr;
      return;
   }

   // 2
   AbstractBlock* pointer {_model->pointer(_current)};
   _view = pointer->makeView().release();
   if ( !_view )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Got unexpected nullptr when creating view widget for block."));
      throw e;
   }

   // 3
   _area->setWidget(_view);
   updateTitle(pointer);
}






/*!
 * Updates this object's current index using its selection model. This does not 
 * update this object's actions or context menu. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the list of selected indexes from this object's selection model. If the 
 *    returned list is not empty then set the first index to this object's current 
 *    index, else set this object's current index as invalid. 
 */
void BlockView::updateIndex()
{
   // 1
   QModelIndexList list {_selectionModel->selection().indexes()};
   if ( list.isEmpty() ) _current = QModelIndex();
   else _current = list.first();
}






/*!
 * Updates this object's context menu based off its current index, updating its 
 * actions beforehand as well. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Update this object's actions and clear its add menu. 
 *
 * 2. Iterate through all of this object's add actions and add them to its add 
 *    menu. If the add menu is empty then disable it or enable it otherwise. 
 */
void BlockView::updateContextMenu()
{
   // 1
   updateActions();
   _addMenu->clear();

   // 2
   for (auto action : qAsConst(_addActions)) _addMenu->addAction(action);
   _addMenu->setDisabled(_addMenu->isEmpty());
}






/*!
 * Updates this object's list of actions, enabling or disabling the primary ones 
 * and rebuilding the list of add actions based off the current index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Update the add actions list based off this object's current index. 
 *
 * 2. Enable or disable this object's remove, edit, copy, cut, move up, and move 
 *    down actions based off its current index being valid or not. 
 *
 * 3. Enable or disable the paste action based off the can paste method and the add 
 *    menu based off if this object has a model or not. 
 */
void BlockView::updateActions()
{
   // 1
   updateAddActions();

   // 2
   bool selected {_current.isValid()};
   _removeAction->setDisabled(!selected);
   _editAction->setDisabled(!selected);
   _copyAction->setDisabled(!selected);
   _cutAction->setDisabled(!selected);
   _moveUpAction->setDisabled(!selected);
   _moveDownAction->setDisabled(!selected);

   // 3
   _pasteAction->setDisabled(!canPaste());
   _addMenu->setDisabled(!_model);
}






/*!
 * Updates the add actions list of this object, clearing out the old list and 
 * building it again based on this object's current index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete all add actions on this object's list and clear it. If this object 
 *    does not have a model then return. 
 *
 * 2. Iterate through the build list of the block this object's current index 
 *    points to, adding a new action to the add actions list for adding it. Connect 
 *    each new action to the add triggered slot using a lambda to remember the 
 *    block type to be added if it is triggered. 
 */
void BlockView::updateAddActions()
{
   // 1
   qDeleteAll(_addActions);
   _addActions.clear();
   if ( !_model ) return;

   // 2
   for (auto type : _model->pointer(_current)->buildList())
   {
      _addActions.append(new QAction(_factory->name(type),this));
      connect(_addActions.back(),&QAction::triggered,[this,type]{ addTriggered(type); });
   }
}






/*!
 * Updates this object's title bar based on using the given block. 
 *
 * @param block  
 *
 *
 * Steps of Operation: 
 *
 * 1. Clear this object's title icon and text labels, setting them to the icon and 
 *    name of the given block. 
 */
void BlockView::updateTitle(AbstractBlock* block)
{
   // 1
   _titleIcon->clear();
   _titleText->clear();
   _titleIcon->setPixmap(block->icon().pixmap(_titleIconSize,_titleIconSize));
   _titleText->setText(block->name());
}






/*!
 * Restores this object's Qt splitter state using Qt settings. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a qt settings object and restore the state of this object's qt 
 *    splitter using this class's key. 
 */
void BlockView::restoreSettings()
{
   // 1
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreState(settings.value(_stateKey).toByteArray());
}






/*!
 * Saves this object's Qt splitter state using Qt settings. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a qt settings object and save the state of this object's qt splitter 
 *    using this class's key. 
 */
void BlockView::saveSettings()
{
   // 1
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_stateKey,saveState());
}






/*!
 * Constructs and initializes all GUI elements for this widget. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Construct and setup the tree view, area, actions, and menu for this new 
 *    widget. 
 */
void BlockView::setupGui()
{
   // 1
   setupTreeView();
   setupArea();
   setupActions();
   setupMenu();
}






/*!
 * Constructs and initializes the tree view of this new widget, adding it to this 
 * qt splitter. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new tree view for this object, setting its header to be hidden and 
 *    its context menu policy to request for a custom menu. 
 *
 * 2. Connect this object's new tree view custom context menu requested signal to 
 *    this object and add the tree view to this qt splitter. 
 */
void BlockView::setupTreeView()
{
   // 1
   _treeView = new QTreeView;
   _treeView->setHeaderHidden(true);
   _treeView->setContextMenuPolicy(Qt::CustomContextMenu);

   // 2
   connect(_treeView,&QTreeView::customContextMenuRequested,this,&BlockView::contextMenuRequested);
   addWidget(_treeView);
}






/*!
 * Constructs and initializes the title and detailed view area of this new widget, 
 * adding it to the qt splitter. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new scroll area for this object, setting it to be widget resizable. 
 *
 * 2. Create a new layout _layout_. Add this object's new title bar by calling the 
 *    setup title bar method, spacing, and then this object's new scroll area to 
 *    _layout_. 
 *
 * 3. Add _layout_ to this qt splitter by first adding it to a plain widget because 
 *    qt splitter does not support adding layouts. 
 */
void BlockView::setupArea()
{
   // 1
   _area = new QScrollArea;
   _area->setWidgetResizable(true);

   // 2
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTitleBar());
   layout->addSpacing(8);
   layout->addWidget(_area);

   // 3
   QWidget* widget {new QWidget};
   widget->setLayout(layout);
   addWidget(widget);
}






/*!
 * Constructs and initializes a new title bar for this new widget, returning a 
 * layout of the new title bar. 
 *
 * @return Pointer to layout of the new title bar. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create new labels for this object's title icon and text. Set the font for the 
 *    title text to be bold and the format to be plain text. 
 *
 * 2. Create a new horizontal layout _ret_, adding this object's new title bar, 
 *    then title text, and then a stretch. 
 *
 * 3. Return _ret_. 
 */
QLayout* BlockView::setupTitleBar()
{
   // 1
   _titleIcon = new QLabel;
   _titleText = new QLabel;
   QFont font;
   font.setBold(true);
   _titleText->setFont(font);
   _titleText->setTextFormat(Qt::PlainText);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_titleIcon);
   ret->addWidget(_titleText);
   ret->addStretch();

   // 3
   return ret;
}






/*!
 * Constructs and initializes all actions for this new widget object's context 
 * menu, excluding the add actions because they are rebuilt dynamically. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new action for this object's remove action. 
 *
 * 2. Create a new action for this object's edit action. 
 *
 * 3. Create a new action for this object's cut action. 
 *
 * 4. Create a new action for this object's copy action. 
 *
 * 5. Create a new action for this object's paste action. 
 *
 * 6. Create a new action for this object's move up action. 
 *
 * 7. Create a new action for this object's move down action. 
 */
void BlockView::setupActions()
{
   // 1
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   _removeAction->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(_removeAction,&QAction::triggered,this,&BlockView::removeTriggered);

   // 2
   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   _editAction->setShortcut(Qt::CTRL + Qt::Key_E);
   connect(_editAction,&QAction::triggered,this,&BlockView::editTriggered);

   // 3
   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   _cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
   connect(_cutAction,&QAction::triggered,this,&BlockView::cutTriggered);

   // 4
   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   _copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
   connect(_copyAction,&QAction::triggered,this,&BlockView::copyTriggered);

   // 5
   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   _pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
   connect(_pasteAction,&QAction::triggered,this,&BlockView::pasteTriggered);

   // 6
   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   _moveUpAction->setShortcut(Qt::CTRL + Qt::Key_Up);
   connect(_moveUpAction,&QAction::triggered,this,&BlockView::moveUpTriggered);

   // 7
   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   _moveDownAction->setShortcut(Qt::CTRL + Qt::Key_Down);
   connect(_moveDownAction,&QAction::triggered,this,&BlockView::moveDownTriggered);
}






/*!
 * Constructs and initializes the context menu for this new widget, excluding the 
 * add menu because it is rebuilt dynamically. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new menu for this object's context menu and then add another new 
 *    menu for this object's add menu to the new context menu. 
 *
 * 2. Add the remove, edit, cut, copy, paste, move up, and move down actions to the 
 *    new context menu. 
 */
void BlockView::setupMenu()
{
   // 1
   _contextMenu = new QMenu(tr("&Edit"),this);
   _addMenu = _contextMenu->addMenu(tr("&Add"));

   // 2
   _contextMenu->addAction(_removeAction);
   _contextMenu->addAction(_editAction);
   _contextMenu->addAction(_cutAction);
   _contextMenu->addAction(_copyAction);
   _contextMenu->addAction(_pasteAction);
   _contextMenu->addAction(_moveUpAction);
   _contextMenu->addAction(_moveDownAction);
}
