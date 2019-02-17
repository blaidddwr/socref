#include "cppqt_functionview.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"



namespace CppQt
{






/*!
 * Constructs a new view object with the given function block.
 *
 * @param block Function block this new view displays.
 */
FunctionView::FunctionView(const Function* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays any arguments this view's function block
 * contains. If this view's function block has no arguments then this returns an
 * empty string.
 *
 * @return Rich text that displays any arguments this view's function block
 *         contains or an empty string if there are no arguments.
 */
QString FunctionView::displayArguments()
{
   // Create an empty return string.
   QString ret;

   // Make a pointer list of all children variables of this view's function block and
   // make sure it is not empty.
   const QList<CppQt::Variable*> list {block<Function>().arguments()};
   if ( list.isEmpty() ) return ret;

   // Append a title.
   ret += QStringLiteral("<h3>") + tr("Arguments") + QStringLiteral("</h3>");

   // Iterate through all child variable blocks.
   for (auto variable : list)
   {
      // If the child block has an initial value then add its rich text.
      QString initializer;
      if ( variable->hasInitializer() ) initializer = QString(" = ").append(variable->initializer());

      // Append the child variable information as rich text including the possible
      // initial value.
      ret += QStringLiteral("<p><u>")
           + variable->variableType().replace("<","&lt;")
           + QStringLiteral("</u> <b>")
           + variable->baseName()
           + QStringLiteral("</b>")
           + initializer
           + QStringLiteral(" : ")
           + variable->description()
           + QStringLiteral("</p>");
   }

   // Return the arguments rich text.
   return ret;
}






/*!
 * Wrapper method that calls the static method in _CppQt::Template_ of the same
 * name.
 *
 * @return See wrapped method docs.
 */
QString FunctionView::displayTemplates()
{
   return Template::displayTemplates(block<Function>());
}






/*!
 * Returns rich text that displays any properties this view's function block has
 * set. If this view's function block has no properties set then this returns an
 * empty string.
 *
 * @return Rich text that displays any properties this view's function block has
 *         set or an empty string if no properties are set.
 */
QString FunctionView::displayProperties()
{
   // Create an empty string list, appending any properties this view's function
   // block has set.
   QStringList list;
   if ( block<Function>().isDefault() ) list << tr("Default");
   if ( block<Function>().isDeleted() ) list << tr("Deleted");
   if ( block<Function>().isQtInvokable() ) list << tr("Qt Invokable");
   if ( block<Function>().isExplicit() ) list << tr("Explicit");
   if ( block<Function>().isVirtual() ) list << tr("Virtual");
   if ( block<Function>().isConst() ) list << tr("Constant");
   if ( block<Function>().isConstExpr() ) list << tr("Constant Expression");
   if ( block<Function>().isStatic() ) list << tr("Static");
   if ( block<Function>().isNoExcept() ) list << tr("No Exceptions");
   if ( block<Function>().isOverride() ) list << tr("Override");
   if ( block<Function>().isFinal() ) list << tr("Final");
   if ( block<Function>().isAbstract() ) list << tr("Abstract(=0)");

   // If the string list is empty then return an empty string.
   if ( list.isEmpty() ) return QString();

   // Else this view's block has set properties so return rich text displaying those
   // set properties.
   else
   {
      return QStringLiteral("<h3>")
             + tr("Properties")
             + QStringLiteral("</h3><ul><li>")
             + list.join("</li><li>")
             + QStringLiteral("</li></ul>");
   }
}






/*!
 * Returns rich text that displays the return type field for this view's
 * function block.
 *
 * @return Rich text that displays the return type field of this view's function
 *         block.
 */
QString FunctionView::displayReturn()
{
   // Get the return type of this view's function block.
   QString returnType {block<Function>().returnType()};

   // If the return type is empty or is equal to "void" then return an empty string.
   if ( returnType.isEmpty() || returnType == QString("void") ) return QString();

   // Else this view's function block has a return type so return rich text
   // displaying that return type and its description.
   else
   {
      return QStringLiteral("<h3>")
             + tr("Return")
             + QStringLiteral("</h3><p><u>")
             + returnType.replace("<","&lt;")
             + QStringLiteral("</u> : ")
             + block<Function>().returnDescription()
             + QStringLiteral("</p>");
   }
}






/*!
 * Returns rich text that displays the number of operations of this view's
 * function block.
 *
 * @return Rich text that displays the number of operations of this view's
 *         function block.
 */
QString FunctionView::displayOperations()
{
   // Return rich text displaying the number of operations.
   return tr("<i>%n operation(s)</i>",nullptr,block<Function>().operations().size());
}

}
