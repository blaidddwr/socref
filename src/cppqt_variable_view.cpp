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
   // Create and return a string that contains HTML for this object's variable type 
   // field along with a title. Replace any special carrot characters so it doesn't 
   // break the HTML code. 
   QString ret {"<h3>"};
   ret.append(tr("Type")).append("</h3><p>");
   ret.append(_block->variableType().replace("<","&lt;")).append("</p>");
   return ret;
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
   // Create an empty string _ret_ and then an empty string list _list_. Append any 
   // properties this object's variable block has set to _list_. If _list_ is empty 
   // then return _ret_. 
   QString ret;
   QStringList list;
   if ( _block->isConstExpr() ) list << "constexpr";
   if ( _block->isStatic() ) list << "static";
   if ( _block->isMutable() ) list << "mutable";
   if ( list.isEmpty() ) return ret;

   // Append an HTML title, then all set properties as an HTML list to _ret_, and 
   // then return _ret_. 
   ret.append("<h3>")
      .append(tr("Properties"))
      .append("</h3><ul><li>")
      .append(list.join("</li><li>"))
      .append("</li></ul>");
   return ret;
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
   // Create an empty string _ret_. If this view's variable block has no initial 
   // value then return _ret_. 
   QString ret;
   if ( !_block->hasInitializer() ) return ret;

   // Append an HTML title and then the initial value as an HTML list. Separate the 
   // initial value as a list using the comma as a separator. 
   const QStringList parts {_block->initializer().split(",")};
   ret.append("<h3>").append(tr("Initializer")).append("</h3>");
   for (auto part : parts) ret.append(part).append("<br/>");

   // Return _ret_. 
   return ret;
}
