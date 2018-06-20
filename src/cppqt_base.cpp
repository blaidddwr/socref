#include "cppqt_base.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
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
 * Implements the interface that returns a reference to this block's factory. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Base::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Base::name() const
{
   return _name;
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Base::fieldSize() const
{
   return Field::Total;
}






/*!
 * Implements the interface that returns the field type for the given field index 
 * of this block. 
 *
 * @param index  
 *
 * @return Field type of the given field index of this block. 
 */
AbstractBlock::Field Base::fieldType(int index) const
{
   // Based off the given index return its field type. If the given index is out of 
   // range then throw an exception. 
   switch (index)
   {
   case Field::Name:
   case Field::Description: return AbstractBlock::Field::String;
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
 * Implements the interface that returns the value of the field with the given 
 * index for this block. 
 *
 * @param index  
 *
 * @return Value of the field with the given index for this block. 
 */
QVariant Base::field(int index) const
{
   // Based off the given index return its field value. If the given index is out of 
   // range then throw an exception. 
   switch (index)
   {
   case Field::Name: return _name;
   case Field::Description: return _description;
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
 * Constructs a new base object with the given name. 
 *
 * @param name  
 */
Base::Base(const QString& name):
   _name(name)
{}






/*!
 * Returns this object's description field. 
 *
 * @return This object's description field. 
 */
QString Base::description() const
{
   return _description;
}






/*!
 * Implements the interface that returns the current data version for this block 
 * type. 
 *
 * @return Current version number. 
 */
int Base::version() const
{
   return 0;
}






/*!
 * Implements the interface that returns the tag name for the field with the given 
 * index for this block. 
 *
 * @param index Index of the field whose tag name is returned. 
 *
 * @return Tag name for the field with the given index for this block. 
 */
QString Base::fieldTag(int index) const
{
   return fields().at(index);
}






/*!
 * Implements the interface that returns the index of the field that has the given 
 * tag name for this block. 
 *
 * @param name Tag name of the field whose index is returned. 
 *
 * @return Index of the field with the given tag name or -1 if no field exists with 
 *         that tag name. 
 */
int Base::fieldIndexOf(const QString& name) const
{
   return fields().indexOf(name);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Base::fieldModified(int index)
{
   // Notify that his block has been modified. 
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
 * Implements the interface that quietly sets the value of the field with the given 
 * index to the new given value. 
 *
 * @param index Index of the field whose value is set to the new given value. 
 *
 * @param value New value that the field with the given index is set to. 
 */
void Base::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new given value. 
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
   // If the syntax of the given new name value is not valid then throw an exception, 
   // else set this block's name field to the new value. 
   if ( !checkName(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid name '%1'.").arg(value));
      throw e;
   }
   _name = value;
}
