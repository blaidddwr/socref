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
 * Returns the HTML rich text that displays the body of this view's type list 
 * block. 
 *
 * @return HTML rich text that displays the body of this view's type list block. 
 */
QString TypeList::View::displayText()
{
   // Return a HTML string of the description, type lists, and types in that order. 
   return displayDescription().append(displayTypeLists()).append(displayTypes());
}






/*!
 * Returns a HTML string that displays all type lists this view's type list block 
 * contains as children. 
 *
 * @return HTML string that displays all type lists this view's type list block 
 *         contains. 
 */
QString TypeList::View::displayTypeLists()
{
   // Create an empty return string. 
   QString ret;

   // Create a pointer list of all type list children of this view's type list block 
   // and make sure it is not empty. 
   const QList<CppQt::TypeList*> list
   {
      _block->makeListOfType<CppQt::TypeList>(BlockFactory::TypeListType)
   };
   if ( list.isEmpty() ) return ret;

   // Add a HTML title and beginning paragraph tag, then iterate through all type 
   // list children and append their info as HTML, and then add a HTML ending 
   // paragraph tag. 
   ret.append(tr("<h3>Type Lists</h3><p>"));
   for (auto typeList: list) ret.append(typeList->name()).append("<br/>");
   ret.append("</p>");

   // Return the HTML string. 
   return ret;
}






/*!
 * Returns a HTML string that displays all type blocks this view's type list block 
 * contains as children. 
 *
 * @return HTML string that displays all type blocks this view's type list block 
 *         contains. 
 */
QString TypeList::View::displayTypes()
{
   // Create an empty return string. 
   QString ret;

   // Create a pointer list of all type children of this view's type list block and 
   // make sure it is not empty. 
   const QList<Type*> list {_block->makeListOfType<Type>(BlockFactory::TypeType)};
   if ( list.isEmpty() ) return ret;

   // Add a HTML title, then iterate through all type children and append their info 
   // as HTML, and then add a HTML ending paragraph. 
   ret.append(tr("<h3>Types</h3><p>"));
   for (auto type: list) ret.append(type->name().replace("<","&lt;")).append("<br/>");
   ret.append("</p>");

   // Return the HTML string. 
   return ret;
}
