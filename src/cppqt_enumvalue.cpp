#include "cppqt_enumvalue.h"
#include "cppqt_enumvalueview.h"



using namespace CppQt;
//






/*!
 * Implements _Abstract::Block_ interface. 
 *
 * @return See interface docs. 
 */
QString EnumValue::name() const
{
   // Create a string setting it to this block's base name. 
   QString ret {getString("name")};

   // If this enumeration value has a set value then append that info to the string. 
   if ( !getString("value").isEmpty() ) ret.append(" =");

   // Return the name string. 
   return ret;
}






/*!
 * Tests if this enumeration value block has a set value or not, returning true if 
 * it does. 
 *
 * @return True of this enumeration value block has a set value or false otherwise. 
 */
bool EnumValue::hasValue() const
{
   // Test if this block's set value is not empty to determine if it has a value. 
   return !getString("value").isEmpty();
}






/*!
 * Returns the set value for this enumeration value block. If this block has no set 
 * value an empty string is returned. 
 *
 * @return The set value for this enumeration value block or an empty string if it 
 *         has none. 
 */
QString EnumValue::value() const
{
   return getString("value");
}






/*!
 * Implements _Basic::Block_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<Basic::BlockView> EnumValue::makeBasicView() const
{
   return new EnumValueView(this);
}
