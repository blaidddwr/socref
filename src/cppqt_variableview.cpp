#include "cppqt_variableview.h"
#include "cppqt_variable.h"



namespace CppQt
{






/*!
 * Constructs a new view object with the given variable block.
 *
 * @param block Variable block this new view displays.
 */
VariableView::VariableView(const Variable* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays any properties this view's variable block has
 * set. If this view's variable block has no properties set then this returns an
 * empty string.
 *
 * @return Rich text that displays any properties this view's variable block has
 *         set or an empty string if no properties are set.
 */
QString VariableView::displayProperties()
{
   // Create an empty string list, appending any properties this view's variable
   // block has set.
   QStringList list;
   if ( block<Variable>().isConstExpr() ) list << "constexpr";
   if ( block<Variable>().isStatic() ) list << "static";
   if ( block<Variable>().isMutable() ) list << "mutable";
   if ( block<Variable>().isThreadLocal() ) list << "thread_local";

   // If the string list is empty then return an empty string.
   if ( list.isEmpty() ) return QString();

   // Else this view's block has set properties so return a HTML string displaying
   // those set properties.
   else return tr("<h3>Properties</h3><ul><li>%1</li></ul>").arg(list.join("</li><li>"));
}






/*!
 * Returns rich text that displays the initial value of this view's variable
 * block. If the variable block has no initial value then an empty string is
 * returned.
 *
 * @return Rich text that displays the initial value of this view's variable
 *         block or an empty string if there is no initial value.
 */
QString VariableView::displayInitializer()
{
   // Create an empty string.
   QString ret;

   // Make sure this view's block has an initial value.
   if ( !block<Variable>().hasInitializer() ) return ret;

   // Append a HTML string that displays this view's variable block's initial value,
   // separating that value by the comma delimiter.
   const QStringList parts {block<Variable>().initializer().split(",")};
   ret.append(tr("<h3>Initializer</h3>"));
   for (auto part : parts) ret.append(part).append("<br/>");

   // Return the initial value HTML string.
   return ret;
}

}
