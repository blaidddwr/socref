#include "blockmodel.h"
#include "abstract_block.h"
#include "abstract_blockfactory.h"



//






/*!
 * Implements _QAbstractItemModel_ interface. 
 *
 * @param row See Qt docs. 
 *
 * @param column See Qt docs. 
 *
 * @param parent See Qt docs. 
 *
 * @return See Qt docs. 
 */
QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   // If the given row or column is not valid then return an invalid index. 
   if ( row < 0 || column != 0 ) return QModelIndex();

   // Get the block pointer for the given parent index and make sure the row is 
   // within range of its child size. 
   Abstract::Block* parentPtr {pointer(parent)};
   if ( row >= parentPtr->size() ) return QModelIndex();

   // Return a new index with its internal pointer set to the block pointer. 
   return createIndex(row,column,parentPtr->get(row));
}






/*!
 * Implements _QAbstractItemModel_ interface. 
 *
 * @param child See Qt docs. 
 *
 * @return See Qt docs. 
 */
QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   // Get the block pointer of the given child index, making sure it is not null. 
   Abstract::Block* childBlock {pointer(child)};
   if ( !childBlock ) return QModelIndex();

   // Get the parent of the child block, making sure it is not null and its parent is 
   // not null. 
   Abstract::Block* parent = childBlock->parent();
   if ( !parent || !parent->parent() ) return QModelIndex();

   // Return the given index's parent index using the grandparent to get the parent's 
   // row. 
   Abstract::Block* grandparent = parent->parent();
   return createIndex(grandparent->indexOf(parent),0,parent);
}






/*!
 * Implements _QAbstractItemModel_ interface. This returns the number of children 
 * the block with the given index contains. 
 *
 * @param index See Qt docs. 
 *
 * @return See Qt docs. 
 */
int BlockModel::rowCount(const QModelIndex& index) const
{
   // Check if this model has no root block. 
   if ( !_root ) return 0;

   // Get the block pointer for the given index, returning 0 if it is null or its 
   // children size if it is not. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return 0;
   else return block->size();
}






/*!
 * Implements _QAbstractItemModel_ interface. This model always has 1 column. 
 *
 * @param parent See Qt docs. 
 *
 * @return See Qt docs. 
 */
int BlockModel::columnCount(const QModelIndex& parent) const
{
   // Return 1. 
   Q_UNUSED(parent)
   return 1;
}






/*!
 * Implements _QAbstractItemModel_ interface. This model only implements the 
 * display and decoration roles. 
 *
 * @param index See Qt docs. 
 *
 * @param role See Qt docs. 
 *
 * @return See Qt docs. 
 */
QVariant BlockModel::data(const QModelIndex& index, int role) const
{
   // Get the block pointer for the given index, returning a null qt variant if it is 
   // null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return QVariant();

   // Based off the role return either the block's display name for display role or 
   // the block's icon for decoration role. The icon is transformed to a pixel map 
   // with specific dimensions. 
   switch (role)
   {
   case Qt::DisplayRole:
      return QVariant(block->name());
   case Qt::DecorationRole:
      return block->icon().pixmap(_iconSize,_iconSize);
   default:
      return QVariant();
   }
}






/*!
 * Constructs a new block model with the optional parent. 
 *
 * @param parent The parent for this model. 
 */
BlockModel::BlockModel(QObject* parent):
   QAbstractItemModel(parent)
{}






/*!
 * Returns the block pointer the given index represents in this model. If the given 
 * index is invalid this model's root block pointer is returned. 
 *
 * @param index The index whose block pointer is returned. 
 *
 * @return Block pointer of the given index or the root block pointer if the index 
 *         is invalid. 
 */
