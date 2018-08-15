#include "dictionarymodel.h"



//






/*!
 * Implements _QAbstractItemModel_ interface. 
 *
 * @param parent See Qt docs. 
 *
 * @return See Qt docs. 
 */
int DictionaryModel::rowCount(const QModelIndex& parent) const
{
   // If this is the root index then return this model's list's size. 
   if ( !parent.isValid() ) return _list.size();

   // Else this is not the root so return 0. 
   else return 0;
}






/*!
 * Implements _QAbstractItemModel_ interface. 
 *
 * @param index See Qt docs. 
 *
 * @param role See Qt docs. 
 *
 * @return See Qt docs. 
 */
QVariant DictionaryModel::data(const QModelIndex& index, int role) const
{
   // Make sure the given index is valid, the role is display, and the index row is 
   // within range. 
   if ( !index.isValid() || role != Qt::DisplayRole || index.row() >= _list.size() )
   {
      return QVariant();
   }

   // Return the word of this model's list at the given index. 
   else return _list.at(index.row());
}






/*!
 * Constructs a new dictionary model with the optional parent. 
 *
 * @param parent Optional parent for this new dictionary model. 
 */
DictionaryModel::DictionaryModel(QObject* parent):
   QAbstractListModel(parent)
{}






/*!
 * Tests if this model's word list contains the given word, returning true if it 
 * does. 
 *
 * @param word  
 *
 * @return True if this model's word list contains the given word or false 
 *         otherwise. 
 */
bool DictionaryModel::hasWord(const QString& word) const
{
   // Search this model's word list for a match with the given word. 
   return _list.contains(word);
}






/*!
 * Writes out this model's word list to an XML element, returning the XML element 
 * containing all the words. 
 *
 * @param document  
 *
 * @return XML element that contains a copy of this model's word list. 
 */
QDomElement DictionaryModel::write(QDomDocument& document) const
{
   // Create a new XML element. 
   QDomElement ret {document.createElement("na")};

   // Iterate through all words of this model's word list, appending a new XML child 
   // element for each word. Each word is copied in the child element's tag name. 
   for (auto word: _list)
   {
      QDomElement element {document.createElement("word")};
      element.appendChild(document.createTextNode(word));
      ret.appendChild(element);
   }

   // Return the root XML element. 
   return ret;
}






/*!
 * Adds the given word to this model's word list if it does not already contain it, 
 * returning true if the word was added. 
 *
 * @param word  
 *
 * @return True if the given word was added to this model's word list or false if 
 *         the word is already contained in this model. 
 */
bool DictionaryModel::addWord(const QString& word)
{
   // Make sure this model's word list doesn't already contain the given word. 
   if ( hasWord(word) ) return false;

   // Iterate through this model's word list. 
   for (int i = 0; i < _list.size() ;++i)
   {
      // Check to see if the given word is alphabetically less then the word at the 
      // current word list index. 
      if ( word < _list.at(i) )
      {
         // Insert the given word at the current index, notifying the model of a new row 
         // being inserted. 
         beginInsertRows(QModelIndex(),i,i);
         _list.insert(i,word);
         endInsertRows();

         // The new word has been added so notify of modification and return true. 
         emit modified();
         return true;
      }
   }

   // Append the given word to the end of this model's word list, notifying the model 
   // of a new row being inserted. 
   beginInsertRows(QModelIndex(),_list.size(),_list.size());
   _list << word;
   endInsertRows();

   // The new word has been added so notify of modification and return true. 
   emit modified();
   return true;
}






/*!
 * Remove the word at the given index from this model's word list, returning true 
 * if it was removed. 
 *
 * @param index The index of the word that is removed from this model's word list. 
 *
 * @return True if the word at the given index was removed from this model's word 
 *         list or false otherwise. 
 */
bool DictionaryModel::removeWord(const QModelIndex& index)
{
   // Make sure the given index is valid and within range of this model's word list. 
   if ( !index.isValid() || index.row() >= _list.size() ) return false;

   // Remove the word at the given index's row from this model's word list, notifying 
   // the model that a row was removed. 
   int row {index.row()};
   beginRemoveRows(QModelIndex(),row,row);
   _list.removeAt(row);
   endRemoveRows();

   // Notify of modification and return true on success. 
   emit modified();
   return true;
}






/*!
 * Read in a list of words from the given XML element to this model's word list, 
 * overwriting any words it currently contains. This also resets this model. 
 *
 * @param element The XML element whose child element's contain a list of words 
 *                that is read into this model's word list. 
 */
void DictionaryModel::read(const QDomElement& element)
{
   // Notify the model that a reset has begun and then clear this model's word list. 
   beginResetModel();
   _list.clear();

   // Iterate through all children of the given XML element. 
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // If the child node is an element then append its tag name to this model's word 
      // list. 
      if ( node.isElement() ) _list << node.toElement().text();

      // Iterate to the next sibling node. 
      node = node.nextSibling();
   }

   // Notify the model that the reset has finished. 
   endResetModel();
}
