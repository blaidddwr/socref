#include "blockmodel.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "exception.h"



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
   if ( row < 0 || column != 0 )
   {
      return QModelIndex();
   }
   AbstractBlock* parent_ {pointer(parent)};
   if ( row >= parent_->childrenSize() )
   {
      return QModelIndex();
   }
   return createIndex(row,column,parent_->child(row));
}






QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   AbstractBlock* parent = pointer(child)->parent();
   if ( !parent || !parent->parent() )
   {
      return QModelIndex();
   }
   AbstractBlock* grandparent = parent->parent();
   return createIndex(grandparent->childIndex(parent),0,parent);
}






int BlockModel::rowCount(const QModelIndex& parent) const
{
   if ( !_root )
   {
      return 0;
   }
   return pointer(parent)->childrenSize();
}






AbstractBlock* BlockModel::pointer(const QModelIndex& index) const
{
   AbstractBlock* ret;
   if ( index.isValid() )
   {
      ret = reinterpret_cast<AbstractBlock*>(index.internalPointer());
   }
   else
   {
      ret = _root;
   }
   return ret;
}






QVariant BlockModel::data(const QModelIndex& index, int role) const
{
   switch (role)
   {
   case Qt::DisplayRole:
      return QVariant(pointer(index)->name());
   case Qt::DecorationRole:
      return QVariant(_factory->icon(pointer(index)->type()));
   default:
      return QVariant();
   }
}






bool BlockModel::insertRow(int row, const QModelIndex& parent, unique_ptr<AbstractBlock>&& object)
{
   if ( !object )
   {
      return false;
   }
   beginInsertRows(parent,row,row);
   pointer(parent)->insertChild(row,std::move(object));
   endInsertRows();
   return true;
}






bool BlockModel::moveRow(int source, int destination, const QModelIndex& parent)
{
   AbstractBlock* parent_ {pointer(parent)};
   if ( source < 0 || source >= parent_->childrenSize() )
   {
      return false;
   }
   if ( destination < 0 || destination > parent_->childrenSize() )
   {
      return false;
   }
   beginMoveRows(parent,source,source,parent,destination);
   unique_ptr<AbstractBlock> child {parent_->takeChild(source)};
   if ( destination > ( source + 1 ) )
   {
      --destination;
   }
   parent_->insertChild(destination,std::move(child));
   endMoveRows();
   return true;
}






bool BlockModel::removeRow(int row, const QModelIndex& parent)
{
   AbstractBlock* parent_ {pointer(parent)};
   if ( row < 0 || row >= parent_->childrenSize() )
   {
      return false;
   }
   beginRemoveRows(parent,row,row);
   parent_->removeChild(row);
   endRemoveRows();
   return true;
}






unique_ptr<AbstractBlock> BlockModel::copyRow(int row, const QModelIndex& parent) const
{
   AbstractBlock* parent_ {pointer(parent)};
   if ( row < 0 || row >= parent_->childrenSize() )
   {
      return nullptr;
   }
   return parent_->child(row)->makeCopy();
}






unique_ptr<AbstractBlock> BlockModel::cutRow(int row, const QModelIndex& parent)
{
   AbstractBlock* parent_ {pointer(parent)};
   if ( row < 0 || row >= parent_->childrenSize() )
   {
      return nullptr;
   }
   beginRemoveRows(parent,row,row);
   unique_ptr<AbstractBlock> ret {parent_->takeChild(row)};
   endRemoveRows();
   return ret;
}






void BlockModel::setRoot(AbstractBlock* root)
{
   beginResetModel();
   _root = root;
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
