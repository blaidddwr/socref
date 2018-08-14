#include "cppqt_variable.h"
#include <socutil/sut_exceptions.h>
#include "cppqt_variable_view.h"
#include "cppqt_variable_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_type.h"



using namespace std;
using namespace Sut;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. This is in addition to the base fields this block 
 * inherits. 
 */
const QStringList Variable::_fields
{
   "constexpr"
   ,"static"
   ,"mutable"
   ,"type"
   ,"initializer"
};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Variable::type() const
{
   return BlockFactory::VariableType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Variable::name() const
{
   // Create a string and set it to this variable's base name. 
   QString ret {Base::name()};

   // Check to see if this variable has an initial vallue. 
   if ( !_initializer.isEmpty() )
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
   if ( _static ) return staticIcon;
   else return regularIcon;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Variable::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Variable::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Variable::fieldSize() const
{
   // Use the field enumeration to return the total number of fields. 
   return Field::Total;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
AbstractBlock::Field Variable::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::ConstExpr:
   case Field::Static:
   case Field::Mutable: return AbstractBlock::Field::Boolean;
   case Field::Type:
   case Field::Initializer: return AbstractBlock::Field::String;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default: return Base::fieldType(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QVariant Variable::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::ConstExpr: return _constExpr;
   case Field::Static: return _static;
   case Field::Mutable: return _mutable;
   case Field::Type: return _type;
   case Field::Initializer: return _initializer;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default: return Base::field(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Variable::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new variable block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Variable::Variable(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault )
   {
      setType(QStringLiteral("int"));
      setName(QStringLiteral("var"));
   }
}






/*!
 * Tests if this variable block's constant expression property is set. 
 *
 * @return True if this block's constant expression property is set or false 
 *         otherwise. 
 */
bool Variable::isConstExpr() const
{
   return _constExpr;
}






/*!
 * Tests if this variable block's static property is set. 
 *
 * @return True if this block's static property is set or false otherwise. 
 */
bool Variable::isStatic() const
{
   return _static;
}






/*!
 * Tests if this variable block's mutable property is set. 
 *
 * @return True if this block's mutable property is set or false otherwise. 
 */
bool Variable::isMutable() const
{
   return _mutable;
}






/*!
 * Returns this variable block's type field value. 
 *
 * @return This variable's C++ type. 
 */
QString Variable::variableType() const
{
   return _type;
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
   return !_initializer.isEmpty();
}






/*!
 * Returns this variable block's initial value or an empty string if none is set. 
 *
 * @return This variable's initial value or an empty string is none is set. 
 */
QString Variable::initializer() const
{
   return _initializer;
}






/*!
 * Tests if this variable block is a member of a C++ class, returning true if it is 
 * a member. 
 *
 * @return True if this variable is a class member or false otherwise. 
 */
bool Variable::isMember() const
{
   // Get this block's parent block pointer and make sure it is not null. 
   AbstractBlock* up {parent()};
   if ( !up ) return false;

   // Test if this variable is an argument by seeing if its parent block is an access 
   // type. 
   return up->type() == BlockFactory::AccessType;
}






/*!
 * Tests if this variable block is an argument of a C++ function, returning true if 
 * it is an argument. 
 *
 * @return True if this is a function argument or false otherwise. 
 */
bool Variable::isArgument() const
{
   // Test if this variable is an argument by seeing if its parent is a function 
   // block type. 
   return qobject_cast<Function*>(parent());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Variable::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Variable);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Variable::fieldModified(int index)
{
   // Based off the given field index notify the changes to this block. 
   switch (index)
   {
   case Field::ConstExpr:
   case Field::Static:
   case Field::Mutable:
   case Field::Type:
   case Field::Initializer:
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
      break;

   // If the given index is not unknown for this block then call its base interface. 
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @param value See interface docs. 
 */
void Variable::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. 
   switch (index)
   {
   case Field::ConstExpr:
      setConstExpr(value.toBool());
      break;
   case Field::Static:
      setStatic(value.toBool());
      break;
   case Field::Mutable:
      setMutable(value.toBool());
      break;
   case Field::Type:
      setType(value.toString());
      break;
   case Field::Initializer:
      _initializer = value.toString();
      break;

   // If the given index is not unknown for this block then call its base interface. 
   default:
      Base::quietlySetField(index,value);
   }
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @return See interface docs. 
 */
QStringList Variable::fields() const
{
   // Initialize an empty static string list. 
   static QStringList ret;

   // If the string list is empty then populate it. 
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }

   // Return the combined fields string list. 
   return ret;
}






/*!
 * Sets this variable block's type field to the new given value. If the new value 
 * is not a valid C++ type then an exception is thrown. 
 *
 * @param value The new value that this variable block's return type is set to. 
 */
void Variable::setType(const QString& value)
{
   // Make sure the given value is valid given this variable's current context. 
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(value));
      throw e;
   }

   // Set this block's value to the new one given. 
   _type = value;
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
   if ( _constExpr ) ret.append("X");
   if ( _static ) ret.append("S");
   if ( _mutable ) ret.append("M");

   // If the string is not empty then enclose it with brackets and a space. 
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");

   // Return the attributes string. 
   return ret;
}






/*!
 * Sets the state of this variable block's constant expression property to the 
 * given state. If the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this variable block's property is set to. 
 */
void Variable::setConstExpr(bool state)
{
   // Make sure the given state is valid given this variable's current context. 
   if ( parent() && state && isArgument() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as constant expression when it is a function argument."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _constExpr = state;
}






/*!
 * Sets the state of this variable block's static property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this variable block's property is set to. 
 */
void Variable::setStatic(bool state)
{
   // Make sure the given state is valid given this variable's current context. 
   if ( parent() && state && !isMember() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is not a class member."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _static = state;
}






/*!
 * Sets the state of this variable block's mutable property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this variable block's property is set to. 
 */
void Variable::setMutable(bool state)
{
   // Make sure the given state is valid given this variable's current context. 
   if ( parent() && state && !isMember() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as mutable when it is not a class member."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _mutable = state;
}
