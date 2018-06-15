#include "cppqt_using.h"
#include <exception.h>
#include "cppqt_using_view.h"
#include "cppqt_using_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Using::_fields {"field"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Using::type() const
{
   return BlockFactory::UsingType;
}






/*!
 * Implements the interface that returns a reference to this block's factory which 
 * produces all block types for this project type. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Using::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Using::name() const
{
   return "using";
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Using::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/declaration.svg");
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Using::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Using::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Using::fieldSize() const
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
AbstractBlock::Field Using::fieldType(int index) const
{
   switch (index)
   {
   case Field::FieldType: return AbstractBlock::Field::String;
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
 * @param index Index of the field whose value is returned. 
 *
 * @return Value of the field with the given index for this block. 
 */
QVariant Using::field(int index) const
{
   switch (index)
   {
   case Field::FieldType: return _field;
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
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Using::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 *
 * @param field  
 */
Using::Using(const QString& field):
   _field(field)
{
   checkField(field);
}






/*!
 */
QString Using::fieldName() const
{
   return _field;
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Using::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Using);
}






/*!
 * Implements the interface that returns the current data version for this block 
 * type. 
 *
 * @return Current data version. 
 */
int Using::version() const
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
QString Using::fieldTag(int index) const
{
   return _fields.at(index);
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
int Using::fieldIndexOf(const QString& name) const
{
   return _fields.indexOf(name);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Using::fieldModified(int index)
{
   switch (index)
   {
   case Field::FieldType:
      notifyModified();
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
void Using::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::FieldType:
      setField(value.toString());
      break;
   }
}






/*!
 *
 * @param value  
 */
void Using::checkField(const QString& value)
{
   if ( !QRegExp("((::)?[a-zA-Z_]+[a-zA-Z_0-9]*)+").exactMatch(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid using field '%1'.").arg(value));
      throw e;
   }
}






/*!
 *
 * @param value  
 */
void Using::setField(const QString& value)
{
   checkField(value);
   _field = value;
}
