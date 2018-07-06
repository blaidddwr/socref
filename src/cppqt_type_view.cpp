#include "cppqt_type_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given type block. 
 *
 * @param block Type block this new view displays. 
 */
Type::View::View(const Type* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's type block. 
 *
 * @return HTML rich text that displays the body of this view's type block. 
 */
QString Type::View::displayText()
{
   // Return this blocks name, making it HTML safe. 
   return _block->name().replace("<","&lt;");
}
