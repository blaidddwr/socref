#include "gui_listedit_model.h"
#include <exception.h>



using namespace Gui;
//






/*!
 * Implements _QAbstractItemModel_ interface. This implementation only returns 
 * header information for the rows. 
 *
 * @param section See Qt docs. 
 *
 * @param orientation See Qt docs. 
 *
 * @param role See Qt docs. 
 *
 * @return See Qt docs. 
 */
QVariant ListEdit::Model::headerData(int section, Qt::Orientation orientation, int role) const
{
   // Make sure the given orientation is vertical and the given role is display. 
   if ( orientation != Qt::Vertical || role != Qt::DisplayRole ) return QVariant();

   // Get a pointer of this object's parent list edit widget and make sure it worked. 
   ListEdit* edit {qobject_cast<ListEdit*>(parent())};
   if ( !edit )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent is not expected type CppQt::Gui::ListDialog"));
      throw e;
   }

   // Return the item title appended with the given section number. 
   return QString(edit->_listItemTitle).append(" #").append(QString::number(section + 1));
}






/*!
 * Implements _QAbstractItemModel_ interface. This implementation returns the 
 * string list size or 0 if its empty. 
 *
 * @param parent See Qt docs. 
 *
 * @return See Qt docs. 
 */
int ListEdit::Model::rowCount(const QModelIndex& parent) const
{
   // If this model has no string list or the given parent is valid then return 0 
   // else return its string list size. 
   if ( !_list || parent.isValid() ) return 0;
   else return _list->size();
}






/*!
 * Implements _QAbstractItemModel_ interface. This implementation returns the 
 * string in it's string list for the display and edit roles. 
 *
 * @param index See Qt docs. 
 *
 * @param role See Qt docs. 
 *
 * @return See Qt docs. 
 */
QVariant ListEdit::Model::data(const QModelIndex& index, int role) const
{
   // Make sure this model has a string list and the given index row is within range. 
   if ( !_list || index.row() < 0 || index.row() >= _list->size() ) return QVariant();

   // If the given role is display or edit then return the string at the given row 
   // else return nothing. 
   if ( role == Qt::DisplayRole || role == Qt::EditRole ) return _list->at(index.row());
   else return QVariant();
}






/*!
 * Implements _QAbstractItemModel_ interface. This implementation sets the string 
 * in this model's string list at the given index. 
 *
 * @param index See Qt docs. 
 *
 * @param value See Qt docs. 
 *
 * @param role See Qt docs. 
 *
 * @return See Qt docs. 
 */
bool ListEdit::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   // Make sure this model has a string list and the given index row is within range. 
   if ( !_list || index.row() < 0 || index.row() >= _list->size() ) return false;

   // Make sure the given role is edit. 
   if ( role == Qt::EditRole )
   {
      // Set the string in this model's string list at the given model index row, signal 
      // the data has changed, and return success. 
      (*_list)[index.row()] = value.toString();
      emit dataChanged(index,index);
      return true;
   }

   // Else this is an unsupported role so return failure. 
   else return false;
}






/*!
 * Constructs a new model with an optional parent. 
 *
 * @param parent Optional parent for this new model. 
 */
ListEdit::Model::Model(QObject* parent):
   QAbstractListModel(parent)
{}






/*!
 * Constructs a new model with the given string list and an optional parent. 
 *
 * @param list Pointer to the string list this new model represents. 
 *
 * @param parent Optional parent for this new model. 
 */
ListEdit::Model::Model(QStringList* list, QObject* parent):
   QAbstractListModel(parent),
   _list(list)
{}






/*!
 * Adds a new string after the given model index, returning true on success. If the 
 * given model index is invalid a new string is added at the beginning of this 
 * model's string list. 
 *
 * @param index The index which has a new string added after it in this model's 
 *              string list. 
 *
 * @return True on success or false otherwise. 
 */
