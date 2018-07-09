#include "cppqt_base.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "common.h"



using namespace std;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. 
 */
const QStringList Base::_fields {"name","description"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& Base::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Base::name() const
{
   return _name;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Base::fieldSize() const
{
   return Field::Total;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
AbstractBlock::Field Base::fieldType(int index) const
{
   // Based off the given index return its field type. 
   switch (index)
   {
   case Field::Name:
   case Field::Description: return AbstractBlock::Field::String;

   // If the given index is unknown then throw an exception. 
   default:
      {
         Exception::OutOfRange e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Given block field index %1 is out of range (%2 max).")
                      .arg(index)
                      .arg(fieldSize() - 1));
         throw e;
      }
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QVariant Base::field(int index) const
{
   // Based off the given index return its field value. 
   switch (index)
   {
   case Field::Name: return _name;
   case Field::Description: return _description;

   // If the given index is unknown then throw an exception. 
   default:
      {
         Exception::OutOfRange e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Given block field index %1 is out of range (%2 max).")
                      .arg(index)
                      .arg(fieldSize() - 1));
         throw e;
      }
   }
}






/*!
 * Returns this block's description field. 
 *
 * @return This block's description field. 
 */
QString Base::description() const
{
   return _description;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Base::version() const
{
   return 0;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Base::fieldTag(int index) const
{
   // Use this block's fields interface to figure out the tag at the given index. 
   return fields().at(index);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param name See interface docs. 
 *
 * @return See interface docs. 
 */
int Base::fieldIndexOf(const QString& name) const
{
   // Use this block's fields interface to figure out the index of the given tag 
   // name. 
   return fields().indexOf(name);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Base::fieldModified(int index)
{
   // Notify of modification regardless of specific field type. 
   notifyModified();

   // Based off the given field index notify that this block's body or name has been 
   // modified. 
   switch (index)
   {
   case Field::Name:
      notifyNameModified();
      break;
   case Field::Description:
      notifyBodyModified();
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
void Base::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. 
   switch (index)
   {
   case Field::Name:
      setName(value.toString());
      break;
   case Field::Description:
      _description = value.toString();
      break;
   }
}






/*!
 * This interface returns the full list of of all field tag names for this block 
 * that matches the order of this block's field enumeration. The default 
 * implementation returns the two base fields for this base class. 
 *
 * @return Full list of all field tag names for this block. 
 */
QStringList Base::fields() const
{
   return _fields;
}






/*!
 * This interface checks to make sure the given name is a valid name for this block 
 * type, returning true if it is valid. This implementation makes sure it is a 
 * basic C++ alphanumeric name. 
 *
 * @param value The name value whose syntax is checked to be valid or not. 
 *
 * @return True if the given name is valid or false otherwise. 
 */
bool Base::checkName(const QString& value)
{
   // Use a regular expression to determine if the given string has correct syntax. 
   return QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*").exactMatch(value);
}






/*!
 * Sets this block's name field, making sure the given new value has correct 
 * syntax. If the syntax is not correct an exception is thrown. 
 *
 * @param value New value for this block's name field. 
 */
void Base::setName(const QString& value)
{
   // Make sure the given value has correct syntax. 
   if ( !checkName(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid name '%1'.").arg(value));
      throw e;
   }

   // Set this block's name field to the new value given. 
   _name = value;
}
