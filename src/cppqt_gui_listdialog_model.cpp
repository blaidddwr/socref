#include <exception.h>
#include "cppqt_gui_listdialog_model.h"



using namespace CppQt::Gui;






ListDialog::Model::Model(QStringList* list, QObject* parent):
   QAbstractListModel(parent),
   _list(list)
{}






QVariant ListDialog::Model::headerData(int section, Qt::Orientation orientation, int role) const
{
   if ( orientation == Qt::Vertical && role == Qt::DisplayRole )
   {
      ListDialog* dialog {qobject_cast<ListDialog*>(parent())};
      if ( !dialog )
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Parent is not expected type CppQt::Gui::ListDialog"));
         throw e;
      }
      return QString(dialog->_listItemTitle).append(" #").append(QString::number(section + 1));
   }
   return QVariant();
}






int ListDialog::Model::rowCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   if ( !_list ) return 0;
   return _list->size();
}






QVariant ListDialog::Model::data(const QModelIndex& index, int role) const
{
   if ( index.row() < _list->size() && ( role == Qt::DisplayRole || role == Qt::EditRole ) ) return _list->at(index.row());
   return QVariant();
}






bool ListDialog::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   if ( index.row() < _list->size() && role == Qt::EditRole )
   {
      (*_list)[index.row()] = value.toString();
      dataChanged(index,index);
      return true;
   }
   return false;
}






bool ListDialog::Model::addAfter(const QModelIndex& index)
{
   int row;
   if ( !index.isValid() ) row = 0;
   else row = index.row() + 1;
   beginInsertRows(index.parent(),row,row);
   _list->insert(row,QString());
   endInsertRows();
   return true;
}






bool ListDialog::Model::remove(const QModelIndex& index)
{
   if ( !index.isValid() ) return false;
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   _list->takeAt(row);
   endRemoveRows();
   return true;
}






QModelIndex ListDialog::Model::moveUp(const QModelIndex& index)
{
   if ( !index.isValid() || index.row() == 0 ) return index;
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   std::swap((*_list)[row - 1],(*_list)[row]);
   endMoveRows();
   return createIndex(row - 1,0);
}






QModelIndex ListDialog::Model::moveDown(const QModelIndex& index)
{
   if ( !index.isValid() || index.row() == (_list->size() - 1) ) return index;
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   std::swap((*_list)[row],(*_list)[row + 1]);
   endMoveRows();
   return createIndex(row + 1,0);
}






void ListDialog::Model::setStringList(QStringList* list)
{
   beginResetModel();
   _list = list;
   endResetModel();
}
