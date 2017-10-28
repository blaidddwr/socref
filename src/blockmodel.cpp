#include "blockmodel.h"
#include "abstractblock.h"
#include "abstractblockfactory.h"
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
   // make sure row and column are valid
   if ( row < 0 || column != 0 )
   {
      return QModelIndex();
   }

   // get pointer and make sure row is not beyond size of children
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row >= parent_->getChildrenSize() )
   {
      return QModelIndex();
   }

   // return new index
   return createIndex(row,column,parent_->getChild(row));
}






//@@
QModelIndex BlockModel::parent(const QModelIndex& child) const
{
   // get parent pointer
   AbstractBlock* parent = getPointer(child)->getParent();

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

   // get pointer and return the parent's amount of children
   return getPointer(parent)->getChildrenSize();
}






//@@
AbstractBlock *BlockModel::getPointer(const QModelIndex& index) const
{
   // if index is valid get pointer from it
   AbstractBlock* ret;
   if ( index.isValid() )
   {
      ret = reinterpret_cast<AbstractBlock*>(index.internalPointer());
   }

   // else use root pointer
   else
   {
      ret = _root;
   }

   // return pointer
   return ret;
}






//@@
QVariant BlockModel::data(const QModelIndex& index, int role) const
{
   // determine which role is requested and return information
   switch (role)
   {
   case Qt::DisplayRole:
      return QVariant(getPointer(index)->getName());
   case Qt::DecorationRole:
      return QVariant(_factory->getIcon(getPointer(index)->getType()));
   default:
      // unsupported type of data so return nothing
      return QVariant();
   }
}






//@@
bool BlockModel::insertRow(int row, const QModelIndex& parent, AbstractBlock* o_object)
{
   // make sure object is valid pointer
   if ( !o_object )
   {
      return false;
   }

   // begin insert operation and insert object pointer at given row
   beginInsertRows(parent,row,row);
   getPointer(parent)->insertChild(row,o_object);

   // end insert operation and return success
   endInsertRows();
   return true;
}






//@@
bool BlockModel::moveRow(int source, int destination, const QModelIndex& parent)
{
   // get pointer from parent and make sure row to move exists
   AbstractBlock* parent_ {getPointer(parent)};
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
bool BlockModel::removeRow(int row, const QModelIndex& parent)
{
   // get pointer from parent and make sure rows to remove exist
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return false;
   }

   // begin remove operation and remove all children starting at given row
   beginRemoveRows(parent,row,row);
   parent_->removeChild(row);

   // end remove operation and return success
   endRemoveRows();
   return true;
}






//@@
AbstractBlock* BlockModel::copyRow(int row, const QModelIndex& parent) const
{
   // get pointer from parent and make sure row to copy exists
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return nullptr;
   }

   // return copy of row
   return parent_->getChild(row)->makeCopy();
}






//@@
AbstractBlock* BlockModel::cutRow(int row, const QModelIndex& parent)
{
   // get pointer from parent and make sure row to copy exists
   AbstractBlock* parent_ {getPointer(parent)};
   if ( row < 0 || row >= parent_->getChildrenSize() )
   {
      return nullptr;
   }

   // begin remove operation and take child
   beginRemoveRows(parent,row,row);
   AbstractBlock* ret {parent_->takeChild(row)};

   // end remove operation and return orphaned child
   endRemoveRows();
   return ret;
}






//@@
void BlockModel::setRoot(AbstractBlock* root)
{
   // begin model reset and set new root
   beginResetModel();
   _root = root;

   // if root is not null set factory and end model reset
   if ( _root )
   {
      _factory = &(_root->getFactory());
   }
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
