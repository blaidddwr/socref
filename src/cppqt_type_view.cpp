#include "cppqt_type_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new type view object with the given type block. 
 *
 * @param block Type block this new view displays. 
 */
Type::View::View(const Type* block):
   _block(block)
{}






/*!
 * Returns the HTML rich text that displays the body of this view's parent type 
 * block. 
 *
 * @return HTML rich text that displays the body of this view's parent type block. 
 */
QString Type::View::displayText()
{
   return _block->name().replace(",","&lt;");
}
