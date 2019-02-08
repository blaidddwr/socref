#include "cppqt_enumeration.h"
#include "cppqt_factory.h"
#include "cppqt_enumvalue.h"



using namespace CppQt;
//






/*!
 * Implements _Abstract::Block_ interface. 
 *
 * @return See interface docs. 
 */
QString Enumeration::name() const
{
   // Create a new string setting it to this block's base name. 
   QString ret {getString("name")};

   // If this enumeration is a class append that info to the string. 
   if ( getBool("class") ) ret.append(" [C]");

   // Return the name string. 
   return ret;
}






/*!
 * Tests of this enumeration is a class, returning true if it is. 
 *
 * @return True if this enumeration is a class or false otherwise. 
 */
bool Enumeration::isClass() const
{
   return getBool("class");
}






/*!
 * Returns a pointer list of all enumeration value blocks this enumeration block 
 * contains. 
 *
 * @return Pointer list of all enumeration value blocks this enumeration contains. 
 */
QList<EnumValue*> Enumeration::values() const
{
   // Make a pointer list of all of this block's enumeration value child blocks and 
   // return it. 
   return makeListOfType<EnumValue>(Factory::EnumValueType);
}
