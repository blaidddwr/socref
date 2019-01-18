#include "cppqt_namespace.h"
#include <socutil/sut_exceptions.h>



using namespace Sut;
using namespace CppQt;
//






/*!
 * This interface returns the real block children for this block. The real block 
 * children are all children of any access block children. If this is not a class 
 * then it is simply all direct children. 
 *
 * @return Pointer list of all real children for this block. 
 */
QList<AbstractBlock*> Namespace::realChildren() const
{
   return list();
}






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 */
const Namespace* Namespace::root() const
{
   // Get the root pointer for this block, casting it as a namespace block and making 
   // sure it worked. 
   const Namespace* ret {qobject_cast<const Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }

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
   Namespace* ret {qobject_cast<Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }

   // Return the root namespace pointer. 
   return ret;
}
