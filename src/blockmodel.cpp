#include "blockmodel.h"
#include <exception.h>
#include "abstractblockfactory.h"
#include "abstractblock.h"



using namespace std;
//






/*!
 * This constructs a new block model with the given root block and parent. Both the 
 * root block and parent can be given as null. A null root block means this model 
 * is empty. 
 *
 * @param root The root block this model will use for its data or null to make this 
 *             model empty. 
 *
 * @param parent The parent for this model. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the root given is not null then get the block type's block factory and 
 *    connect the name changed signal with the new root block, else do nothing. 
 */
BlockModel::BlockModel(AbstractBlock* root, QObject* parent):
   QAbstractItemModel(parent),
   _root(root)
{
   if ( _root )
   {
      _factory = &(_root->factory());
      connect(_root,&AbstractBlock::nameChanged,this,&BlockModel::blockNameChanged);
   }
}






/*!
 * This implements the interface that returns a a model index for the given row, 
 * column, and parent. 
 *
 * @param row The row whose index is returned. 
 *
 * @param column The column whose index is returned. 
 *
 * @param parent The parent index of the index that is returned. 
 *
 * @return A model index for the given row, column, and parent. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given row is less than 0 or the column is not 0 then return an invalid 
 *    index, else go to the next step. 
 *
 * 2. Get the block pointer of the given parent index. If the given row is out of 
 *    range of the parent block's list of children then return an invalid index, 
 *    else go to the next step. 
 *
 * 3. Return a new index with the given row, column, and parent index. 
 */
QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   if ( row < 0 || column != 0 ) return QModelIndex();
   AbstractBlock* parent_ {pointer(parent)};
   if ( row >= parent_->size() ) return QModelIndex();
   return createIndex(row,column,parent_->get(row));
}






/*!
 * This implements the interface that returns the parent index of a given index. 
 *
 * @param child The index whose parent is returned. 
 *
 * @return Parent index of the given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the pointer to the given index's parent block. If the parent block 
 *    pointer is null or the parent's parent is null then return an invalid index, 
 *    else to go the next step. 
 *
 * 2. Return the given index's parent index using the grandparent to get the 
 *    parent's row. 
 */
QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   AbstractBlock* parent = pointer(child)->parent();
   if ( !parent || !parent->parent() ) return QModelIndex();
   AbstractBlock* grandparent = parent->parent();
   return createIndex(grandparent->indexOf(parent),0,parent);
}






/*!
 * This implements the interface that returns the number of rows the given index 
 * contains. 
 *
 * @param index The index whose contained number of rows is returned. 
 *
 * @return Number of rows the given index contains. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model's root pointer is null then return 0, else return the number of 
 *    block children this index's block contains. 
 */
int BlockModel::rowCount(const QModelIndex& index) const
{
   if ( !_root ) return 0;
   return pointer(index)->size();
}






/*!
 * This implements the interface that returns the number of columns the given index 
 * contains. This model always contains 1 column regardless of index. 
 *
 * @param parent The index is not used for this model because the number of columns 
 *               is static. 
 *
 * @return This models column count is always 1. 
 */
int BlockModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   return 1;
}






/*!
 * This implements the interface that returns data for the given index with the 
 * given role. This model only implements the display and decoration roles. 
 *
 * @param index The index whose data is returned. 
 *
 * @param role The role of the requested data. 
 *
 * @return Data requested from given index and role. For this model this is a 
 *         string or qt image. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given role is the display role then return the given index's block 
 *    name, else if the given role is the decoration role then return the given 
 *    index's block icon resized to this model's preferred icon size, else this is 
 *    an unsupported role so return nothing. 
 */
QVariant BlockModel::data(const QModelIndex& index, int role) const
{
   switch (role)
   {
   case Qt::DisplayRole:
      return QVariant(pointer(index)->name());
   case Qt::DecorationRole:
      return pointer(index)->icon().pixmap(_iconSize,_iconSize);
   default:
      return QVariant();
   }
}






