#include "cppqt_variable_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new type view object with the given variable block. 
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and return a string that contains HTML for this object's variable type 
 *    field along with a title. Replace any special carrot characters so it doesn't 
 *    break the HTML code. 
 */
QString Variable::View::displayType()
{
   // 1
   QString ret {"<h3>Type</h3><p>"};
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an empty string _ret_ and then an empty string list _list_. Append any 
 *    properties this object's variable block has set to _list_. If _list_ is empty 
 *    then return _ret_. 
 *
 * 2. Append an HTML title, then all set properties as an HTML list to _ret_, and 
 *    then return _ret_. 
 */
QString Variable::View::displayProperties()
{
   // 1
   QString ret;
   QStringList list;
   if ( _block->isConstExpr() ) list << "constexpr";
   if ( _block->isStatic() ) list << "static";
   if ( _block->isMutable() ) list << "mutable";
   if ( list.isEmpty() ) return ret;

   // 2
   ret.append("<h3>Properties</h3><ul><li>").append(list.join("</li><li>")).append("</li></ul>");
   return ret;
}






/*!
 * Returns a HTML string that displays the initial value of this view's variable 
 * block. If the variable block has no initial value then an empty string is 
 * returned. 
 *
 * @return HTML string that displays the initial value of this view's variable 
 *         block or an empty string if there is no initial value. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an empty string _ret_. If this view's variable block has no initial 
 *    value then return _ret_. 
 *
 * 2. Append an HTML title and then the initial value as an HTML list. Separate the 
 *    initial value as a list using the comma as a separator. 
 *
 * 3. Return _ret_. 
 */
QString Variable::View::displayInitializer()
{
   // 1
   QString ret;
   if ( !_block->hasInitializer() ) return ret;

   // 2
   const QStringList parts {_block->initializer().split(",")};
   ret.append("<h3>Initializer</h3>");
   for (auto part : parts) ret.append(part).append("<br/>");

   // 3
   return ret;
}
