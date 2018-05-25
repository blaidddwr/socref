#include "cppqt_enumvalue.h"
#include <exception.h>
#include "cppqt_view_enumvalue.h"
#include "cppqt_edit_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
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
   QString ret {Base::name()};
   if ( !_value.isEmpty() ) ret.append(" = ").append(_value);
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon EnumValue::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumvalue.svg");
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
   return unique_ptr<QWidget>(new View::EnumValue(this));
}






/*!
 */
int EnumValue::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field EnumValue::fieldType(int index) const
{
   switch (index)
   {
   case Field::Value: return AbstractBlock::Field::String;
   default: return Base::fieldType(index);
   }
}






/*!
 *
 * @param index  
 */
QVariant EnumValue::field(int index) const
{
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
   return unique_ptr<AbstractEdit>(new Edit::EnumValue(this));
}






/*!
 *
 * @param name  
 */
EnumValue::EnumValue(const QString& name):
   Base(name)
{}






/*!
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
 *
 * @param index  
 */
void EnumValue::fieldModified(int index)
{
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
 *
 * @param index  
 *
 * @param value  
 */
void EnumValue::quietlySetField(int index, const QVariant& value)
{
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
 */
QStringList EnumValue::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }
   return ret;
}






/*!
 *
 * @param value  
 */
void EnumValue::setValue(const QString& value)
{
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid enumeration value '%1'.").arg(value));
      throw e;
   }
   _value = value;
}
