#include "gui_blockview.h"
#include <QTreeView>
#include <QScrollArea>
#include <QMenu>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "gui_abstractedit.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "blockmodel.h"
#include "application.h"



using namespace Gui;
//



/*!
 * Pointer to a copied block that any instance of this class can use to paste into 
 * their current block model if possible. 
 */
AbstractBlock* BlockView::_copy {nullptr};






/*!
 * Constructs a new block view with no model and the optional given parent. 
 *
 * @param parent  
 */
BlockView::BlockView(QWidget* parent):
   QSplitter(parent)
{
   // Setup this object's GUI and update its context menu. 
   setupGui();
   updateContextMenu();
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
 */
bool BlockView::canPaste() const
{
   // Make sure this view has a model, the copy pointer is not null, and the block 
   // factory project types match. 
   if ( !_model || !_copy || _factory->type() != _copy->factory().type() ) return false;

   // If this view's current index is not a null pointer and the copied block's type 
   // is contained within the current block's build list then return true, else 
   // return false. 
   AbstractBlock* index {_model->pointer(_current)};
   if ( index && index->buildList().contains(_copy->type()) ) return true;
   else return false;
}






/*!
 * Sets a new block model for this block view, releasing but not deleting any 
 * previous model set to it. If a null pointer is given this view is set to empty. 
 * This block view does not take ownership of the given model. 
 *
 * @param model Pointer to the new block model this block view will display for 
 *              editing. 
 */
void BlockView::setModel(BlockModel* model)
{
   // If this view already had a model then disconnect its and its selection model's 
   // signals. 
   if ( _model )
   {
      disconnect(_model);
      disconnect(_selectionModel);
   }

   // Set the new model for this view and reset its current index to invalid. 
   _treeView->setModel(model);
   _model = model;
   _current = QModelIndex();

   // Check to see if the new model is a null pointer. 
   if ( !_model )
   {
      // Reset this view's factory and selection model pointers to null. 
      _factory = nullptr;
      _selectionModel = nullptr;
   }

   // Else the given pointer is not null and this view's new model is real. 
   else
   {
      // Set the block factory and selection model pointers of this view. 
      _factory = model->factory();
      _selectionModel = _treeView->selectionModel();

      // Connect all selection model and model signals to this view. 
      connect(_selectionModel
              ,&QItemSelectionModel::selectionChanged
              ,this
              ,&BlockView::selectionModelChanged);
      connect(_model,&BlockModel::dataChanged,this,&BlockView::modelDataChanged);
      connect(_model,&BlockModel::destroyed,this,&BlockView::modelDestroyed);
   }

   // Update this object's view and context menu. 
   updateView();
   updateContextMenu();
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
   // Make sure this view has a model. 
   if ( !_model ) return;

   // Insert a new block of the given type into this view's model with its current 
   // index, updating its current index and context menu. 
   _model->insert(_current,_factory->makeBlock(type,true));
   updateIndex();
   updateContextMenu();
}






/*!
 * Called when the remove action is triggered. If this object has a valid current 
 * index then it is removed from its model. 
 */
void BlockView::removeTriggered()
{
   // Make sure this view's current index is valid. 
   if ( !_current.isValid() ) return;

   // Remove the currently selected index from its model, updating this view's 
   // current index and context menu. 
   _model->remove(_current);
   updateIndex();
   updateContextMenu();
}






/*!
 * Called when the edit action is triggered. If this object has a valid current 
 * index then this will open an edit dialog of the index's block. 
 */
void BlockView::editTriggered()
{
   // Make sure this view's current index is valid. 
   if ( !_current.isValid() ) return;

   // Get the block pointer from the current index and make sure it is not null. 
   AbstractBlock* block {_model->pointer(_current)};
   if ( !block ) return;

   // Make a new edit dialog from the block pointer and make sure it is not null. 
   Soc::Ut::QPtr<AbstractEdit> edit {block->makeEdit()};
   if ( !edit ) return;

   // Initialize the edit dialog and then execute it. 
   edit->initialize();
   edit->setWindowIcon(block->icon());
   edit->setWindowTitle(tr("Edit %1").arg(_factory->name(block->type())));
   edit->exec();
}






/*!
 * Called when the cut action is triggered. If this object has a valid current 
 * index then this will cut it from this object's model. 
 */
void BlockView::cutTriggered()
{
   // Make sure this view's current index is valid. 
   if ( !_current.isValid() ) return;

   // Delete any existing block copy. SPECIAL NOTE: This view's copy pointer must be 
   // set to null before a new block is cut from the model or else a dead copy 
   // pointer will cause a segmentation fault. 
   delete _copy;
   _copy = nullptr;

   // Cut the current index's block from its model, updating this view's index and 
   // context menu. 
   _copy = _model->cut(_current).release(QApplication::instance());
   updateIndex();
   updateContextMenu();
}






/*!
 * Called when the copy action is triggered. If this object has a valid current 
 * index then this will copy it from this object's model. 
 */
void BlockView::copyTriggered()
{
   // Make sure this view's current index is valid. 
   if ( !_current.isValid() ) return;

   // Delete any existing block copy. 
   delete _copy;

   // Copy the current index's block from its model, updating this view's actions. 
   _copy = _model->copy(_current).release(QApplication::instance());
   updateActions();
}






/*!
 * Called when the paste action is triggered. If this object can paste its copied 
 * block into its model then it does so. 
 */
void BlockView::pasteTriggered()
{
   // If pasting is possible then insert a new copy of the copied block into this 
   // view's model at the current index. 
   if ( canPaste() ) _model->insert(_current,_copy->makeCopy());
}






/*!
 * Called when the move up action is triggered. If this object has a valid current 
 * index then this will move it up by one. 
 */
void BlockView::moveUpTriggered()
{
   // If this view's current index is valid then move it up by one and update it. 
   if ( _current.isValid() ) _current = _model->moveUp(_current);
}






/*!
 * Called when the move down action is triggered. If this object has a valid 
 * current index then this will move it down by one. 
 */
void BlockView::moveDownTriggered()
{
   // If this view's current index is valid then move it down by one and update it. 
   if ( _current.isValid() ) _current = _model->moveDown(_current);
}






/*!
 * Called when this object's selection model has changed. This determines its 
 * object's new current index, updating its view and context menu afterwords. 
 *
 * @param selected List of indexes that have been selected by the user. 
 *
 * @param deselected Unused list of indexes that have been deselected by the user. 
 */
void BlockView::selectionModelChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
   // This does not care what was deselected. 
   Q_UNUSED(deselected)

   // Update this view's current index, setting it to invalid to no indexes are 
   // selected. 
   if ( selected.isEmpty() ) _current = QModelIndex();
   else _current = selected.indexes().first();

   // Update this object's view and context menu. 
   updateView();
   updateContextMenu();
}






