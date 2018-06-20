#include "cppqt_enumvalue_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given enumeration value block. 
 *
 * @param block Enumeration value block this new view displays. 
 */
EnumValue::View::View(const EnumValue* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns HTML rich text that displays the body of this view's enumeration value 
 * block. 
 *
 * @return HTML rich text that displays the body of this view's enumeration value 
 *         block. 
 */
QString EnumValue::View::displayText()
{
   return displayDescription().append(displayValue());
}






/*!
 * Returns a HTML string that displays the value field for this view's enumeration 
 * value block. If there is no value then this returns an empty string. 
 *
 * @return HTML string that displays the value field for this view's enumeration 
 *         value block or an empty string if there is no value. 
 */
QString EnumValue::View::displayValue()
{
   // Get this view's enumeration value block's value field. If the value returned is 
   // an empty string then return an empty string. 
   QString value {_block->value()};
   if ( value.isEmpty() ) return QString();

   // Return a HTML string that displays this view's enumeration value block's value 
   // field along with a title. 
   return QString("<h3>").append(tr("Value")).append("</h3><p>").append(value).append("</p>");
}
