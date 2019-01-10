#include "glsl_function_view.h"
#include "glsl_variable.h"



using namespace GLSL;
//






/*!
 * Constructs a new view with the given function block. 
 *
 * @param block The function block that this new view displays. 
 */
Function::View::View(const Function* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays all variable arguments for this view's function 
 * block. If there are no arguments an empty string is returned. 
 *
 * @return Rich text that displays all variable arguments for this view's function 
 *         block or an empty string if there are no arguments to display. 
 */
QString Function::View::displayArguments()
{
   // Create an empty return string. 
   QString ret;

   // Make a pointer list of all children variables of this view's function block and 
   // make sure it is not empty. 
   const QList<Variable*> list {_block->arguments()};
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
 * Returns rich text that displays the return type and description for this view's 
 * function block. If the return type is void then an empty string is returned. 
 *
 * @return Rich text that displays the return type and description for this view's 
 *         function block or an empty string if the return type is void. 
 */
QString Function::View::displayReturn()
{
   // Get the return type of this view's function block. 
   QString returnType {_block->returnType()};

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
             + _block->returnDescription()
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
QString Function::View::displayOperations()
{
   return tr("<i>%n operation(s)</i>",nullptr,_block->operations().size());
}
