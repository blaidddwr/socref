#include "cppqt_base.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace CppQt;
//



/*!
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
 */
int Base::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Base::fieldType(int index) const
{
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
 *
 * @param index  
 */
QVariant Base::field(int index) const
{
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
 *
 * @param name  
 */
Base::Base(const QString& name):
   _name(name)
{}






/*!
 */
QString Base::description() const
{
   return _description;
}






/*!
 * This interface returns the current version number of XML elements written for 
 * this block type. 
 *
 * @return Current version number. 
 */
int Base::version() const
{
   return 0;
}






/*!
 *
 * @param index  
 */
QString Base::fieldTag(int index) const
{
   return fields().at(index);
}






/*!
 *
 * @param name  
 */
int Base::fieldIndexOf(const QString& name) const
{
   return fields().indexOf(name);
}






/*!
 *
 * @param index  
 */
void Base::fieldModified(int index)
{
   notifyModified();
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
 *
 * @param index  
 *
 * @param value  
 */
void Base::quietlySetField(int index, const QVariant& value)
{
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
 */
QStringList Base::fields() const
{
   return _fields;
}






/*!
 *
 * @param value  
 */
void Base::setName(const QString& value)
{
   if ( QRegExp("[a-zA-Z_]+((::)?[a-zA-Z0-9_])*").exactMatch(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid name '%1'.").arg(value));
      throw e;
   }
   _name = value;
}
