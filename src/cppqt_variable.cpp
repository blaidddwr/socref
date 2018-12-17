#include "cppqt_variable.h"
#include <QIcon>
#include "cppqt_variable_view.h"



using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Variable::name() const
{
   // Create a string and set it to this variable's base name. 
   QString ret {getString("name")};

   // Check to see if this variable has an initial vallue. 
   if ( !hasInitializer() )
   {
      // Append the initial value indicator appropriate for the context of this 
      // variable. 
      if ( isArgument() ) ret.append(" =");
      else ret.append(" {}");
   }

   // Append this variable's property markers. 
   ret.append(attributes());

   // Return this variable's display name. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Variable::icon() const
{
   // Initialize the static icons for this block type. 
   static QIcon regularIcon(":/icons/variable.svg");
   static QIcon staticIcon(":/icons/svariable.svg");

   // Return the correct icon based off this variable's properties. 
   if ( isStatic() ) return staticIcon;
   else return regularIcon;
}






/*!
 * Tests if this variable block's constant expression property is set. 
 *
 * @return True if this block's constant expression property is set or false 
 *         otherwise. 
 */
bool Variable::isConstExpr() const
{
   return getBool("constexpr");
}






/*!
 * Tests if this variable block's static property is set. 
 *
 * @return True if this block's static property is set or false otherwise. 
 */
bool Variable::isStatic() const
{
   return getBool("static");
}






/*!
 * Tests if this variable block's mutable property is set. 
 *
 * @return True if this block's mutable property is set or false otherwise. 
 */
bool Variable::isMutable() const
{
   return getBool("mutable");
}






/*!
 * Tests if this variable block's thread local property is set. 
 *
 * @return True if this block's thread local property is set or false otherwise. 
 */
bool Variable::isThreadLocal() const
{
   return getBool("threadlocal");
}






/*!
 * Returns this variable block's type field value. 
 *
 * @return This variable's C++ type. 
 */
QString Variable::variableType() const
{
   return getString("type");
}






/*!
 * Tests if this variable block has an initial value. 
 *
 * @return True if this variable has an initial value or false otherwise. 
 */
bool Variable::hasInitializer() const
{
   // Test if this variable's initial value string is empty to determine if it has 
   // one. 
   return !initializer().isEmpty();
}






/*!
 * Returns this variable block's initial value or an empty string if none is set. 
 *
 * @return This variable's initial value or an empty string is none is set. 
 */
QString Variable::initializer() const
{
   return getString("initializer");
}






/*!
 * Tests if this variable block is a member of a C++ class, returning true if it is 
 * a member. 
 *
 * @return True if this variable is a class member or false otherwise. 
 */
bool Variable::isMember() const
{
   return false;
   // Get this block's parent block pointer and make sure it is not null. 
   //AbstractBlock* up {parent()};
   //if ( !up ) return false;

   // Test if this variable is an argument by seeing if its parent block is an access 
   // type. 
   //return up->type() == Factory::AccessType;
}






/*!
 * Tests if this variable block is an argument of a C++ function, returning true if 
 * it is an argument. 
 *
 * @return True if this is a function argument or false otherwise. 
 */
bool Variable::isArgument() const
{
   return false;
   // Test if this variable is an argument by seeing if its parent is a function 
   // block type. 
   //return qobject_cast<Function*>(parent());
}






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<BasicBlock::View> Variable::makeBasicView() const
{
   return new View(this);
}






/*!
 * Returns a string displaying any properties this variable block has set. If no 
 * properties are set then this returns an empty string. 
 *
 * @return String displaying any properties this variable block has set. 
 */
QString Variable::attributes() const
{
   // Create a string, appending any properties this variable has using single 
   // capital letters. 
   QString ret;
   if ( isConstExpr() ) ret.append("X");
   if ( isStatic() ) ret.append("S");
   if ( isMutable() ) ret.append("M");
   if ( isThreadLocal() ) ret.append("L");

   // If the string is not empty then enclose it with brackets and a space. 
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");

   // Return the attributes string. 
   return ret;
}
