#include "cppqt_base.h"



using namespace CppQt;
//






/*!
 * Returns the base name for this block. 
 *
 * @return Base name for this block. 
 */
QString Base::baseName() const
{
   return getString("name");
}






/*!
 * Returns the description for this block. 
 *
 * @return Description for this block. 
 */
QString Base::description() const
{
   return getString("description");
}
