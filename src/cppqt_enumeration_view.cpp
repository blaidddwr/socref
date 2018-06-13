#include "cppqt_enumeration_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given enumeration block. 
 *
 * @param block Enumeration block this new view displays. 
 */
Enumeration::View::View(const Enumeration* block):
   Base::View(block)
{
   setText(displayDescription());
}