/*!
 * Returns the block pointer the given index represents in this model. If the given 
 * index is invalid this model's root block pointer is returned. 
 *
 * @param index The index whose block pointer is returned. 
 *
 * @return Block pointer of the given index or the root block pointer if the index 
 *         is invalid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is valid return the given index's internal pointer recast 
 *    as a block pointer, else it is invalid so return this model's root block 
 *    pointer. 
 */
AbstractBlock* BlockModel::pointer(const QModelIndex& index) const
{
   AbstractBlock* ret;
   if ( index.isValid() ) ret = reinterpret_cast<AbstractBlock*>(index.internalPointer());
   else ret = _root;
   return ret;
}






/*!
 * Inserts a new block into this model at the beginning of the given index's list 
 * of children. 
 *
 * @param index The index where the given block is inserted. 
 *
 * @param block Pointer to the block that is inserted into this model. 
 *
 * @return True on success or false on failure. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given block pointer is null then return false, else go to the next 
 *    step. 
 *
 * 2. Insert the given block pointer as a new child of the given index's block, 
 *    signaling the model that a new row is being inserted. The new block is 
 *    prepended to the block's list of children. 
 *
 * 3. Return true on success. 
 */
bool BlockModel::insert(const QModelIndex& index, std::unique_ptr<AbstractBlock>&& block)
{
   if ( !block ) return false;
   beginInsertRows(index,0,0);
   pointer(index)->insert(0,std::move(block));
   endInsertRows();
   return true;
}






/*!
 * Moves the given index up by one in the list of siblings it is part of. If the 
 * given index is already at row 0 then this does nothing. Returns the new index 
 * for the given index that was or was not moved. 
 *
 * @param index The index that will possibly be moved up by 1. 
 *
 * @return New index for the given index that was possibly moved. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index's row is already 0 then return the given index unchanged, 
 *    else go to the next step. 
 *
 * 2. Move the block of the given index up by one in it's parent's list of 
 *    children, signaling the model that the row is being moved. SPECIAL NOTE: the 
 *    given index is destroyed when the begin move rows qt function is called. 
 *
 * 3. Return a new index where this index has been successfully moved to. 
 */
QModelIndex BlockModel::moveUp(const QModelIndex& index)
{
   if ( index.row() == 0 ) return index;
   AbstractBlock* block {pointer(index)};
   AbstractBlock* parent {block->parent()};
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   parent->moveUp(row);
   endMoveRows();
   return createIndex(row - 1,0,block);
}






/*!
 * Moves the given index down by one in the list of siblings it is part of. If the 
 * given index is already at the end of its list of siblings then this does 
 * nothing. Returns the new index for the given index that was or was not moved. 
 *
 * @param index The index that will possibly be moved up by 1. 
 *
 * @return New index for the given index that was possibly moved. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index's row is already at the end of its sibling list then 
 *    return the given index unchanged, else go to the next step. 
 *
 * 2. Move the block of the given index down by one in it's parent's list of 
 *    children, signaling the model that the row is being moved. SPECIAL NOTE: the 
 *    given index is destroyed when the begin move rows qt function is called. 
 *
 * 3. Return a new index where this index has been successfully moved to. 
 */
QModelIndex BlockModel::moveDown(const QModelIndex& index)
{
   if ( index.row() >= (rowCount(index.parent()) - 1) ) return index;
   AbstractBlock* block {pointer(index)};
   AbstractBlock* parent {block->parent()};
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   parent->moveDown(row);
   endMoveRows();
   return createIndex(row + 1,0,block);
}






/*!
 * Removes the given index and its block for this model. If the index is not given 
 * this does nothing and returns false. 
 *
 * @param index The index that is removed from this model. 
 *
 * @return True on success or false on failure. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is invalid then return false, else go to the next step. 
 *
 * 2. Remove the given index's block from its parent block, signaling the model 
 *    that a row is being removed. SPECIAL NOTE: the given index is destroyed when 
 *    the begin remove rows qt function is called. 
 *
 * 3. Return true on success. 
 */
