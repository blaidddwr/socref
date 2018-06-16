#include "gui_listedit_model.h"
#include <exception.h>



using namespace Gui;
//






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
 * Implements the qt model interface that returns header data for the given 
 * section, orientation, and role. This implementation only returns header 
 * information for the rows. 
 *
 * @param section The section number that header data is returned for. This is 
 *                either the row or column index. 
 *
 * @param orientation The orientation that header data is returned for. This is 
 *                    either vertical(rows) or horizontal(columns). 
 *
 * @param role The role of data that is returned. 
 *
 * @return Header value if this is a row else an empty variant. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given orientation is not vertical or the role is not the display role 
 *    then return an empty variant. 
 *
 * 2. Get a pointer _edit_ of this object's parent list edit widget. If casting 
 *    this object's parent as a list edit widget fails then throw an exception. 
 *
 * 3. Return a string title that is the list item title for _edit_ appended with 
 *    the row's index plus one. 
 */
QVariant ListEdit::Model::headerData(int section, Qt::Orientation orientation, int role) const
{
   // 1
   if ( orientation != Qt::Vertical || role != Qt::DisplayRole ) return QVariant();

   // 2
   ListEdit* edit {qobject_cast<ListEdit*>(parent())};
   if ( !edit )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent is not expected type CppQt::Gui::ListDialog"));
      throw e;
   }

   // 3
   return QString(edit->_listItemTitle).append(" #").append(QString::number(section + 1));
}






/*!
 * Implements the qt model interface which returns the row count for the given 
 * model index. This implementation simply returns the string list size or 0 if its 
 * empty. 
 *
 * @param parent The unused parent model index. This is not used because this model 
 *               is simple and has no layers. 
 *
 * @return This model's string list size or 0 if it has no string list. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list then return 0, else return its string list 
 *    size. 
 */
int ListEdit::Model::rowCount(const QModelIndex& parent) const
{
   // 1
   if ( !_list || parent.isValid() ) return 0;
   else return _list->size();
}






/*!
 * Implements the qt model interface that returns data about a given model index 
 * with the given role. This implementation returns the string in it's string list. 
 *
 * @param index Model index whose data with the given role is returned. 
 *
 * @param role The role of the data that is returned. 
 *
 * @return String of this model's string list at the given model index row or an 
 *         empty variant of this model has no string list. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list, the given index row is out of range, or the 
 *    given role is not the display or edit role then return an empty variant, else 
 *    return the string of this model's string list at the given model index row. 
 */
QVariant ListEdit::Model::data(const QModelIndex& index, int role) const
{
   // 1
   if ( !_list || index.row() < 0 || index.row() >= _list->size() )
   {
      return QVariant();
   }
   if ( role == Qt::DisplayRole || role == Qt::EditRole )
   {
      return _list->at(index.row());
   }
   else return QVariant();
}






/*!
 * Implements the qt model interface that sets data for a given model index with 
 * the given role. This implementation sets the strings in this model's string 
 * list. 
 *
 * @param index Model index whose data is set if the given role is edit. 
 *
 * @param value The new string value that the string with the given model index row 
 *              is set to if the given role is edit. 
 *
 * @param role This must be the edit role for this to set the model index data. 
 *
 * @return True if the string at the given model index row was set or false 
 *         otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list, the given model index row is out of range, 
 *    or the given role is not the edit role then return false. 
 *
 * 2. Set the string in this model's string list at the given model index row, emit 
 *    the data changed signal for the given index, and return true. 
 */
bool ListEdit::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   // 1
   if ( !_list || index.row() < 0 || index.row() >= _list->size() || role != Qt::EditRole )
   {
      return false;
   }

   // 2
   (*_list)[index.row()] = value.toString();
   emit dataChanged(index,index);
   return true;
}






/*!
 * Adds a new string after the given model index, returning true on success. 
 *
 * @param index The index which has a new string added after it in this model's 
 *              string list. 
 *
 * @return True on success or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list or the given index row is out of range then 
 *    return false. 
 *
 * 2. Create the integer _row_, setting it to 0 if the given index is invalid else 
 *    setting it to the given index row plus one. 
 *
 * 3. Signal that a row insertion is happening to this model, then insert a new 
 *    empty string at _row_, then signal the row insertion is finished to this 
 *    model, and then return true. 
 */
bool ListEdit::Model::addAfter(const QModelIndex& index)
{
   // 1
   if ( !_list || index.row() >= _list->size() ) return false;

   // 2
   int row;
   if ( !index.isValid() ) row = 0;
   else row = index.row() + 1;

   // 3
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list, the given index is not valid, or the given 
 *    index row is out of range then return false. 
 *
 * 2. Remove the string with the given index row from this model's string list, 
 *    signaling to this model that a row has been removed. 
 *
 * 3. Return true. 
 */
bool ListEdit::Model::remove(const QModelIndex& index)
{
   // 1
   if ( !_list || !index.isValid() || index.row() >= _list->size() ) return false;

   // 2
   int row {index.row()};
   beginRemoveRows(index.parent(),row,row);
   _list->takeAt(row);
   endRemoveRows();

   // 3
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list then throw an exception. 
 *
 * 2. If the given index is not valid or it is out of range then return the given 
 *    index. 
 *
 * 3. Move the string with the given index up this model's string list by one by 
 *    decreasing its index, signaling to this model that a row has been moved. 
 *
 * 4. Return a new model index for the moved string. 
 */
QModelIndex ListEdit::Model::moveUp(const QModelIndex& index)
{
   // 1
   if ( !_list )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot move any index of model that has no string list."));
      throw e;
   }

   // 2
   if ( !index.isValid() || index.row() <= 0 || index.row() >= _list->size() ) return index;

   // 3
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row - 1);
   std::swap((*_list)[row - 1],(*_list)[row]);
   endMoveRows();

   // 4
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this model has no string list then throw an exception. 
 *
 * 2. If the given index is not valid or it is out of range then return the given 
 *    index. 
 *
 * 3. Move the string with the given index down this model's string list by one by 
 *    increasing its index, signaling to this model that a row has been moved. 
 *
 * 4. Return a new model index for the moved string. 
 */
QModelIndex ListEdit::Model::moveDown(const QModelIndex& index)
{
   // 1
   if ( !_list )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot move any index of model that has no string list."));
      throw e;
   }

   // 2
   if ( !index.isValid() || index.row() < 0 || index.row() == (_list->size() - 1) ) return index;

   // 3
   int row {index.row()};
   beginMoveRows(index.parent(),row,row,index.parent(),row + 2);
   std::swap((*_list)[row],(*_list)[row + 1]);
   endMoveRows();

   // 4
   return createIndex(row + 1,0);
}






/*!
 * Sets the string list for this model. This model does not take ownership of the 
 * given string list and must not be deleted as long as it is part of this model. A 
 * null pointer can be passed to set this model to a null state. 
 *
 * @param list Pointer to the string list this model is set to or null to set this 
 *             model to a null state. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this model's string list pointer to the given one, signaling to this 
 *    model that a reset of the entire model has occurred. 
 */
void ListEdit::Model::setList(QStringList* list)
{
   // 1
   beginResetModel();
   _list = list;
   endResetModel();
}