/*!
 * Called when this object's block model is about to be deleted. This clears its 
 * object's view to a null state and all internal pointers. 
 */
void BlockView::modelDestroyed()
{
   // Clear this view's title. 
   _titleIcon->clear();
   _titleText->clear();

   // Reset this view as empty with no model. 
   _model = nullptr;
   _factory = nullptr;
   _selectionModel = nullptr;
   _current = QModelIndex();

   // Update this object's view and context menu. 
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
 */
void BlockView::modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
   // This does not care about the bottom right because this view works as a single 
   // selection model. 
   Q_UNUSED(bottomRight)

   // Make sure the index whose data has changed is the current index and it is 
   // valid. 
   if ( topLeft == _current && _current.isValid() )
   {
      // Update this view's title if needed, detailed view if needed, and context menu. 
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
 */
void BlockView::contextMenuRequested(const QPoint& position)
{
   // Update this view's current index based off the given position, updating the 
   // view and context menu afterwords. 
   _current = _treeView->indexAt(position);
   updateView();
   updateContextMenu();

   // Execute this object's context menu at the mouse's current position. 
   _contextMenu->exec(QCursor::pos());
}






/*!
 * Updates this object's detailed view using its current index. This does not 
 * update this object's title. 
 */
void BlockView::updateView()
{
   // Delete any current view of this object. 
   delete _view;
   _view = nullptr;

   // Make sure this view's current index is valid. 
   if ( !_current.isValid() ) return;

   // Get the block pointer of the current index and make sure it is not null. 
   AbstractBlock* block {_model->pointer(_current)};
   if ( !block ) return;

   // Make a new view from the block pointer and make sure it is not null. 
   _view = block->makeView().release(this);
   Q_CHECK_PTR(_view);

   // Set this object's detailed view and update its title. 
   _area->setWidget(_view);
   updateTitle(block);
}






/*!
 * Updates this object's current index using its selection model. This does not 
 * update this object's actions or context menu. 
 */
void BlockView::updateIndex()
{
   // Get the list of selected indexes from this view's selection model. 
   QModelIndexList list {_selectionModel->selection().indexes()};

   // Set this view's current index to the first index in the list or invalid if the 
   // list is empty. 
   if ( list.isEmpty() ) _current = QModelIndex();
   else _current = list.first();
}






/*!
 * Updates this object's context menu based off its current index, updating its 
 * actions beforehand as well. 
 */
void BlockView::updateContextMenu()
{
   // Update this view's actions and clear its context menu's add menu. 
   updateActions();
   _addMenu->clear();

   // Populate the context menu's add menu with this view's add actions, disabling it 
   // if there are no actions. 
   for (auto action : qAsConst(_addActions)) _addMenu->addAction(action);
   _addMenu->setDisabled(_addMenu->isEmpty());
}






/*!
 * Updates this object's list of actions, enabling or disabling the primary ones 
 * and rebuilding the list of add actions based off the current index. 
 */
void BlockView::updateActions()
{
   // Update the add actions for this view. 
   updateAddActions();

   // Enable or disable all actions of this view dependent on the current index being 
   // valid or not. 
   bool selected {_current.isValid()};
   _removeAction->setDisabled(!selected);
   _editAction->setDisabled(!selected);
   _copyAction->setDisabled(!selected);
   _cutAction->setDisabled(!selected);
   _moveUpAction->setDisabled(!selected);
   _moveDownAction->setDisabled(!selected);

   // Enable or disable all remaining actions of this view. 
   _pasteAction->setDisabled(!canPaste());
   _addMenu->setDisabled(!_model);
}






/*!
 * Updates the add actions list of this object, clearing out the old list and 
 * building it again based on this object's current index. 
 */
void BlockView::updateAddActions()
{
   // Delete all previous add actions of this view. 
   qDeleteAll(_addActions);
   _addActions.clear();

   // Make sure this view has a model. 
   if ( !_model ) return;

   // Get the block pointer from this view's current index and make sure it is not 
   // null. 
   AbstractBlock* block {_model->pointer(_current)};
   if ( !block ) return;

   // Iterate through the build list of the block pointer, adding a new action for 
   // each block type in the list. 
   for (auto type : block->buildList())
   {
      _addActions.append(new QAction(_factory->name(type),this));
      connect(_addActions.back(),&QAction::triggered,[this,type]{ addTriggered(type); });
   }
}






/*!
 * Updates this object's title bar based on the given block's info. If the given 
 * pointer is null then the title is cleared. 
 *
 * @param block Pointer to the block whose icon and display name is used for the 
 *              title if it is not null. 
 */
void BlockView::updateTitle(AbstractBlock* block)
{
   // Clear this view's current title. 
   _titleIcon->clear();
   _titleText->clear();

   // Make sure the given pointer is not null. 
   if ( !block ) return;

   // Set this view's new title using the icon and display name of the given block. 
   _titleIcon->setPixmap(block->icon().pixmap(_titleIconSize,_titleIconSize));
   _titleText->setText(block->name());
}






/*!
 * Constructs and initializes all GUI elements for this widget. 
 */
void BlockView::setupGui()
{
   // Setup the tree view, area, actions, and menu for this new view. 
   setupTreeView();
   setupArea();
   setupActions();
   setupMenu();
}






/*!
 * Constructs and initializes the tree view of this new widget, adding it to this 
 * qt splitter. 
 */
void BlockView::setupTreeView()
{
   // Create and initialize the tree view for this object. 
   _treeView = new QTreeView;
   _treeView->setHeaderHidden(true);
   _treeView->setContextMenuPolicy(Qt::CustomContextMenu);

   // Connect the context menu requested signal and add the tree view to this 
   // object's splitter. 
   connect(_treeView,&QTreeView::customContextMenuRequested,this,&BlockView::contextMenuRequested);
   addWidget(_treeView);
}






/*!
 * Constructs and initializes the title and detailed view area of this new widget, 
 * adding it to the qt splitter. 
 */
void BlockView::setupArea()
{
   // Create and initialize the scroll area for this view. 
   _area = new QScrollArea;
   _area->setWidgetResizable(true);

   // Create a new vertical layout, adding the title bar then spacing and then the 
   // new scroll area. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTitleBar());
   layout->addSpacing(8);
   layout->addWidget(_area);

   // Add the new layout to this object's splitter as a widget. 
   QWidget* widget {new QWidget};
   widget->setLayout(layout);
   addWidget(widget);
}






/*!
 * Constructs and initializes a new title bar for this new widget, returning a 
 * layout of the new title bar. 
 *
 * @return Pointer to layout of the new title bar. 
 */
QLayout* BlockView::setupTitleBar()
{
   // Create the title icon label for this view. 
   _titleIcon = new QLabel;

   // Create and initialize the title text label for this view. 
   _titleText = new QLabel;
   QFont font;
   font.setBold(true);
   _titleText->setFont(font);
   _titleText->setTextFormat(Qt::PlainText);

   // Create a new horizontal layout, adding the title icon label then the title text 
   // label and then a stretch. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_titleIcon);
   ret->addWidget(_titleText);
   ret->addStretch();

   // Return the title bar layout. 
   return ret;
}






/*!
 * Constructs and initializes all actions for this new widget object's context 
 * menu, excluding the add actions because they are rebuilt dynamically. 
 */
void BlockView::setupActions()
{
   // Create and initialize the remove action for this view. 
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   _removeAction->setShortcut(Qt::CTRL + Qt::Key_Delete);
   connect(_removeAction,&QAction::triggered,this,&BlockView::removeTriggered);

   // Create and initialize the edit action for this view. 
   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   _editAction->setShortcut(Qt::CTRL + Qt::Key_E);
   connect(_editAction,&QAction::triggered,this,&BlockView::editTriggered);

   // Create and initialize the cut action for this view. 
   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   _cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
   connect(_cutAction,&QAction::triggered,this,&BlockView::cutTriggered);

   // Create and initialize the copy action for this view. 
   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   _copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
   connect(_copyAction,&QAction::triggered,this,&BlockView::copyTriggered);

   // Create and initialize the paste action for this view. 
   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   _pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
   connect(_pasteAction,&QAction::triggered,this,&BlockView::pasteTriggered);

   // Create and initialize the move up action for this view. 
   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   _moveUpAction->setShortcut(Qt::CTRL + Qt::Key_Up);
   connect(_moveUpAction,&QAction::triggered,this,&BlockView::moveUpTriggered);

   // Create and initialize the move down action for this view. 
   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   _moveDownAction->setShortcut(Qt::CTRL + Qt::Key_Down);
   connect(_moveDownAction,&QAction::triggered,this,&BlockView::moveDownTriggered);
}






/*!
 * Constructs and initializes the context menu for this new widget, excluding the 
 * add menu because it is rebuilt dynamically. 
 */
void BlockView::setupMenu()
{
   // Create the context menu for this view. 
   _contextMenu = new QMenu(tr("&Edit"),this);

   // Add the add menu to the context menu. 
   _addMenu = _contextMenu->addMenu(tr("&Add"));

   // Add all other actions to the context menu. 
   _contextMenu->addAction(_removeAction);
   _contextMenu->addAction(_editAction);
   _contextMenu->addAction(_cutAction);
   _contextMenu->addAction(_copyAction);
   _contextMenu->addAction(_pasteAction);
   _contextMenu->addAction(_moveUpAction);
   _contextMenu->addAction(_moveDownAction);
}
