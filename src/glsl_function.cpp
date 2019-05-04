#include "glsl_function.h"
#include "glsl_functionview.h"
#include "glsl_factory.h"
#include "glsl_variable.h"



namespace GLSL
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

   // If this block's void return indicator is not true then append a return
   // indicator.
   if ( !isVoidReturn() ) ret += QStringLiteral("... ");

   // Append the function name and then the number of arguments.
   const QList<Variable*> list {arguments()};
   ret += QString("%1(%2)").arg(baseName()).arg(list.size());

   // Return the full display name string.
   return ret;
}






/*!
 * Returns the return type of this function block.
 *
 * @return Return type of this function block.
 */
QString Function::returnType() const
{
   return getString("type");
}






/*!
 * Returns the return description of this function block.
 */
QString Function::returnDescription() const
{
   return getString("return_description");
}






/*!
 * Returns the full list of all operations of this function block.
 *
 * @return Full list of all operations for this function block.
 */
QStringList Function::operations() const
{
   return getStringList("operation");
}






/*!
 * Tests if this function block's return block is void and therefore has no
 * return, returning true if it is.
 *
 * @return True if this function block's return type is void or false otherwise.
 */
bool Function::isVoidReturn() const
{
   return returnType() == QStringLiteral("void");
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
   // Notify that this block requires updating.
   Q_UNUSED(child)
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
   // Notify that this block requires updating.
   Q_UNUSED(child)
   update();

   // Return false to end propagation.
   return false;
}

}
