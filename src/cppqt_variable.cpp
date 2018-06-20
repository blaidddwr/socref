#include "cppqt_variable.h"
#include <exception.h>
#include "cppqt_variable_view.h"
#include "cppqt_variable_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_type.h"



using namespace std;
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
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Variable::type() const
{
   return BlockFactory::VariableType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Variable::name() const
{
   // Create a string _ret_, setting it to this variable's base name. 
   QString ret {Base::name()};

   // If this variable has an initial value that append that to _ret_, using 
   // different display types depending on if it is an argument or not. 
   if ( !_initializer.isEmpty() )
   {
      if ( isArgument() ) ret.append(" =");
      else ret.append(" {}");
   }

   // Append this variable's properties to _ret_ and then return _ret_. 
   ret.append(attributes());
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Variable::icon() const
{
   // If the variable block's static icons are not loaded then load them. 
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon static_;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/variable.svg");
      static_ = QIcon(":/icons/svariable.svg");
   }

   // Return the static or regular icon, depending on if this variable has its static 
   // property set or not. 
   if ( _static ) return static_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Variable::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Variable::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Variable::fieldSize() const
{
   return Field::Total;
}






/*!
 * Implements the interface that returns the field type for the given field index 
 * of this block. 
 *
 * @param index Index of the field whose field type is returned. 
 *
 * @return Field type of the given field index of this block. 
 */
AbstractBlock::Field Variable::fieldType(int index) const
{
   switch (index)
   {
   case Field::ConstExpr:
   case Field::Static:
   case Field::Mutable: return AbstractBlock::Field::Boolean;
   case Field::Type:
   case Field::Initializer: return AbstractBlock::Field::String;
   default: return Base::fieldType(index);
   }
}






/*!
 * Implements the interface that returns the value of the field with the given 
 * index for this block. 
 *
 * @param index Index of the field whose value is returned. 
 *
 * @return Value of the field with the given index for this block. 
 */
QVariant Variable::field(int index) const
{
   switch (index)
   {
   case Field::ConstExpr: return _constExpr;
   case Field::Static: return _static;
   case Field::Mutable: return _mutable;
   case Field::Type: return _type;
   case Field::Initializer: return _initializer;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Variable::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new variable block with the given name. 
 *
 * @param name The value this new variable block's base name field is set to. 
 */
Variable::Variable(const QString& name):
   Base(name)
{}






/*!
 * Constructs a new variable block with the given type and name. 
 *
 * @param type The value this new variable block's type field is set to. 
 *
 * @param name The value this new variable block's base name field is set to. 
 */
Variable::Variable(const QString& type, const QString& name):
   Base(name),
   _type(type)
{
   checkType(type);
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
   return parent()->type() == BlockFactory::AccessType;
}






/*!
 * Tests if this variable block is an argument of a C++ function, returning true if 
 * it is an argument. 
 *
 * @return True if this is a function argument or false otherwise. 
 */
bool Variable::isArgument() const
{
   return qobject_cast<Function*>(parent());
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Variable::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Variable);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Variable::fieldModified(int index)
{
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
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 * Implements the interface that quietly sets the value of the field with the given 
 * index to the new given value. 
 *
 * @param index Index of the field whose value is set to the new given value. 
 *
 * @param value New value that the field with the given index is set to. 
 */
void Variable::quietlySetField(int index, const QVariant& value)
{
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
   default:
      Base::quietlySetField(index,value);
   }
}






/*!
 * Implements the interface that returns the full list of of all field tag names 
 * for this block that matches the order of this block's field enumeration. 
 *
 * @return Full list of all field tag names for this block. 
 */
QStringList Variable::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }
   return ret;
}






/*!
 * Checks the given string to verify it is a valid C++ type. If it is invalid then 
 * an exception is thrown. 
 *
 * @param value The string that is verified to have valid C++ type syntax. 
 */
void Variable::checkType(const QString& value)
{
   // If the given string is not a valid C++ type then throw an exception. 
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(value));
      throw e;
   }
}






/*!
 * Returns a string displaying any properties this variable block has set. If no 
 * properties are set then this returns an empty string. 
 *
 * @return String displaying any properties this variable block has set. 
 */
QString Variable::attributes() const
{
   // Create a string _ret_, appending any properties it has using single capital 
   // letters. If any properties are appended then enclose them with brackets and a 
   // space. 
   QString ret;
   if ( _constExpr ) ret.append("X");
   if ( _static ) ret.append("S");
   if ( _mutable ) ret.append("M");
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");

   // Return _ret_. 
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
   // If the new given state is illegal then throw an exception, else set this 
   // block's property to the new given state. 
   if ( state && isArgument() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as constant expression when it is a function argument."));
      throw e;
   }
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
   // If the new given state is illegal then throw an exception, else set this 
   // block's property to the new given state. 
   if ( state && !isMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is not a class member."));
      throw e;
   }
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
   // If the new given state is illegal then throw an exception, else set this 
   // block's property to the new given state. 
   if ( state && !isMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as mutable when it is not a class member."));
      throw e;
   }
   _mutable = state;
}






/*!
 * Set this variable block's type field to the new given value. If the new value is 
 * not a valid C++ type then an exception is thrown. 
 *
 * @param value  
 */
void Variable::setType(const QString& value)
{
   // Check the C++ type syntax for the given value and then set this variable 
   // block's type field to the new value. 
   checkType(value);
   _type = value;
}