bool ListEdit::Model::addAfter(const QModelIndex& index)
{
   // Make sure this model has a string list and the given index row is within range. 
   if ( !_list || index.row() >= _list->size() ) return false;

   // Determine the row where a new string will be inserted into this model's string 
   // list. 
   int row;
   if ( !index.isValid() ) row = 0;
   else row = index.row() + 1;

   // Insert a new string into this model's string list, notifying this model that 
   // the insertion happened and returning success. 
   beginInsertRows(index.parent(),row,row);
   _list->insert(row,QString());
   endInsertRows();
   return true;
}






/*!
 * Removes a string with the given model index, returning true on success. 
 *
 * @param index The model index of the string that is removed. 
 *
 * @return True on success or false otherwise. 
 */
bool ListEdit::Model::remove(const QModelIndex& index)
{
   // Make sure this model has a string list, the given index is valid, and the given 
   // index row is within range. 
   if ( !_list || !index.isValid() || index.row() >= _list->size() ) return false;

   // Remove the string with the given index row from this model's string list, 
   // notifying this model that a row has been removed and returning success. SPECIAL 
   // NOTE: the given index is destroyed when the begin remove rows qt function is 
   // called. 
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   _list->takeAt(row);
   endRemoveRows();
   return true;
}






/*!
 * Moves the string with the given model index up its model's string list by one, 
 * returning a new model index for the moved string. Moving up means decreasing its 
 * index by one. The given index becomes invalid if the string is moved and should 
 * always be overwritten by the returned index. If this model has no string list 
 * then an exception is thrown. 
 *
 * @param index The model index of the string that is moved. 
 *
 * @return New model index for the string with the given index. 
 */
QModelIndex ListEdit::Model::moveUp(const QModelIndex& index)
{
   // Make sure this model has a string list. 
   if ( !_list )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot move any index of model that has no string list."));
      throw e;
   }

   // Make sure the given index is valid and within range. 
   if ( !index.isValid() || index.row() <= 0 || index.row() >= _list->size() ) return index;

   // Move the string with the given index up this model's string list by one, 
   // notifying this model that a row has been moved. SPECIAL NOTE: the given index 
   // is destroyed when the begin remove rows qt function is called. 
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   std::swap((*_list)[row - 1],(*_list)[row]);
   endMoveRows();

   // Return a new model index for the moved string. 
   return createIndex(row - 1,0);
}






/*!
 * Moves the string with the given model index down its model's string list by one, 
 * returning a new model index for the moved string. Moving down means increasing 
 * its index by one. The given index becomes invalid if the string is moved and 
 * should always be overwritten by the returned index. If this model has no string 
 * list then an exception is thrown. 
 *
 * @param index The model index of the string that is moved. 
 *
 * @return New model index for the string with the given index. 
 */
QModelIndex ListEdit::Model::moveDown(const QModelIndex& index)
{
   // Make sure this model has a string list. 
   if ( !_list )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot move any index of model that has no string list."));
      throw e;
   }

   // Make sure the given index is valid and within range. 
   if ( !index.isValid() || index.row() < 0 || index.row() == (_list->size() - 1) ) return index;

   // Move the string with the given index down this model's string list by one, 
   // notifying this model that a row has been moved. SPECIAL NOTE: the given index 
   // is destroyed when the begin remove rows qt function is called. 
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   std::swap((*_list)[row],(*_list)[row + 1]);
   endMoveRows();

   // Return a new model index for the moved string. 
   return createIndex(row + 1,0);
}






/*!
 * Sets the string list for this model. This model does not take ownership of the 
 * given string list and must not be deleted as long as it is part of this model. A 
 * null pointer can be passed to set this model to a null state. 
 *
 * @param list Pointer to the string list this model is set to or null to set this 
 *             model to a null state. 
 */
void ListEdit::Model::setList(QStringList* list)
{
   // Set this model's string list pointer to the new value given, notifying this 
   // model that a reset of the entire model has occurred. 
   beginResetModel();
   _list = list;
   endResetModel();
}
