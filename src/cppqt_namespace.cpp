#include "cppqt_namespace.h"



using namespace CppQt;
//






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 */
const Namespace* Namespace::root() const
{
   // Get the root pointer for this block, casting it as a namespace block and making 
   // sure it worked. 
   const Namespace* ret {qobject_cast<const Namespace*>(Abstract::Block::root())};
   Q_CHECK_PTR(ret);

   // Return the root namespace pointer. 
   return ret;
}






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 */
Namespace* Namespace::root()
{
   // Get the root pointer for this block, casting it as a namespace block and making 
   // sure it worked. 
   Namespace* ret {qobject_cast<Namespace*>(Abstract::Block::root())};
   Q_CHECK_PTR(ret);

   // Return the root namespace pointer. 
   return ret;
}
