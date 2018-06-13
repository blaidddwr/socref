#include "cppqt_typelist_view.h"
#include "cppqt_type.h"
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given type list block. 
 *
 * @param block Type list block this new view displays. 
 */
TypeList::View::View(const TypeList* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's parent type 
 * list block. 
 *
 * @return HTML rich text that displays the body of this view's parent type list 
 *         block. 
 */
QString TypeList::View::displayText()
{
   return displayDescription().append(displayTypeLists()).append(displayTypes());
}






/*!
 * Returns a HTML string that displays all type lists this view's parent type list 
 * block contains as children. 
 *
 * @return HTML string that displays all type lists this view's parent type list 
 *         block contains. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an empty string _ret_ and get a pointer list of this view's type list 
 *    block's child type list blocks saving it to _list_. If _list_ is empty then 
 *    return _ret_. 
 *
 * 2. Add a HTML title to _ret_ and then add every child type list block in _list_ 
 *    to _ret_ using its name. 
 *
 * 3. Return _ret_. 
 */
QString TypeList::View::displayTypeLists()
{
   // 1
   QString ret;
   const QList<CppQt::TypeList*> list
   {
      _block->makeListOfType<CppQt::TypeList>(BlockFactory::TypeListType)
   };
   if ( list.isEmpty() ) return ret;

   // 2
   ret.append("<h3>").append(tr("Type Lists")).append("</h3><p>");
   for (auto typeList: list) ret.append(typeList->name()).append("<br/>");
   ret.append("</p>");

   // 3
   return ret;
}






/*!
 * Returns a HTML string that displays all type blocks this view's parent type list 
 * block contains as children. 
 *
 * @return HTML string that displays all type blocks this view's parent type list 
 *         block contains. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an empty string _ret_ and get a pointer list of this view's type list 
 *    block's child type blocks saving it to _list_. If _list_ is empty then return 
 *    _ret_. 
 *
 * 2. Add a HTML title to _ret_ and then add every child type block in _list_ to 
 *    _ret_ using its name. Replace all opening carrot characters in the child type 
 *    block names with HTML safe code. 
 *
 * 3. Return _ret_. 
 */
QString TypeList::View::displayTypes()
{
   // 1
   QString ret;
   const QList<Type*> list {_block->makeListOfType<Type>(BlockFactory::TypeType)};
   if ( list.isEmpty() ) return ret;

   // 2
   ret.append("<h3>").append(tr("Types")).append("</h3><p>");
   for (auto type: list) ret.append(type->name().replace("<","&lt;")).append("<br/>");
   ret.append("</p>");

   // 3
   return ret;
}
