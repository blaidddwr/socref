#include "blockmodel.h"
#include <exception.h>
#include "abstractblockfactory.h"
#include "abstractblock.h"



using namespace std;
//






/*!
 *
 * @param root  
 *
 * @param parent  
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
 *
 * @param row  
 *
 * @param column  
 *
 * @param parent  
 */
QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   if ( row < 0 || column != 0 ) return QModelIndex();
   AbstractBlock* parent_ {pointer(parent)};
   if ( row >= parent_->size() ) return QModelIndex();
   return createIndex(row,column,parent_->get(row));
}






/*!
 *
 * @param child  
 */
QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   AbstractBlock* parent = pointer(child)->parent();
   if ( !parent || !parent->parent() ) return QModelIndex();
   AbstractBlock* grandparent = parent->parent();
   return createIndex(grandparent->indexOf(parent),0,parent);
}






/*!
 *
 * @param parent  
 */
int BlockModel::rowCount(const QModelIndex& parent) const
{
   if ( !_root ) return 0;
   return pointer(parent)->size();
}






/*!
 *
 * @param parent  
 */
int BlockModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   return 1;
}






/*!
 *
 * @param index  
 *
 * @param role  
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
 *
 * @param index  
 */
AbstractBlock* BlockModel::pointer(const QModelIndex& index) const
{
   AbstractBlock* ret;
   if ( index.isValid() ) ret = reinterpret_cast<AbstractBlock*>(index.internalPointer());
   else ret = _root;
   return ret;
}






/*!
 *
 * @param index  
 *
 * @param block  
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
 *
 * @param index  
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
 *
 * @param index  
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
 *
 * @param index  
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
 *
 * @param index  
 */
std::unique_ptr<AbstractBlock> BlockModel::copy(const QModelIndex& index) const
{
   if ( !index.isValid() ) return nullptr;
   return pointer(index.parent())->get(index.row())->makeCopy();
}






/*!
 *
 * @param index  
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
 */
const AbstractBlockFactory* BlockModel::factory() const
{
   return _factory;
}






/*!
 *
 * @param newRoot  
 */
void BlockModel::setRoot(AbstractBlock* newRoot)
{
   beginResetModel();
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
 *
 * @param block  
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
