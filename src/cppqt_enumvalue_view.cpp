#include "cppqt_enumvalue_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given enumeration value block. 
 *
 * @param block Enumeration value block this new view displays. 
 */
EnumValue::View::View(const EnumValue* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays the value field for this view's enumeration 
 * value block. If there is no value then this returns an empty string. 
 *
 * @return Rich text that displays the value field for this view's enumeration 
 *         value block or an empty string if there is no value. 
 */
QString EnumValue::View::displayValue()
{
   // Get this view's enumeration value block's value field and make sure it is not 
   // empty. 
   QString value {_block->value()};
   if ( value.isEmpty() ) return QString();

   // Return the rich text that displays this view's enumeration value block's value 
   // field along with a title. 
   return tr("<h3>Value</h3><p>%1</p>").arg(value);
}
