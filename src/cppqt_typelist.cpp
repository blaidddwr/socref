#include "cppqt_typelist.h"
#include "cppqt_typelist_view.h"



using namespace Sut;
using namespace CppQt;
//






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<BasicBlock::View> TypeList::makeBasicView() const
{
   return new View(this);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childNameModified(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the body has changed and return false. 
   notifyBodyModified();
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childAdded(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body has changed and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childRemoved(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body has changed and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}
