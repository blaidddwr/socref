#include "cppqt_parent.h"
#include "cppqt_parent_view.h"
#include "cppqt_parent_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type.h"
#include "common.h"



using namespace std;
using namespace Sut;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. 
 */
const QStringList Parent::_fields {"access","class"};
/*!
 * List of access type names that follow the same order as this block's enumeration 
 * of possible access types. 
 */
const QStringList Parent::_accessNames {"public","protected","private"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Parent::type() const
{
   return BlockFactory::ParentType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& Parent::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Parent::name() const
{
   // Return only the access type string of this parent block. 
   return accessString();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Parent::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon publicIcon(":/icons/parent.svg");
   static QIcon protectedIcon(":/icons/proparent.svg");
   static QIcon privateIcon(":/icons/priparent.svg");

   // Return the appropriate icon based off this parent block's access type. 
   switch (_access)
   {
   case Access::Public: return publicIcon;
   case Access::Protected: return protectedIcon;
   case Access::Private: return privateIcon;
   default: return publicIcon;
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Parent::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Parent::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Parent::fieldSize() const
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
AbstractBlock::Field Parent::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::AccessType:
   case Field::ClassName: return AbstractBlock::Field::String;

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
QVariant Parent::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::AccessType: return accessString();
   case Field::ClassName: return _className;

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
std::unique_ptr<::Gui::AbstractEdit> Parent::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new parent block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Parent::Parent(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setClassName(QStringLiteral("object"));
}






/*!
 * Return the access type of this parent block. 
 *
 * @return Access type for this parent block. 
 */
Parent::Access Parent::access() const
{
   return _access;
}






/*!
 * Return the string version of this block's access type. 
 *
 * @return String version of this block's access type. 
 */
QString Parent::accessString() const
{
   // Use this block's access names list to return the string version of this block's 
   // access type. 
   return _accessNames.at(static_cast<int>(_access));
}






/*!
 * Returns the class name of this parent block. 
 *
 * @return Class name of this parent block. 
 */
QString Parent::className() const
{
   return _className;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Parent::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Parent);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Parent::version() const
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
QString Parent::fieldTag(int index) const
{
   // Use this block's fields list to figure out the tag at the given index. 
   return _fields.at(index);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param name See interface docs. 
 *
 * @return See interface docs. 
 */
int Parent::fieldIndexOf(const QString& name) const
{
   // Use this block's fields list to figure out the index of the given tag name. 
   return _fields.indexOf(name);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Parent::fieldModified(int index)
{
   // Notify that this block's body has changed and it is modified. 
   notifyModified();
   notifyBodyModified();

   // If the given field index is the access type then also notify this block's name 
   // has changed. 
   if ( index == Field::AccessType ) notifyNameModified();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @param value See interface docs. 
 */
void Parent::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. If the 
   // given index is unknown then do nothing. 
   switch (index)
   {
   case Field::AccessType:
      _access = static_cast<Access>(_accessNames.indexOf(value.toString()));
      break;
   case Field::ClassName:
      setClassName(value.toString());
      break;
   }
}






/*!
 * Sets the class name field for this parent block. If the given value is not valid 
 * then an exception is thrown. 
 *
 * @param value The new value for this parent block's class name field. 
 */
void Parent::setClassName(const QString& value)
{
   // Make sure the new given value is valid. 
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Class name '%1' is not valid.").arg(value));
      throw e;
   }

   // Set the new value to this parent block's class name field. 
   _className = value;
}
