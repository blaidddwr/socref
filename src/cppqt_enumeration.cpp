#include "cppqt_enumeration.h"
#include <exception.h>
#include "cppqt_enumeration_view.h"
#include "cppqt_enumeration_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_enumvalue.h"
#include "domelementreader.h"
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
const QStringList Enumeration::_fields {"class"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Enumeration::type() const
{
   return BlockFactory::EnumerationType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Enumeration::name() const
{
   // Create a new string _ret_ setting it to this block's base name. If this 
   // enumeration is a class then append that to _ret_. 
   QString ret {Base::name()};
   if ( _class ) ret.append(" [C]");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Enumeration::icon() const
{
   // If the static qt icon _ret_ is null then lead this block's icon. 
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumeration.svg");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Enumeration::buildList() const
{
   return QList<int>{BlockFactory::EnumValueType};
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Enumeration::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Enumeration::fieldSize() const
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
AbstractBlock::Field Enumeration::fieldType(int index) const
{
   // Based off the given field index return its type. If the given index is not 
   // defined then call the base class interface returning its return. 
   switch (index)
   {
   case Field::Class: return AbstractBlock::Field::Boolean;
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
QVariant Enumeration::field(int index) const
{
   // Based off the given field index return its value. If the given index is not 
   // defined then call the base class interface returning its return. 
   switch (index)
   {
   case Field::Class: return _class;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Enumeration::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new enumeration block with the given name. 
 *
 * @param name The name that this new enumeration block's base name field is set 
 *             to. 
 */
Enumeration::Enumeration(const QString& name):
   Base(name)
{}






/*!
 * Tests of this enumeration is a class, returning true if it is. 
 *
 * @return True if this enumeration is a class or false otherwise. 
 */
bool Enumeration::isClass() const
{
   return _class;
}






/*!
 * Returns a pointer list of all enumeration value blocks this enumeration block 
 * contains. 
 *
 * @return Pointer list of all enumeration value blocks this enumeration contains. 
 */
QList<EnumValue*> Enumeration::values() const
{
   return makeListOfType<EnumValue>(BlockFactory::EnumValueType);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Enumeration::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Enumeration);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Enumeration::fieldModified(int index)
{
   // Based off the given field index notify that this block has been modified. If 
   // the given index is not defined for this block then call the base interface. 
   switch (index)
   {
   case Field::Class:
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
void Enumeration::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. If the 
   // given index is not defined for this block then call the base interface. 
   switch (index)
   {
   case Field::Class:
      _class = value.toBool();
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
QStringList Enumeration::fields() const
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
 * Implements _CppQt::Base::checkName_. This allows an empty string to be valid as 
 * a given name for anonymous enumerations. 
 *
 * @param value The name value whose syntax is checked to be valid or not. 
 *
 * @return True if the given name is valid or false otherwise. 
 */
bool Enumeration::checkName(const QString& value)
{
   if ( value.isEmpty() ) return true;
   else return Base::checkName(value);
}
