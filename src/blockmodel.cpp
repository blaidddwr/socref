#include "blockmodel.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
#include "exception.h"






BlockModel::BlockModel(AbstractBlock* root, QObject* parent):
   QAbstractItemModel(parent),
   _root(root)
{
   if ( _root )
   {
      _factory = &(_root->getFactory());
      connect(_root,&AbstractBlock::nameChanged,this,&BlockModel::blockNameChanged);
   }
}






QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   if ( row < 0 || column != 0 )
   {
      return QModelIndex();
   }
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row >= parent_->getChildrenSize() )
   {
      return QModelIndex();
   }
   return createIndex(row,column,parent_->getChild(row));
}






QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   AbstractBlock* parent = getPointer(child)->getParent();
   if ( !parent || !parent->getParent() )
   {
      return QModelIndex();
   }
   AbstractBlock* grandparent = parent->getParent();
   return createIndex(grandparent->getChildIndex(parent),0,parent);
}






int BlockModel::rowCount(const QModelIndex& parent) const
{
   if ( !_root )
   {
      return 0;
   }
   return getPointer(parent)->getChildrenSize();
}






AbstractBlock *BlockModel::getPointer(const QModelIndex& index) const
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
      return QVariant(getPointer(index)->getName());
   case Qt::DecorationRole:
      return QVariant(_factory->getIcon(getPointer(index)->getType()));
   default:
      return QVariant();
   }
}






bool BlockModel::insertRow(int row, const QModelIndex& parent, AbstractBlock* takenObject)
{
   if ( !takenObject )
   {
      return false;
   }
   beginInsertRows(parent,row,row);
   getPointer(parent)->insertChild(row,takenObject);
   endInsertRows();
   return true;
}






bool BlockModel::moveRow(int source, int destination, const QModelIndex& parent)
{
   AbstractBlock* parent_ {getPointer(parent)};
   if ( source < 0 || source >= parent_->getChildrenSize() )
   {
      return false;
   }
   if ( destination < 0 || destination > parent_->getChildrenSize() )
   {
      return false;
   }
   beginMoveRows(parent,source,source,parent,destination);
   AbstractBlock* child {parent_->takeChild(source)};
   if ( destination > ( source + 1 ) )
   {
      --destination;
   }
   parent_->insertChild(destination,child);
   endMoveRows();
   return true;
}






bool BlockModel::removeRow(int row, const QModelIndex& parent)
{
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return false;
   }
   beginRemoveRows(parent,row,row);
   parent_->removeChild(row);
   endRemoveRows();
   return true;
}






AbstractBlock* BlockModel::copyRow(int row, const QModelIndex& parent) const
{
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return nullptr;
   }
   return parent_->getChild(row)->makeCopy();
}






AbstractBlock* BlockModel::cutRow(int row, const QModelIndex& parent)
{
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return nullptr;
   }
   beginRemoveRows(parent,row,row);
   AbstractBlock* ret {parent_->takeChild(row)};
   endRemoveRows();
   return ret;
}






void BlockModel::setRoot(AbstractBlock* root)
{
   beginResetModel();
   _root = root;
   if ( _root )
   {
      _factory = &(_root->getFactory());
      connect(_root,&AbstractBlock::nameChanged,this,&BlockModel::blockNameChanged);
   }
   endResetModel();
}






void BlockModel::blockNameChanged(AbstractBlock* object)
{
   if ( !object->getParent() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("A name changed signal was emitted with the root block."));
      throw e;
   }
   QModelIndex index = createIndex(object->getParent()->getChildIndex(object),0,object);
   emit dataChanged(index,index);
}
