#include "cppqt_variable_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given variable block. 
 *
 * @param block Variable block this new view displays. 
 */
Variable::View::View(const Variable* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's variable block. 
 *
 * @return HTML rich text that displays the body of this view's variable block. 
 */
QString Variable::View::displayText()
{
   // Return as HTML this view's variable block's type, description, properties, and 
   // initial value in that order. 
   return displayType().append(displayDescription())
                       .append(displayProperties())
                       .append(displayInitializer());
}






/*!
 * Returns a HTML string that displays the type field for this view's variable 
 * block. 
 *
 * @return HTML string that displays the type field of this view's variable block. 
 */
QString Variable::View::displayType()
{
   // Create and return a HTML string that displays this view's variable block type 
   // field. All special characters must be replaced to preserve the HTML. 
   return tr("<h3>Type</h3><p>%1</p>").arg(_block->variableType().replace("<","&lt;"));
}






/*!
 * Returns a HTML string that displays any properties this view's variable block 
 * has set. If this view's variable block has no properties set then this returns 
 * an empty string. 
 *
 * @return HTML string that displays any properties this view's variable block has 
 *         set or an empty string if no properties are set. 
 */
QString Variable::View::displayProperties()
{
   // Create an empty string list, appending any properties this view's variable 
   // block has set. 
   QStringList list;
   if ( _block->isConstExpr() ) list << "constexpr";
   if ( _block->isStatic() ) list << "static";
   if ( _block->isMutable() ) list << "mutable";
   if ( _block->isThreadLocal() ) list << "thread_local";

   // If the string list is empty then return an empty string. 
   if ( list.isEmpty() ) return QString();

   // Else this view's block has set properties so return a HTML string displaying 
   // those set properties. 
   else return tr("<h3>Properties</h3><ul><li>%1</li></ul>").arg(list.join("</li><li>"));
}






/*!
 * Returns a HTML string that displays the initial value of this view's variable 
 * block. If the variable block has no initial value then an empty string is 
 * returned. 
 *
 * @return HTML string that displays the initial value of this view's variable 
 *         block or an empty string if there is no initial value. 
 */
QString Variable::View::displayInitializer()
{
   // Create an empty string. 
   QString ret;

   // Make sure this view's block has an initial value. 
   if ( !_block->hasInitializer() ) return ret;

   // Append a HTML string that displays this view's variable block's initial value, 
   // separating that value by the comma delimiter. 
   const QStringList parts {_block->initializer().split(",")};
   ret.append(tr("<h3>Initializer</h3>"));
   for (auto part : parts) ret.append(part).append("<br/>");

   // Return the initial value HTML string. 
   return ret;
}
