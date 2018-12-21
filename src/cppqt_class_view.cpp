#include "cppqt_class_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given class block. 
 *
 * @param block Class block this new view displays. 
 */
Class::View::View(const Class* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays the sole qt object property of this view's class 
 * block. 
 *
 * @return Rich text that displays the qt object property of this view's class 
 *         block. 
 */
QString Class::View::displayQtObject()
{
   // Return qt object property state of this view's class block as rich text. 
   return tr("<p><b>Qt Object</b> : %1</p>").arg(_block->isQtObject() ? "Yes" : "No");
}






/*!
 * Returns rich text that displays all inherited parent classes of this view's 
 * class block. If this view's class block does not inherit any classes then this 
 * returns an empty string. 
 *
 * @return Rich text that displays the inheritance of this view's class block or an 
 *         empty string if it has no inheritance. 
 */
QString Class::View::displayParents()
{
   // Create an empty return string. 
   QString ret;

   // Make a pointer list of child parent blocks this view's class block contains and 
   // make sure it is not empty. 
   const QList<Parent*> list {_block->parents()};
   if ( list.isEmpty() ) return ret;

   // Append a rich text title. 
   ret += QStringLiteral("<h3>Inheritance</h3>");

   // Iterate through all of the parent block pointer list and append each one as 
   // rich text. 
   for (auto parent : list)
   {
      ret += QStringLiteral("<p><b>%1</b> %2</p>");
           //+ parent->accessString()
           //+ parent->className().replace("<","&lt;");
   }

   // Return the inheritance rich text. 
   return ret;
}