Abstract::Block* BlockModel::pointer(const QModelIndex& index) const
{
   // If the given index is valid then return its internal pointer else return this 
   // model's root block pointer. 
   if ( index.isValid() ) return reinterpret_cast<Abstract::Block*>(index.internalPointer());
   return _root;
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
 */
bool BlockModel::insert(const QModelIndex& index, Soc::Ut::QPtr<Abstract::Block>&& block)
{
   // Make sure the given pointer is not null. 
   if ( !block ) return false;

   // Get the parent block pointer to the given index and make sure it is not null. 
   Abstract::Block* parent {pointer(index)};
   if ( !parent ) return false;

   // Insert the given child block at the beginning of the given parent's block's 
   // child list, notifying the model of a row insertion and returning success. 
   beginInsertRows(index,0,0);
   parent->insert(0,std::move(block));
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
 */
QModelIndex BlockModel::moveUp(const QModelIndex& index)
{
   // Make sure the given index is valid and is not in the first row. 
   if ( !index.isValid() || index.row() == 0 ) return index;

   // Get the block pointer from the given index and make sure it is not null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return index;

   // Get the parent block pointer and make sure it is not null. 
   Abstract::Block* parent {block->parent()};
   if ( !parent ) return index;

   // Move the block with the given index up by one in it's parent's child list, 
   // notifying the model that the row has moved. SPECIAL NOTE: the given index is 
   // destroyed when the begin move rows qt function is called. 
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   parent->moveUp(row);
   endMoveRows();

   // Return a new index where the given index's block moved. 
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
 */
QModelIndex BlockModel::moveDown(const QModelIndex& index)
{
   // Make sure the given index is valid and is not in the last row. 
   if ( index.row() >= (rowCount(index.parent()) - 1) ) return index;

   // Get the block pointer from the given index and make sure it is not null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return index;

   // Get the parent block pointer and make sure it is not null. 
   Abstract::Block* parent {block->parent()};
   if ( !parent ) return index;

   // Move the block with the given index down by one in it's parent's child list, 
   // notifying the model that the row has moved. SPECIAL NOTE: the given index is 
   // destroyed when the begin move rows qt function is called. 
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   parent->moveDown(row);
   endMoveRows();

   // Return a new index where the given index's block moved. 
   return createIndex(row + 1,0,block);
}






/*!
 * Removes the given index and its block for this model. If the index is not given 
 * this does nothing and returns false. 
 *
 * @param index The index that is removed from this model. 
 *
 * @return True on success or false on failure. 
 */
bool BlockModel::remove(const QModelIndex& index)
{
   // Make sure the given index is valid. 
   if ( !index.isValid() ) return false;

   // Get the block pointer from the given index and make sure it is not null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return false;

   // Get the parent block pointer and make sure it is not null. 
   Abstract::Block* parent {block->parent()};
   if ( !parent ) return false;

   // Remove the block with the given index from its parent block, notifying the 
   // model that a row was removed and returning success. SPECIAL NOTE: the given 
   // index is destroyed when the begin remove rows qt function is called. 
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
 */
Soc::Ut::QPtr<Abstract::Block> BlockModel::copy(const QModelIndex& index) const
{
   // If the given index is not valid then return null. 
   if ( !index.isValid() ) return nullptr;

   // Get the block pointer from the given index and make sure it is not null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return nullptr;

   // Make a copy of the block with the given index and return its pointer. 
   return block->makeCopy();
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
 */
Soc::Ut::QPtr<Abstract::Block> BlockModel::cut(const QModelIndex& index)
{
   // Make sure the given index is valid. 
   if ( !index.isValid() ) return nullptr;

   // Get the block pointer from the given index and make sure it is not null. 
   Abstract::Block* block {pointer(index)};
   if ( !block ) return nullptr;

   // Get the parent block pointer and make sure it is not null. 
   Abstract::Block* parent {block->parent()};
   if ( !parent ) return nullptr;

   // Take the child block with the given index from its parent block, notifying the 
   // model that a row was removed and returning the taken block's pointer. SPECIAL 
   // NOTE: the given index is destroyed when the begin remove rows qt function is 
   // called. 
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   Soc::Ut::QPtr<Abstract::Block> ret {parent->take(row)};
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
const Abstract::BlockFactory* BlockModel::factory() const
{
   return _factory;
}






/*!
 * Sets a new root block for this model's data. If the given pointer is null then 
 * this model is set to be empty with no data to represent. 
 *
 * @param root Pointer to root block that this model will use or null to set this 
 *             model as empty. 
 */
void BlockModel::setRoot(Abstract::Block* root)
{
   // Notify the model that a reset has begun. 
   beginResetModel();

   // Disconnect any previous root block of this model. 
   if ( _root ) disconnect(_root);

   // Set the new root for this model and its factory to null just in case the new 
   // root is null. 
   _root = root;
   _factory = nullptr;

   // Check if the new root is not null. 
   if ( _root )
   {
      // Set this model's block factory to the one from this model's new root block. 
      _factory = &(_root->factory());

      // Connect the child updated signal of the new root block to this model. 
      connect(_root,&Abstract::Block::childUpdated,this,&BlockModel::blockUpdated);
   }

   // Notify the model that the reset has ended. 
   endResetModel();
}






/*!
 * Called when the root block contained within this model emits a child updated 
 * signal indicating a block with the given pointer within this model has updated 
 * itself. 
 *
 * @param block Pointer to the block whose name has changed. 
 */
void BlockModel::blockUpdated(Abstract::Block* block)
{
   // Get the parent block pointer of the given block and make sure it is not null. 
   Abstract::Block* parent {block->parent()};
   Q_CHECK_PTR(parent);

   // Determine the index for the given block and signal the data changed for that 
   // index and given roles. 
   QModelIndex index = createIndex(parent->indexOf(block),0,block);
   emit dataChanged(index,index);
}
