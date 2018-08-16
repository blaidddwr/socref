#include "cppqt_using_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view widget with the given using block. 
 *
 * @param block Using block this new view displays. 
 */
Using::View::View(const Using* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's using block. 
 *
 * @return HTML rich text that displays the body of this view's using block. 
 */
QString Using::View::displayText()
{
   // Return a HTML string that displays the using declaration string following by 
   // this view's using block's field. 
   return _block->name().append(" ").append(_block->fieldName().replace("<","&lt;"));
}
