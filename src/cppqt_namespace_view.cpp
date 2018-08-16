#include "cppqt_namespace_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given namespace block. 
 *
 * @param block Namespace block this new view displays. 
 */
Namespace::View::View(const Namespace* block):
   Base::View(block)
{
   // Display the base description for this view's namespace block and nothing else. 
   setText(displayDescription());
}
