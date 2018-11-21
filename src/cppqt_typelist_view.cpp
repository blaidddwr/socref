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
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns a rich text string that displays all type lists this view's type list 
 * block contains as children. 
 *
 * @return Rich text that displays all type lists this view's type list block 
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

   // Add a rich text title. 
   ret += QStringLiteral("<h3>Type Lists</h3>");

   // Add a beginning paragraph tag, then iterate through all type list children and 
   // append their info as rich text, and then add an ending paragraph tag. 
   ret += QStringLiteral("<p>");
   for (auto typeList: list) ret += typeList->name() + QStringLiteral("<br/>");
   ret += QStringLiteral("</p>");

   // Return the rich text string. 
   return ret;
}






/*!
 * Returns a rich text string that displays all type blocks this view's type list 
 * block contains as children. 
 *
 * @return Rich text that displays all type blocks this view's type list block 
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

   // Add a rich text title. 
   ret += QStringLiteral("<h3>Types</h3>");

   // Add a beginning paragraph tag, then iterate through all type children and 
   // append their info as rich text, and then add an ending paragraph. 
   ret += QStringLiteral("<p>");
   for (auto type: list) ret += type->name().replace("<","&lt;") + QStringLiteral("<br/>");
   ret += QStringLiteral("</p>");

   // Return the rich text string. 
   return ret;
}
