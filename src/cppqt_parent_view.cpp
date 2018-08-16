#include "cppqt_parent_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view widget with the given parent block. 
 *
 * @param block Parent block this new widget displays. 
 */
Parent::View::View(const Parent* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's parent block. 
 *
 * @return HTML rich text that displays the body of this view's parent block. 
 */
QString Parent::View::displayText()
{
   // Return a HTML string displayed the access type and class name of this view's 
   // parent block. 
   return QString("<b>%1</b> %2")
          .arg(_block->accessString())
          .arg(_block->className().replace("<","&lt;"));
}