bool BlockModel::remove(const QModelIndex& index)
{
   if ( !index.isValid() ) return false;
   AbstractBlock* parent {pointer(index.parent())};
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   parent->remove(row);
   endRemoveRows();
   return true;
}






/*!
 * Returns a pointer to a copy of the block at the given index. If the given index 
 * is not valid then a null pointer is returned. 
 *
 * @param index The given index whose block is copied and returned. 
 *
 * @return Pointer to a copy of the given index or a null pointer if the given 
 *         index is invalid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is not valid then return a null pointer, else return a 
 *    pointer to a new copy of the block at the given index. 
 */
std::unique_ptr<AbstractBlock> BlockModel::copy(const QModelIndex& index) const
{
   if ( !index.isValid() ) return nullptr;
   return pointer(index.parent())->get(index.row())->makeCopy();
}






/*!
 * Returns a pointer to the block at the given index, removing that index from the 
 * model but not deleting its block. Instead the ownership of the block is 
 * returned. If the given index is not valid then a null pointer is returned. 
 *
 * @param index The index that is removed from this model and its block returned. 
 *
 * @return Pointer to the block at the given index now removed or null pointer if 
 *         the given index is not valid. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is not valid then return a null pointer, else go to the 
 *    next step. 
 *
 * 2. Take the block at the given index from its parent block, signaling the model 
 *    that a row is being removed. SPECIAL NOTE: the given index is destroyed when 
 *    the begin remove rows qt function is called. 
 *
 * 3. Return a pointer to the block that was taken from its parent. 
 */
std::unique_ptr<AbstractBlock> BlockModel::cut(const QModelIndex& index)
{
   if ( !index.isValid() ) return nullptr;
   AbstractBlock* parent {pointer(index.parent())};
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   unique_ptr<AbstractBlock> ret {parent->take(row)};
   endRemoveRows();
   return ret;
}






/*!
 * Returns a read only pointer to the block factory of the project type for this 
 * model's blocks. If the model is not set to any block data then a null pointer is 
 * returned. 
 *
 * @return Read only pointer to black factory for this model's block type or null 
 *         pointer if this model is empty. 
 */
const AbstractBlockFactory* BlockModel::factory() const
{
   return _factory;
}






/*!
 * Sets a new root block for this model's data. If the given pointer is null then 
 * this model is set to be empty with no data to represent. 
 *
 * @param newRoot Pointer to root block that this model will use or null to set 
 *                this model as empty. 
 *
 *
 * Steps of Operation: 
 *
 * 1. For this entire operation signal to the model that the entire model is being 
 *    reset to a new one. 
 *
 * 2. If this model already has a root block pointer then disconnect all qt signals 
 *    between this model and that root block. 
 *
 * 3. Set this model's root block to the new pointer given. If the new pointer is 
 *    not null then set this model's block factory pointer to this block type's 
 *    block factory and connect the name changed signal, else set this model's 
 *    block factory pointer to null. 
 */
void BlockModel::setRoot(AbstractBlock* newRoot)
{
   beginResetModel();
   if ( _root )
   {
      disconnect(_root);
   }
   _root = newRoot;
   _factory = nullptr;
   if ( _root )
   {
      _factory = &(_root->factory());
      connect(_root,&AbstractBlock::nameChanged,this,&BlockModel::blockNameChanged);
   }
   endResetModel();
}






/*!
 * Called when a block contained within this model has changed its display name. 
 *
 * @param block Pointer to the block whose display name has changed. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given block pointer has no parent then throw an exception, else go to 
 *    the next step. 
 *
 * 2. Determine the index for the given block pointer within this model and signal 
 *    the model that the index's data has changed. 
 */
void BlockModel::blockNameChanged(AbstractBlock* block)
{
   if ( !block->parent() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("A name changed signal was emitted with the root block."));
      throw e;
   }
   QModelIndex index = createIndex(block->parent()->indexOf(block),0,block);
   emit dataChanged(index,index);
}
