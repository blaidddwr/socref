#include "cppqt_enumvalue.h"
#include <exception.h>
#include "cppqt_enumvalue_view.h"
#include "cppqt_enumvalue_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. This is in addition to the base fields this block 
 * inherits. 
 */
const QStringList EnumValue::_fields {"value"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int EnumValue::type() const
{
   return BlockFactory::EnumValueType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString EnumValue::name() const
{
   // Create a string setting it to this block's base name. 
   QString ret {Base::name()};

   // If this enumeration value has a set value then append that info to the string. 
   if ( !_value.isEmpty() ) ret.append(" =");

   // Return the name string. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon EnumValue::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/enumvalue.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> EnumValue::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> EnumValue::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int EnumValue::fieldSize() const
{
   // Use the field enumeration to return the total number of fields. 
   return Field::Total;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
AbstractBlock::Field EnumValue::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::Value: return AbstractBlock::Field::String;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default: return Base::fieldType(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QVariant EnumValue::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::Value: return _value;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default: return Base::field(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> EnumValue::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Tests if this enumeration value block has a set value or not, returning true if 
 * it does. 
 *
 * @return True of this enumeration value block has a set value or false otherwise. 
 */
bool EnumValue::hasValue() const
{
   // Test if this block's set value is not empty to determine if it has a value. 
   return !_value.isEmpty();
}






/*!
 * Returns the set value for this enumeration value block. If this block has no set 
 * value an empty string is returned. 
 *
 * @return The set value for this enumeration value block or an empty string if it 
 *         has none. 
 */
QString EnumValue::value() const
{
   return _value;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> EnumValue::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new EnumValue);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void EnumValue::fieldModified(int index)
{
   // Based off the given field index notify the changes to this block. 
   switch (index)
   {
   case Field::Value:
      notifyModified();
      notifyNameModified();
      break;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @param value See interface docs. 
 */
void EnumValue::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. 
   switch (index)
   {
   case Field::Value:
      _value = value.toString();
      break;

   // If the given index is unknown for this block then call its base class 
   // interface. 
   default:
      Base::quietlySetField(index,value);
      break;
   }
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @return See interface docs. 
 */
QStringList EnumValue::fields() const
{
   // Initialize an empty static string list. 
   static QStringList ret;

   // If the string list is empty then populate it. 
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }

   // Return the combined fields string list. 
   return ret;
}
