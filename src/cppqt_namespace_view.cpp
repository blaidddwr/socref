#include "cppqt_namespace_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new type view object with the given namespace block. 
 *
 * @param block Namespace block this new view displays. 
 */
Namespace::View::View(const Namespace* block):
   Base::View(block)
{
   setText(displayDescription());
}
