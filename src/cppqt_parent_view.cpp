#include "cppqt_parent_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given variable block. 
 *
 * @param block Variable block this new view displays. 
 */
Parent::View::View(const Parent* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's variable block. 
 *
 * @return HTML rich text that displays the body of this view's variable block. 
 */
QString Parent::View::displayText()
{
   return QString("<b>").append(_block->accessString())
                        .append("</b> ")
                        .append(_block->className().replace("<","&lt;"));
}
