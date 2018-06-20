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
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int EnumValue::type() const
{
   return BlockFactory::EnumValueType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString EnumValue::name() const
{
   // Create a string _ret_ setting to this block's base name. If this block's value 
   // field is not empty then append an indicator stating as much. 
   QString ret {Base::name()};
   if ( !_value.isEmpty() ) ret.append(" =");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon EnumValue::icon() const
{
   // If this block's static icon _ret_ is null then load it from memory. 
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumvalue.svg");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> EnumValue::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> EnumValue::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int EnumValue::fieldSize() const
{
   return Field::Total;
}






/*!
 * Implements the interface that returns the field type for the given field index 
 * of this block. 
 *
 * @param index Index of the field whose field type is returned. 
 *
 * @return Field type of the given field index of this block. 
 */
AbstractBlock::Field EnumValue::fieldType(int index) const
{
   // Based off the given field index return its type. If the given index is not 
   // defined then call the base class interface returning its return. 
   switch (index)
   {
   case Field::Value: return AbstractBlock::Field::String;
   default: return Base::fieldType(index);
   }
}






/*!
 * Implements the interface that returns the value of the field with the given 
 * index for this block. 
 *
 * @param index Index of the field whose value is returned. 
 *
 * @return Value of the field with the given index for this block. 
 */
QVariant EnumValue::field(int index) const
{
   // Based off the given field index return its value. If the given index is not 
   // defined then call the base class interface returning its return. 
   switch (index)
   {
   case Field::Value: return _value;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> EnumValue::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new enumeration value block with the given name. 
 *
 * @param name The name which is set to this new enumeration value block's base 
 *             name field. 
 */
EnumValue::EnumValue(const QString& name):
   Base(name)
{}






/*!
 */
bool EnumValue::hasValue() const
{
   return !_value.isEmpty();
}






/*!
 * Returns the set value for this enumeration value block. If this block has no set 
 * value an empty string is returned. 
 *
 * @return The set value for this enumeration value block, if any. 
 */
QString EnumValue::value() const
{
   return _value;
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> EnumValue::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new EnumValue);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void EnumValue::fieldModified(int index)
{
   // Based off the given field index notify that this block has been modified. If 
   // the given index is not defined for this block then call the base interface. 
   switch (index)
   {
   case Field::Value:
      notifyModified();
      notifyNameModified();
      break;
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 * Implements the interface that quietly sets the value of the field with the given 
 * index to the new given value. 
 *
 * @param index Index of the field whose value is set to the new given value. 
 *
 * @param value New value that the field with the given index is set to. 
 */
void EnumValue::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. If the 
   // given index is not defined for this block then call the base interface. 
   switch (index)
   {
   case Field::Value:
      setValue(value.toString());
      break;
   default:
      Base::quietlySetField(index,value);
      break;
   }
}






/*!
 * Implements the interface that returns the full list of of all field tag names 
 * for this block that matches the order of this block's field enumeration. 
 *
 * @return Full list of all field tag names for this block. 
 */
QStringList EnumValue::fields() const
{
   // If the static string list _ret_ is empty then populate its list, first with the 
   // fields from the base block class and then with this block's additional fields. 
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }

   // Return _ret_. 
   return ret;
}






/*!
 * Set the value field of this block to the given value, making sure it has valid 
 * C++ type syntax. If the given value does not have valid syntax then an exception 
 * is thrown. 
 *
 * @param value  
 */
void EnumValue::setValue(const QString& value)
{
   _value = value;
}
