#include "cppqt_function.h"
#include <QRegularExpression>
#include "cppqt_functionview.h"
#include "cppqt_variable.h"
#include "cppqt_factory.h"
#include "cppqt_template.h"
#include "cppqt_class.h"
#include "cppqt_access.h"



namespace CppQt
{






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QString Function::name() const
{
   // Create an empty return string.
   QString ret;

   // If this block has templates then append a template indicator.
   if ( hasTemplates() ) ret += QStringLiteral("<> ");

   // If this block is not a constructor, destructor, and its has void return
   // indicator is not true then append a return indicator.
   if ( !isConstructor() && !isDestructor() && !isVoidReturn() ) ret += QStringLiteral("... ");

   // Append the function name and then the number of arguments.
   const QList<Variable*> list {arguments()};
   ret += QString("%1(%2)").arg(baseName()).arg(list.size());

   // Append the attributes display string.
   ret += attributes();

   // Return the full display name string.
   return ret;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QIcon Function::icon() const
{
   // Initialize the static icons for this block type.
   static QIcon constructorIcon(":/icons/constructor.svg");
   static QIcon regularDestructorIcon(":/icons/destructor.svg");
   static QIcon virtualDestructorIcon(":/icons/vdestructor.svg");
   static QIcon abstractDestructorIcon(":/icons/adestructor.svg");
   static QIcon regularOperatorIcon(":/icons/operator.svg");
   static QIcon virtualOperatorIcon(":/icons/voperator.svg");
   static QIcon abstractOperatorIcon(":/icons/aoperator.svg");
   static QIcon signalIcon(":/icons/signal.svg");
   static QIcon regularSlotIcon(":/icons/slot.svg");
   static QIcon virtualSlotIcon(":/icons/vslot.svg");
   static QIcon abstractSlotIcon(":/icons/aslot.svg");
   static QIcon regularIcon(":/icons/function.svg");
   static QIcon virtualIcon(":/icons/virtual.svg");
   static QIcon abstractIcon(":/icons/abstract.svg");
   static QIcon staticIcon(":/icons/static.svg");

   // Return the appropriate icon for this function block based off its function type
   // and certain properties.
   if ( isConstructor() ) return constructorIcon;
   else if ( isDestructor() )
   {
      if ( isAbstract() ) return abstractDestructorIcon;
      else if ( isVirtual() ) return virtualDestructorIcon;
      else return regularDestructorIcon;
   }
   else if ( isOperator() )
   {
      if ( isAbstract() ) return abstractOperatorIcon;
      else if ( isVirtual() ) return virtualOperatorIcon;
      else return regularOperatorIcon;
   }
   else if ( isSignal() ) return signalIcon;
   else if ( isSlot() )
   {
      if ( isAbstract() ) return abstractSlotIcon;
      else if ( isVirtual() ) return virtualSlotIcon;
      else return regularSlotIcon;
   }
   else
   {
      if ( isAbstract() ) return abstractIcon;
      else if ( isVirtual() ) return virtualIcon;
      else if ( isStatic() ) return staticIcon;
      else return regularIcon;
   }
}






/*!
 * Tests if this function block is a constructor, returning true if it is.
 *
 * @return True if this function block is a constructor or false otherwise.
 */
bool Function::isConstructor() const
{
   return Base::baseName() == QStringLiteral("^");
}






/*!
 * Tests if this function block is a destructor, returning true if it is.
 *
 * @return True if this function block is a destructor or false otherwise.
 */
bool Function::isDestructor() const
{
   return Base::baseName() == QStringLiteral("~^");
}






/*!
 * Tests if this function block is an operator, returning true if it is.
 *
 * @return True if this function block is an operator or false otherwise.
 */
bool Function::isOperator() const
{
   return QRegularExpression("\\Aoperator*\\z").match(baseName()).hasMatch();
}






/*!
 * Tests if this function block is a Qt signal, returning true if it is.
 *
 * @return True if this function block is a Qt signal or false otherwise.
 */
bool Function::isSignal() const
{
   if ( !isMethod() ) return false;
   return parent()->cast<Access>(Factory::AccessType)->isSignal();
}






/*!
 * Tests if this function block is a Qt slot, returning true if it is.
 *
 * @return True if this function block is a Qt slot or false otherwise.
 */
bool Function::isSlot() const
{
   if ( !isMethod() ) return false;
   return parent()->cast<Access>(Factory::AccessType)->isSlot();
}






/*!
 * Tests if this function block's default property is set, returning true if it
 * is.
 *
 * @return True if this block's default property is set or false otherwise.
 */
bool Function::isDefault() const
{
   return getBool("default");
}






/*!
 * Tests if this function block's deleted property is set, returning true if it
 * is.
 *
 * @return True if this block's deleted property is set or false otherwise.
 */
bool Function::isDeleted() const
{
   return getBool("deleted");
}






/*!
 * Tests if this function block's qt invokable property is set, returning true
 * if it is.
 *
 * @return True if this block's qt invokable property is set or false otherwise.
 */
bool Function::isQtInvokable() const
{
   return getBool("invokable");
}






/*!
 * Tests if this function block's explicit property is set, returning true if it
 * is.
 *
 * @return True if this block's explicit property is set or false otherwise.
 */
bool Function::isExplicit() const
{
   return getBool("explicit");
}






/*!
 * Tests if this function block's virtual property is set, returning true if it
 * is.
 *
 * @return True if this block's virtual property is set or false otherwise.
 */
bool Function::isVirtual() const
{
   return getBool("virtual");
}






/*!
 * Tests if this function block's constant property is set, returning true if it
 * is.
 *
 * @return True if this block's constant property is set or false otherwise.
 */
bool Function::isConst() const
{
   return getBool("const");
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
   return getBool("constexpr");
}






/*!
 * Tests if this function block's static property is set, returning true if it
 * is.
 *
 * @return True if this block's static property is set or false otherwise.
 */
bool Function::isStatic() const
{
   return getBool("static");
}






/*!
 * Tests if this function block's no exception property is set, returning true
 * if it is.
 *
 * @return True if this block's no exception property is set or false otherwise.
 */
bool Function::isNoExcept() const
{
   return getBool("noexcept");
}






/*!
 * Tests if this function block's override property is set, returning true if it
 * is.
 *
 * @return True if this block's override property is set or false otherwise.
 */
bool Function::isOverride() const
{
   return getBool("override");
}






/*!
 * Tests if this function block's final property is set, returning true if it
 * is.
 *
 * @return True if this block's final property is set or false otherwise.
 */
bool Function::isFinal() const
{
   return getBool("final");
}






/*!
 * Tests if this function block's abstract property is set, returning true if it
 * is.
 *
 * @return True if this block's abstract property is set or false otherwise.
 */
bool Function::isAbstract() const
{
   return getBool("abstract");
}






/*!
 * Returns the base name for this function block. This overrides the method from
 * the base class because of constructors and destructors.
 *
 * @return Base name for this function block.
 */
QString Function::baseName() const
{
   // Create a new return string with the regular base name of this function.
   QString ret {Base::baseName()};

   // If this function is not a method then return the regular base name.
   if ( !isMethod() ) return ret;

   // If this function is a constructor or destructor then change its base name to
   // the name of its class, prepending a ~ if it is a destructor.
   if ( ret == QStringLiteral("^") )
   {
      ret = parent()->parent()->cast<Class>(Factory::ClassType)->baseName();
   }
   else if ( ret == QStringLiteral("~^") )
   {
      ret = QStringLiteral("~") + parent()->parent()->cast<Class>(Factory::ClassType)->baseName();
   }

   // Return the base name.
   return ret;
}






/*!
 * Returns the return type for this function block. 
 *
 * @return The return type for this function block.
 */
QString Function::returnType() const
{
   return getString("type");
}






/*!
 * Returns the description for the return type of this function block.
 *
 * @return The description for the return type of this function block.
 */
QString Function::returnDescription() const
{
   return getString("return_description");
}






/*!
 * Tests if this function block's return block is void and therefore has no
 * return, returning true if it is.
 *
 * @return True if this function block's return type is void or false otherwise.
 */
bool Function::isVoidReturn() const
{
   // Test to see if this function block's return type is void.
   return returnType() == QStringLiteral("void");
}






/*!
 * Returns the steps of operations for this function block, whose list of
 * strings is used for inline comments.
 *
 * @return The steps of operations for this function block.
 */
QStringList Function::operations() const
{
   return getStringList("operation");
}






/*!
 * Tests if this function block is a method of a class, returning true if it is.
 *
 * @return True if this function block is a method of a class or false
 *         otherwise.
 */
bool Function::isMethod() const
{
   // Get this block's parent pointer and make sure it is not null.
   Abstract::Block* up {parent()};
   if ( !up ) return false;

   // Determine if this function block is a method by seeing if its parent is an
   // access block.
   return up->type() == Factory::AccessType;
}






/*!
 * Tests if this function block is a private method of a class, returning true
 * if it is.
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
   return parent()->cast<Access>(Factory::AccessType)->isPrivate();
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
      return parent()->parent()->cast<Class>(Factory::ClassType)->hasAnyTemplates();
   }

   // If this is reached then there are no templates so return false.
   return false;
}






/*!
 * Tests if this function block has templates arguments, returning true if it
 * does. This does not test if any parent classes contain templates.
 *
 * @return True if this function block has template arguments or false
 *         otherwise.
 */
bool Function::hasTemplates() const
{
   // See if any direct children blocks of this block are templates.
   return containsType(Factory::TemplateType);
}






/*!
 * Returns a pointer list of all template children blocks of this function
 * block.
 *
 * @return Pointer list of all template children blocks of this function block.
 */
QList<Template*> Function::templates() const
{
   return createListOfType<Template>(Factory::TemplateType);
}






/*!
 * Returns a pointer list of all variable children blocks of this function
 * block.
 *
 * @return Pointer list of all variable children blocks of this function block.
 */
QList<Variable*> Function::arguments() const
{
   return createListOfType<Variable>(Factory::VariableType);
}






/*!
 * Called when this function's class block has changed its name. This in turn
 * notifies this block's name has changed if it is a constructor or destructor.
 */
void Function::classNameChanged()
{
   if ( isConstructor() || isDestructor() ) update();
}






/*!
 * Implements _Basic::Block_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Basic::BlockView> Function::makeBasicView() const
{
   return new FunctionView(this);
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @param child See interface docs.
 *
 * @return See interface docs.
 */
bool Function::childAdded(Abstract::Block* child)
{
   // Notify this block requires updating.
   Q_UNUSED(child);
   update();

   // Return false to end propagation.
   return false;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @param child See interface docs.
 *
 * @return See interface docs.
 */
bool Function::childRemoved(Abstract::Block* child)
{
   // Notify this block requires updating.
   Q_UNUSED(child);
   update();

   // Return false to end propagation.
   return false;
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
   if ( isDefault() ) ret += QStringLiteral("D");
   if ( isDeleted() ) ret += QStringLiteral("R");
   if ( isQtInvokable() ) ret += QStringLiteral("Q");
   if ( isExplicit() ) ret += QStringLiteral("E");
   if ( isConst() ) ret += QStringLiteral("C");
   if ( isConstExpr() ) ret += QStringLiteral("X");
   if ( isStatic() ) ret += QStringLiteral("S");
   if ( isNoExcept() ) ret += QStringLiteral("N");
   if ( isVirtual() ) ret += QStringLiteral("V");
   if ( isOverride() ) ret += QStringLiteral("O");
   if ( isFinal() ) ret += QStringLiteral("F");
   if ( isAbstract() ) ret += QStringLiteral("A");

   // If the flag string is not empty then surround it with brackets and a space.
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");

   // Return the attributes display string.
   return ret;
}

}
