#include "cppqt_using_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given variable block. 
 *
 * @param block Variable block this new view displays. 
 */
Using::View::View(const Using* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's variable block. 
 *
 * @return HTML rich text that displays the body of this view's variable block. 
 */
QString Using::View::displayText()
{
   return _block->name().append(" ").append(_block->fieldName().replace("<","&lt;"));
}
