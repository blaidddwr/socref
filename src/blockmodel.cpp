#include "blockmodel.h"
#include "abstractblock.h"
#include "exception.h"






//@@
BlockModel::BlockModel(AbstractBlock* root, QObject* parent):
   QAbstractItemModel(parent),
   _root(root)
{
   // if root is not null set factory
   if ( _root )
   {
      _factory = &(_root->getFactory());
   }
}






//@@
QModelIndex BlockModel::index(int row, int column, const QModelIndex& parent) const
{
   // if parent is valid get pointer from it
   AbstractBlock* parent_;
   if ( parent.isValid() )
   {
      parent_ = reinterpret_cast<AbstractBlock*>(parent.internalPointer());
   }

   // else use root as pointer
   else
   {
      parent_ = _root;
   }

   // return index of child with given row
   return createIndex(row,column,parent_->getChild(row));
}






//@@
QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   // get parent pointer
   AbstractBlock* parent = reinterpret_cast<AbstractBlock*>(child.internalPointer())->getParent();

   // if child has no parent or no grand parent then return invalid index
   if ( !parent || !parent->getParent() )
   {
      return QModelIndex();
   }

   // get grand parent pointer and return index of parent
   AbstractBlock* grandparent = parent->getParent();
   return createIndex(grandparent->getChildIndex(parent),0,parent);
}






//@@
int BlockModel::rowCount(const QModelIndex& parent) const
{
   // return 0 if this model has no data
   if ( !_root )
   {
      return 0;
   }

   // if parent is valid get pointer from it
   AbstractBlock* parent_;
   if ( parent.isValid() )
   {
      parent_ = reinterpret_cast<AbstractBlock*>(parent.internalPointer());
   }

   // else use root as pointer
   else
   {
      parent_ = _root;
   }

   // return the parent's amount of children
   return parent_->getChildrenSize();
}






//@@
QVariant BlockModel::data(const QModelIndex& index, int role) const
{
   // if the role is not display return empty variant
   if ( role != Qt::DisplayRole )
   {
      return QVariant();
   }

   // get pointer and return title
   return QVariant(reinterpret_cast<AbstractBlock*>(index.internalPointer())->getName());
}






//@@
bool BlockModel::insertRow(int row, const QModelIndex& parent, AbstractBlock* object)
{
   // make sure object is valid pointer
   if ( !object )
   {
      return false;
   }

   // if parent is valid get pointer from it
   AbstractBlock* parent_;
   if ( parent.isValid() )
   {
      parent_ = reinterpret_cast<AbstractBlock*>(parent.internalPointer());
   }

   // else use root as pointer
   else
   {
      parent_ = _root;
   }

   // begin insert operation and insert object pointer at given row
   beginInsertRows(parent,row,row);
   parent_->insertChild(row,object);

   // end insert operation and return success
   endInsertRows();
   return true;
}






//@@
bool BlockModel::moveRow(int source, int destination, const QModelIndex& parent)
{
   // if parent is valid get pointer from it
   AbstractBlock* parent_;
   if ( parent.isValid() )
   {
      parent_ = reinterpret_cast<AbstractBlock*>(parent.internalPointer());
   }

   // else use root pointer
   else
   {
      parent_ = _root;
   }

   // make sure row to move exists
   if ( source < 0 || source >= parent_->getChildrenSize() )
   {
      return false;
   }

   // remove child from current row position
   beginRemoveRows(parent,source,source);
   AbstractBlock* child {parent_->takeChild(source)};
   endRemoveRows();

   // decrement destination by one if it is at or after source
   if ( destination >= source )
   {
      --destination;
   }

   // insert child into new row position and return success
   beginInsertRows(parent,destination,destination);
   parent_->insertChild(destination,child);
   endInsertRows();
   return true;
}






//@@
bool BlockModel::removeRows(int row, int count, const QModelIndex& parent)
{
   // if parent is valid get pointer from it
   AbstractBlock* parent_;
   if ( parent.isValid() )
   {
      parent_ = reinterpret_cast<AbstractBlock*>(parent.internalPointer());
   }

   // else use root pointer
   else
   {
      parent_ = _root;
   }

   // make sure rows to remove exist
   if ( row < 0 || (row + count) > parent_->getChildrenSize() )
   {
      return false;
   }

   // begin remove operation and remove all children starting at given row
   beginRemoveRows(parent,row,row+count-1);
   for (int i = 0; i < count ;++i)
   {
      parent_->removeChild(row);
   }

   // end remove operation and return success
   endRemoveRows();
   return true;
}






//@@
void BlockModel::setRoot(AbstractBlock* root)
{
   // set new root and reset model
   beginResetModel();
   _root = root;
   endResetModel();
}






//@@
void BlockModel::blockNameChanged(AbstractBlock* object)
{
   // make sure that emitted signal is not the root
   if ( !object->getParent() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("A name changed signal was emitted with the root block."));
      throw e;
   }

   // create index of block that emitted signal and emit data changed signal
   QModelIndex index = createIndex(object->getParent()->getChildIndex(object),0,object);
   emit dataChanged(index,index);
}
