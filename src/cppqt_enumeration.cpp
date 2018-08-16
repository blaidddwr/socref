#include "cppqt_enumeration.h"
#include <socutil/sut_exceptions.h>
#include "cppqt_enumeration_view.h"
#include "cppqt_enumeration_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_enumvalue.h"
#include "common.h"



using namespace Sut;
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
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Enumeration::type() const
{
   return BlockFactory::EnumerationType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Enumeration::name() const
{
   // Create a new string setting it to this block's base name. 
   QString ret {Base::name()};

   // If this enumeration is a class append that info to the string. 
   if ( _class ) ret.append(" [C]");

   // Return the name string. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Enumeration::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/enumeration.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Enumeration::buildList() const
{
   return QList<int>{BlockFactory::EnumValueType};
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> Enumeration::makeView() const
{
   return QPtr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Enumeration::fieldSize() const
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
AbstractBlock::Field Enumeration::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::Class: return AbstractBlock::Field::Boolean;

   // If the given index is unknown for this block then call its base interface. 
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
QVariant Enumeration::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::Class: return _class;

   // If the given index is unknown for this block then call its base interface. 
   default: return Base::field(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> Enumeration::makeEdit()
{
   return QPtr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new enumeration block with a default state or null state based off 
 * the given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Enumeration::Enumeration(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setName(QStringLiteral("enum_list"));
}






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
   // Make a pointer list of all of this block's enumeration value child blocks and 
   // return it. 
   return makeListOfType<EnumValue>(BlockFactory::EnumValueType);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> Enumeration::makeBlank() const
{
   return QPtr<AbstractBlock>(new Enumeration);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Enumeration::fieldModified(int index)
{
   // Based off the given field index notify the changes to this block. 
   switch (index)
   {
   case Field::Class:
      notifyModified();
      notifyNameModified();
      break;

   // If the given index is unknown for this block then call its base interface. 
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
void Enumeration::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. 
   switch (index)
   {
   case Field::Class:
      _class = value.toBool();
      break;

   // If the given index is unknown for this block then call its base interface. 
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
QStringList Enumeration::fields() const
{
   // Initialize an empty static string list. 
   static QStringList ret;

   // If the static string list is empty then populate it. 
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }

   // Return the combined fields list. 
   return ret;
}






/*!
 * Implements _CppQt::Base::checkName_ interface. This implementation allows an 
 * empty string to be valid as a given name for anonymous enumerations. 
 *
 * @param value See interface docs. 
 *
 * @return See interface docs. 
 */
bool Enumeration::checkName(const QString& value)
{
   // If the given value is empty then return true else call and return this block's 
   // base interface. 
   if ( value.isEmpty() ) return true;
   else return Base::checkName(value);
}
