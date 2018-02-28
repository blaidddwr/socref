#include "blockmodel.h"
#include <exception.h>
#include "abstractblockfactory.h"
#include "abstractblock.h"



using namespace std;






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






QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   if ( row < 0 || column != 0 ) return QModelIndex();
   AbstractBlock* parent_ {pointer(parent)};
   if ( row >= parent_->childrenSize() ) return QModelIndex();
   return createIndex(row,column,parent_->child(row));
}






QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   AbstractBlock* parent = pointer(child)->parent();
   if ( !parent || !parent->parent() ) return QModelIndex();
   AbstractBlock* grandparent = parent->parent();
   return createIndex(grandparent->childIndex(parent),0,parent);
}






int BlockModel::rowCount(const QModelIndex& parent) const
{
   if ( !_root ) return 0;
   return pointer(parent)->childrenSize();
}






int BlockModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   return 1;
}






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






AbstractBlock* BlockModel::pointer(const QModelIndex& index) const
{
   AbstractBlock* ret;
   if ( index.isValid() ) ret = reinterpret_cast<AbstractBlock*>(index.internalPointer());
   else ret = _root;
   return ret;
}






bool BlockModel::insert(const QModelIndex& index, unique_ptr<AbstractBlock>&& object)
{
   if ( !object ) return false;
   beginInsertRows(index,0,0);
   pointer(index)->insertChild(0,std::move(object));
   endInsertRows();
   return true;
}






QModelIndex BlockModel::moveUp(const QModelIndex& index)
{
   if ( index.row() == 0 ) return index;
   AbstractBlock* block {pointer(index)};
   AbstractBlock* parent {block->parent()};
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   parent->moveChildUp(row);
   endMoveRows();
   return createIndex(row - 1,0,block);
}






QModelIndex BlockModel::moveDown(const QModelIndex& index)
{
   if ( index.row() >= (rowCount(index.parent()) - 1) ) return index;
   AbstractBlock* block {pointer(index)};
   AbstractBlock* parent {block->parent()};
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   parent->moveChildDown(row);
   endMoveRows();
   return createIndex(row + 1,0,block);
}






bool BlockModel::remove(const QModelIndex& index)
{
   if ( !index.isValid() ) return false;
   AbstractBlock* parent {pointer(index.parent())};
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   parent->removeChild(row);
   endRemoveRows();
   return true;
}






unique_ptr<AbstractBlock> BlockModel::copy(const QModelIndex& index) const
{
   if ( !index.isValid() ) return nullptr;
   return pointer(index.parent())->child(index.row())->makeCopy();
}






unique_ptr<AbstractBlock> BlockModel::cut(const QModelIndex& index)
{
   if ( !index.isValid() ) return nullptr;
   AbstractBlock* parent {pointer(index.parent())};
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   unique_ptr<AbstractBlock> ret {parent->takeChild(row)};
   endRemoveRows();
   return ret;
}






const AbstractBlockFactory*BlockModel::factory() const
{
   return _factory;
}






void BlockModel::setRoot(AbstractBlock* root)
{
   beginResetModel();
   _root = root;
   _factory = nullptr;
   if ( _root )
   {
      _factory = &(_root->factory());
      connect(_root,&AbstractBlock::nameChanged,this,&BlockModel::blockNameChanged);
   }
   endResetModel();
}






void BlockModel::blockNameChanged(AbstractBlock* object)
{
   if ( !object->parent() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("A name changed signal was emitted with the root block."));
      throw e;
   }
   QModelIndex index = createIndex(object->parent()->childIndex(object),0,object);
   emit dataChanged(index,index);
}
