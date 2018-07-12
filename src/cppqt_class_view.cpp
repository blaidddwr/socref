#include "cppqt_class_view.h"
#include "cppqt_parent.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given class block. 
 *
 * @param block Class block this new view displays. 
 */
Class::View::View(const Class* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's class block. 
 *
 * @return HTML rich text that displays the body of this view's class block. 
 */
QString Class::View::displayText()
{
   // Return as HTML this view's class block's description, qt object property, 
   // templates, and inheritance in that order. 
   return displayDescription().append(displayQtObject())
                              .append(displayTemplates())
                              .append(displayParents());
}






/*!
 * Returns a HTML string that displays the sole qt object property of this view's 
 * class block. 
 *
 * @return HTML string that displays the qt object property of this view's class 
 *         block. 
 */
QString Class::View::displayQtObject()
{
   // Return as HTML qt object property state of this view's class block. 
   return tr("<p><b>Qt Object</b> : %1</p>").arg(_block->isQtObject() ? "Yes" : "No");
}






/*!
 * Returns a HTML string that displays all inherited parent classes of this view's 
 * class block. If this view's class block does not inherit any classes then this 
 * returns an empty string. 
 *
 * @return HTML string that displays the inheritance of this view's class block or 
 *         an empty string if it has no inheritance. 
 */
QString Class::View::displayParents()
{
   // Create an empty return string. 
   QString ret;

   // Make a pointer list of child parent blocks this view's class block contains and 
   // make sure it is not empty. 
   const QList<Parent*> list {_block->parents()};
   if ( list.isEmpty() ) return ret;

   // Append an HTML title. 
   ret.append("<h3>Inheritance</h3>");

   // Iterate through all of the parent block pointer list and append each one as 
   // HTML. 
   for (auto parent : list)
   {
      ret.append(tr("<p><b>%1</b> %2</p>")
                 .arg(parent->accessString())
                 .arg(parent->className().replace("<","&lt;")));
   }

   // Return the inheritance HTML string. 
   return ret;
}
