#include "cppqt_function.h"
#include <exception.h>
#include "cppqt_function_view.h"
#include "cppqt_function_edit.h"
#include "cppqt_variable.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_template.h"
#include "cppqt_access.h"
#include "cppqt_type.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. This is in addition to the base fields this block 
 * inherits. 
 */
const QStringList Function::_fields
{
   "default"
   ,"explicit"
   ,"virtual"
   ,"const"
   ,"constexpr"
   ,"static"
   ,"noexcept"
   ,"override"
   ,"final"
   ,"abstract"
   ,"type"
   ,"return_description"
   ,"operation"
};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Function::type() const
{
   return BlockFactory::FunctionType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Function::name() const
{
   // Create an empty return string. 
   QString ret;

   // If this function has templates then append a marker. 
   if ( hasTemplates() ) ret.append("<> ");

   // Return the display name, appending the full function name to it. 
   return ret.append(fullName(!isVoidReturn(),Base::name()));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Function::icon() const
{
   // Initialize the static icons for this block type. 
   static QIcon regularIcon(":/icons/function.svg");
   static QIcon virtualIcon(":/icons/virtual.svg");
   static QIcon abstractIcon(":/icons/abstract.svg");
   static QIcon staticIcon(":/icons/static.svg");

   // Return the appropriate icon for this function block based off certain 
   // properties. 
   if ( _abstract ) return abstractIcon;
   else if ( _virtual ) return virtualIcon;
   else if ( isStatic() ) return staticIcon;
   else return regularIcon;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Function::buildList() const
{
   // Initialize the return build list for this block type. 
   QList<int> ret {BlockFactory::VariableType};

   // If this function block is not virtual then add templates to its build list. 
   if ( !_virtual ) ret << BlockFactory::TemplateType;

   // Return the build list. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Function::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Function::fieldSize() const
{
   // Use the field enumeration to return the total field size. 
   return Field::Total;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
AbstractBlock::Field Function::fieldType(int index) const
{
   // Based off the given field index return its field type. 
   switch (index)
   {
   case Field::Default:
   case Field::Explicit:
   case Field::Virtual:
   case Field::Const:
   case Field::ConstExpr:
   case Field::Static:
   case Field::NoExcept:
   case Field::Override:
   case Field::Final:
   case Field::Abstract: return AbstractBlock::Field::Boolean;
   case Field::ReturnType:
   case Field::ReturnDescription: return AbstractBlock::Field::String;
   case Field::Operations: return AbstractBlock::Field::StringList;

   // If the given index is unknown for this block then call its base interface. 
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
QVariant Function::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::Default: return _default;
   case Field::Explicit: return _explicit;
   case Field::Virtual: return _virtual;
   case Field::Const: return _const;
   case Field::ConstExpr: return _constExpr;
   case Field::Static: return _static;
   case Field::NoExcept: return _noExcept;
   case Field::Override: return _override;
   case Field::Final: return _final;
   case Field::Abstract: return _abstract;
   case Field::ReturnType: return _returnType;
   case Field::ReturnDescription: return _returnDescription;
   case Field::Operations: return _operations;

   // If the given index is unknown for this block then call its base interface. 
   default: return Base::field(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Function::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Tests if this function block's default property is set, returning true if it is. 
 *
 * @return True if this block's default property is set or false otherwise. 
 */
bool Function::isDefault() const
{
   return _default;
}






/*!
 * Tests if this function block's explicit property is set, returning true if it 
 * is. 
 *
 * @return True if this block's explicit property is set or false otherwise. 
 */
bool Function::isExplicit() const
{
   return _explicit;
}






/*!
 * Tests if this function block's virtual property is set, returning true if it is. 
 *
 * @return True if this block's virtual property is set or false otherwise. 
 */
bool Function::isVirtual() const
{
   return _virtual;
}






/*!
 * Tests if this function block's constant property is set, returning true if it 
 * is. 
 *
 * @return True if this block's constant property is set or false otherwise. 
 */
bool Function::isConst() const
{
   return _const;
}






/*!
 * Tests if this function block's constant expression property is set, returning 
 * true if it is. 
 *
 * @return True if this block's constant expression property is set or false 
 *         otherwise. 
 */
bool Function::isConstExpr() const
{
   return _constExpr;
}






/*!
 * Tests if this function block's static property is set, returning true if it is. 
 *
 * @return True if this block's static property is set or false otherwise. 
 */
bool Function::isStatic() const
{
   return _static;
}






/*!
 * Tests if this function block's no exception property is set, returning true if 
 * it is. 
 *
 * @return True if this block's no exception property is set or false otherwise. 
 */
bool Function::isNoExcept() const
{
   return _noExcept;
}






/*!
 * Tests if this function block's override property is set, returning true if it 
 * is. 
 *
 * @return True if this block's override property is set or false otherwise. 
 */
bool Function::isOverride() const
{
   return _override;
}






/*!
 * Tests if this function block's final property is set, returning true if it is. 
 *
 * @return True if this block's final property is set or false otherwise. 
 */
bool Function::isFinal() const
{
   return _final;
}






/*!
 * Tests if this function block's abstract property is set, returning true if it 
 * is. 
 *
 * @return True if this block's abstract property is set or false otherwise. 
 */
bool Function::isAbstract() const
{
   return _abstract;
}






/*!
 * Returns the return type for this function block.  
 *
 * @return The return type for this function block. 
 */
QString Function::returnType() const
{
   return _returnType;
}






/*!
 * Returns the description for the return type of this function block. 
 *
 * @return The description for the return type of this function block. 
 */
QString Function::returnDescription() const
{
   return _returnDescription;
}






/*!
 * Tests if this function block's return block is void and therefore has no return, 
 * returning true if it is. 
 *
 * @return True if this function block's return type is void or false otherwise. 
 */
bool Function::isVoidReturn() const
{
   // Test to see if this function block's return type is void. 
   return _returnType == QString("void");
}






/*!
 * Returns the steps of operations for this function block, whose list of strings 
 * is used for inline comments. 
 *
 * @return The steps of operations for this function block. 
 */
QStringList Function::operations() const
{
   return _operations;
}






/*!
 * Tests if this function block is a method of a class, returning true if it is. 
 *
 * @return True if this function block is a method of a class or false otherwise. 
 */
bool Function::isMethod() const
{
   // Get this block's parent pointer and make sure it is not null. 
   AbstractBlock* up {parent()};
   if ( !up ) return false;

   // Determine if this function block is a method by seeing if its parent is an 
   // access block. 
   return up->type() == BlockFactory::AccessType;
}






/*!
 * Tests if this function block is a private method of a class, returning true if 
 * it is. 
 *
 * @return True if this function block is a private method of a class or false 
 *         otherwise. 
 */
bool Function::isPrivateMethod() const
{
   // Make sure this function block is a method. 
   if ( !isMethod() ) return false;

   // Determine if this function block is a private method by seeing if its parent 
   // access block's type is private. 
   return parent()->cast<Access>(BlockFactory::AccessType)->accessType() == Access::Type::Private;
}






/*!
 * Tests if this function block or any class it inherits from has any templates, 
 * returning true if so. 
 *
 * @return True if this function block or any of its inherited classes contain 
 *         templates or false otherwise. 
 */
bool Function::hasAnyTemplates() const
{
   // Check to see if this function block has any templates. 
   if ( hasTemplates() ) return true;

   // If this function block is a method then call on its class to determine if it or 
   // any parent classes contain templates. 
   if ( isMethod() )
   {
      return parent()->parent()->cast<Class>(BlockFactory::ClassType)->hasAnyTemplates();
   }

   // If this is reached then there are no templates so return false. 
   return false;
}






/*!
 * Tests if this function block has templates arguments, returning true if it does. 
 * This does not test if any parent classes contain templates. 
 *
 * @return True if this function block has template arguments or false otherwise. 
 */
bool Function::hasTemplates() const
{
   // See if any direct children blocks of this block are templates. 
   return containsType(BlockFactory::TemplateType);
}






/*!
 * Returns a pointer list of all template children blocks of this function block. 
 *
 * @return Pointer list of all template children blocks of this function block. 
 */
QList<Template*> Function::templates() const
{
   return makeListOfType<Template>(BlockFactory::TemplateType);
}






/*!
 * Returns a pointer list of all variable children blocks of this function block. 
 *
 * @return Pointer list of all variable children blocks of this function block. 
 */
QList<Variable*> Function::arguments() const
{
   return makeListOfType<Variable>(BlockFactory::VariableType);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Function::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Function);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Function::fieldModified(int index)
{
   // Based off the given field index notify the changes to this block. 
   switch (index)
   {
   case Field::Default:
   case Field::Explicit:
   case Field::Virtual:
   case Field::Const:
   case Field::ConstExpr:
   case Field::Static:
   case Field::NoExcept:
   case Field::Override:
   case Field::Final:
   case Field::Abstract:
   case Field::ReturnType:
   case Field::ReturnDescription:
   case Field::Operations:
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
      break;

   // If the given index is unknown for this block then call its base interface. 
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
void Function::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. 
   switch (index)
   {
   case Field::Default:
      setDefault(value.toBool());
      break;
   case Field::Explicit:
      setExplicit(value.toBool());
      break;
   case Field::Virtual:
      setVirtual(value.toBool());
      break;
   case Field::Const:
      setConst(value.toBool());
      break;
   case Field::ConstExpr:
      setConstExpr(value.toBool());
      break;
   case Field::Static:
      setStatic(value.toBool());
      break;
   case Field::NoExcept:
      _noExcept = value.toBool();
      break;
   case Field::Override:
      setOverride(value.toBool());
      break;
   case Field::Final:
      setFinal(value.toBool());
      break;
   case Field::Abstract:
      setAbstract(value.toBool());
      break;
   case Field::ReturnType:
      setReturnType(value.toString());
      break;
   case Field::ReturnDescription:
      _returnDescription = value.toString();
      break;
   case Field::Operations:
      _operations = value.toStringList();
      break;

   // If the given index is unknown for this block then call its base interface. 
   default: return Base::quietlySetField(index,value);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Function::childAdded(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return false to end propagation. 
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Function::childRemoved(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return false to end propagation. 
   return false;
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @return See interface docs. 
 */
QStringList Function::fields() const
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
 * Constructs and returns the full display name of this function with the given has 
 * return indicator and function name. This takes the given information and adds 
 * display information about the arguments and properties of this function block. 
 *
 * @param hasReturn True if this function has a return type that is not void or 
 *                  false otherwise. Used to indicate there is a return or not in 
 *                  the display name. 
 *
 * @param name The function name that is used to construct the full display name. 
 *
 * @return Full display name of this function with the given has return indicator 
 *         and function name. 
 */
QString Function::fullName(bool hasReturn, const QString& name) const
{
   // Create an empty return string. 
   QString ret;

   // If the given has return indicator is true then append a return indicator. 
   if ( hasReturn ) ret.append("... ");

   // Append the function name and then the number of arguments. 
   const QList<Variable*> list {arguments()};
   ret.append(QString("%1(%2)").arg(name).arg(list.size()));

   // Append the attributes display string. 
   ret.append(attributes());

   // Return the full display name string. 
   return ret;
}






/*!
 * Returns a display string showing all set properties for this function block 
 * using capital letter flags. 
 *
 * @return Display string showing all set properties for this function block. 
 */
QString Function::attributes() const
{
   // Create an empty string, appending all set properties of this function block as 
   // capital letter flags. 
   QString ret;
   if ( _default ) ret.append("D");
   if ( _explicit ) ret.append("E");
   if ( _const ) ret.append("C");
   if ( _constExpr ) ret.append("X");
   if ( _noExcept ) ret.append("N");
   if ( _override ) ret.append("O");
   if ( _final ) ret.append("F");
   if ( _abstract ) ret.append("A");

   // If the flag string is not empty then surround it with brackets and a space. 
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");

   // Return the attributes display string. 
   return ret;
}






/*!
 * Sets the state of this function block's default property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setDefault(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as default when it is not a class method."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _default = state;
}






/*!
 * Sets the state of this function block's explicit property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setExplicit(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as explicit when it is not a class method."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _explicit = state;
}






/*!
 * Sets the state of this function block's virtual property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setVirtual(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && ( isStatic() || hasTemplates() || !isMethod() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as virtual when it is static, has templates, or is not a class method."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _virtual = state;
}






/*!
 * Sets the state of this function block's constant property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setConst(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as const when it is not a class method."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _const = state;
}






/*!
 * Sets the state of this function block's constant expression property to the 
 * given state. If the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setConstExpr(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as constexpr when it is virtual."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _constExpr = state;
}






/*!
 * Sets the state of this function block's static property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setStatic(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as static when it is virtual."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _static = state;
}






/*!
 * Sets the state of this function block's override property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setOverride(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as override when it is not virtual or it is abstract."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _override = state;
}






/*!
 * Sets the state of this function block's final property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setFinal(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as final when it is not virtual or it is abstract."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _final = state;
}






/*!
 * Sets the state of this function block's abstract property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this function block's property is set to. 
 */
void Function::setAbstract(bool state)
{
   // Make sure the given state is valid given this function block's current context. 
   if ( parent() && state && ( !_virtual || _override || _final ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as abstract when it is not virtual or it is override/final."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _abstract = state;
}






/*!
 * Sets this function block's return type field to the new given value. If the new 
 * value is not a valid C++ type then an exception is thrown. 
 *
 * @param value The new value that this function block's return type field is set 
 *              to. 
 */
void Function::setReturnType(const QString& value)
{
   // Make sure the given value is valid given this function block's current context. 
   if ( !value.isEmpty() && !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid return type '%1'.").arg(value));
      throw e;
   }

   // Set this block's value to the new one given. 
   _returnType = value;
}
