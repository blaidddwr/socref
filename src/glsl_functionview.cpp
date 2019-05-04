#include "glsl_functionview.h"
#include "glsl_function.h"
#include "glsl_variable.h"



namespace GLSL
{






/*!
 * Constructs a new view with the given function block.
 *
 * @param block The function block that this new view displays.
 */
FunctionView::FunctionView(const Function* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays all variable arguments for this view's
 * function block. If there are no arguments an empty string is returned.
 *
 * @return Rich text that displays all variable arguments for this view's
 *         function block or an empty string if there are no arguments to
 *         display.
 */
QString FunctionView::displayArguments()
{
   // Create an empty return string.
   QString ret;

   // Make a pointer list of all children variables of this view's function block and
   // make sure it is not empty.
   const QList<Variable*> list {block<Function>().arguments()};
   if ( list.isEmpty() ) return ret;

   // Append a title.
   ret += QStringLiteral("<h3>") + tr("Arguments") + QStringLiteral("</h3><p>");

   // Iterate through all child variable blocks, appending a description of each
   // variable into the returned rich text.
   for (auto variable : list)
   {
      ret += QStringLiteral("<p><u>")
           + variable->typeString().replace("<","&lt;")
           + QStringLiteral("</u> <b>")
           + variable->baseName()
           + QStringLiteral("</b>: ")
           + variable->description();
   }

   // Return the arguments rich text.
   return ret;
}






/*!
 * Returns rich text that displays the return type and description for this
 * view's function block. If the return type is void then an empty string is
 * returned.
 *
 * @return Rich text that displays the return type and description for this
 *         view's function block or an empty string if the return type is void.
 */
QString FunctionView::displayReturn()
{
   // Get the return type of this view's function block.
   QString returnType {block<Function>().returnType()};

   // If the return type is empty or is equal to void then return an empty string.
   if ( returnType.isEmpty() || returnType == QString("void") ) return QString();

   // Else this view's function block has a return type so return rich text
   // displaying that return type and its description.
   else
   {
      return QStringLiteral("<h3>")
             + tr("Return")
             + QStringLiteral("</h3><p><u>")
             + returnType + QStringLiteral("</u> : ")
             + block<Function>().returnDescription()
             + QStringLiteral("</p>");
   }
}






/*!
 * Returns rich text that displays the number of operations for this view's
 * function block.
 *
 * @return Rich text that displays the number of operations for this view's
 *         function block.
 */
QString FunctionView::displayOperations()
{
   return tr("<i>%n operation(s)</i>",nullptr,block<Function>().operations().size());
}

}
