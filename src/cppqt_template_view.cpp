#include "cppqt_template_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given template block. 
 *
 * @param block Template block this new view displays. 
 */
Template::View::View(const Template* block):
   Base::View(block)
{
   // Only display the base description field for this block type. 
   setText(displayDescription());
}
