#include "cppqt_using.h"
#include <QRegularExpression>
#include "cppqt_using_view.h"
#include "cppqt_using_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace Sut;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. 
 */
const QStringList Using::_fields {"field"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Using::type() const
{
   return BlockFactory::UsingType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& Using::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Using::name() const
{
   return "using";
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Using::icon() const
{
   // Initialize the static icons for this block type. 
   static QIcon ret(":/icons/declaration.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Using::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> Using::makeView() const
{
   return QPtr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Using::fieldSize() const
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
AbstractBlock::Field Using::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::FieldType: return AbstractBlock::Field::String;

   // If the given index is unknown then throw an exception. 
   default:
      {
         Exception::OutOfRange e;
         SUT_MARK_EXCEPTION(e);
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
QVariant Using::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::FieldType: return _field;

   // If the given index is unknown then throw an exception. 
   default:
      {
         Exception::OutOfRange e;
         SUT_MARK_EXCEPTION(e);
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
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> Using::makeEdit()
{
   return QPtr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new using block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Using::Using(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setField(QStringLiteral("space"));
}






/*!
 * Returns the field of this using block. 
 *
 * @return The field of this using block. 
 */
QString Using::fieldName() const
{
   return _field;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> Using::makeBlank() const
{
   return QPtr<AbstractBlock>(new Using);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Using::version() const
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
QString Using::fieldTag(int index) const
{
   // Use this block's field list to return the tag name with the given index. 
   return _fields.at(index);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param name See interface docs. 
 *
 * @return See interface docs. 
 */
int Using::fieldIndexOf(const QString& name) const
{
   // Use this block's field list to determine the index of the given tag name. 
   return _fields.indexOf(name);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Using::fieldModified(int index)
{
   // Based off the given field index notify about changes to this block. If the 
   // given index is unknown then do nothing. 
   switch (index)
   {
   case Field::FieldType:
      notifyModified();
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
void Using::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. If the 
   // given index is unknown then do nothing. 
   switch (index)
   {
   case Field::FieldType:
      setField(value.toString());
      break;
   }
}






/*!
 * This interface checks if the given value is a valid field field string. If it is 
 * invalid then an exception is thrown. 
 *
 * @param value The field field value whose syntax is checked. 
 */
void Using::checkField(const QString& value)
{
   // Make sure the given field is valid, throwing an exception if it is not. 
   if ( !QRegularExpression("\\A((::)?[a-zA-Z_]+[a-zA-Z_0-9]*)+(\\s*=\\s*[a-zA-Z0-9<>&\\* ]*)?\\z").match(value).hasMatch() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid using field '%1'.").arg(value));
      throw e;
   }
}






/*!
 * Sets the field of this using block to the given value, making sure the syntax is 
 * valid using the check name interface. If it is invalid then an exception is 
 * thrown. 
 *
 * @param value The new value for this using block's field. 
 */
void Using::setField(const QString& value)
{
   // Make sure the new value given is valid. 
   checkField(value);

   // Set this block's field to the new value. 
   _field = value;
}
