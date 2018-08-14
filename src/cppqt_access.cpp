#include "cppqt_access.h"
#include <socutil/sut_exceptions.h>
#include "cppqt_access_view.h"
#include "cppqt_access_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
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
const QStringList Access::_fields {"type"};
/*!
 * List of access type names that follow the same order as this block's enumeration 
 * of possible types. 
 */
const QStringList Access::_typeNames
{
   "public:"
   ,"protected:"
   ,"private:"
   ,"signals:"
   ,"public slots:"
   ,"protected slots:"
   ,"private slots:"
};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Access::type() const
{
   return BlockFactory::AccessType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& Access::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Access::name() const
{
   // Return the display name of this access block using its access type and the 
   // number of children it contains. 
   return QString("%1 (%2)").arg(_typeNames.at(static_cast<int>(_type))).arg(size());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Access::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon publicIcon(":/icons/public.svg");
   static QIcon signalsIcon(":/icons/signals.svg");
   static QIcon publicSlotsIcon(":/icons/pubslots.svg");
   static QIcon protectedIcon(":/icons/protected.svg");
   static QIcon protectedSlotsIcon(":/icons/proslots.svg");
   static QIcon privateIcon(":/icons/private.svg");
   static QIcon privateSlotsIcon(":/icons/prislots.svg");

   // Based off this access block's type return the appropriate icon. 
   switch (_type)
   {
   case Type::Public: return publicIcon;
   case Type::Signals: return signalsIcon;
   case Type::PublicSlots: return publicSlotsIcon;
   case Type::Protected: return protectedIcon;
   case Type::ProtectedSlots: return protectedSlotsIcon;
   case Type::Private: return privateIcon;
   case Type::PrivateSlots: return privateSlotsIcon;
   default: return publicIcon;
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Access::buildList() const
{
   // Create an empty return build list. 
   QList<int> ret;

   // If this block's access type is a slot then add the slot type. 
   if ( isSlot(_type) ) ret << BlockFactory::SlotType;

   // Else if this block's access type is signals then add the signals type. 
   else if ( _type == Type::Signals ) ret << BlockFactory::SignalType;

   // Else this block's access type is normal so add all normal types. 
   else
   {
      ret << BlockFactory::FunctionType
          << BlockFactory::OperatorType
          << BlockFactory::ConstructorType
          << BlockFactory::DestructorType
          << BlockFactory::VariableType
          << BlockFactory::EnumerationType
          << BlockFactory::ClassType
          << BlockFactory::UsingType
          << BlockFactory::FriendType;
   }

   // Return the build list. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Access::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Access::fieldSize() const
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
AbstractBlock::Field Access::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::AccessType: return AbstractBlock::Field::String;

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
QVariant Access::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::AccessType: return accessTypeString();

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
std::unique_ptr<::Gui::AbstractEdit> Access::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Tests if the given access type is a slot. 
 *
 * @param value The access type value that is tested. 
 *
 * @return True if the given access type is a slot or false otherwise. 
 */
bool Access::isSlot(Type value)
{
   // Test if the given type is any of the possible slot types. 
   return ( value == Type::PublicSlots
            || value == Type::ProtectedSlots
            || value == Type::PrivateSlots );
}






/*!
 * Tests if the given access type is normal. 
 *
 * @param value The access type value that is tested. 
 *
 * @return True if the given access type is normal or false otherwise. 
 */
bool Access::isNormal(Type value)
{
   // Test if the given type is any of the possible normal types. 
   return ( value == Type::Public || value == Type::Protected || value == Type::Private );
}






/*!
 * Tests if this access block contains any signal block children. 
 *
 * @return True if this access block contains any signals or false otherwise. 
 */
bool Access::hasSignals() const
{
   return containsType(BlockFactory::SignalType);
}






/*!
 * Tests if this access block contains any slot block children. 
 *
 * @return True if this access block contains any slots or false otherwise. 
 */
bool Access::hasSlots() const
{
   return containsType(BlockFactory::SlotType);
}






/*!
 * Tests if this access block contains any signal and/or slot block children. 
 *
 * @return True if this access block contains any signals and/or slots, otherwise 
 *         false is returned. 
 */
bool Access::hasSignalsOrSlots() const
{
   return containsType({BlockFactory::SignalType,BlockFactory::SlotType});
}






/*!
 * Tests if this access block contains any regular block children. By regular this 
 * means children that are not signals or slots. 
 *
 * @return True if this access block contains any regular block children or false 
 *         otherwise. 
 */
bool Access::hasRegular() const
{
   return containsType({BlockFactory::FunctionType
                        ,BlockFactory::OperatorType
                        ,BlockFactory::ConstructorType
                        ,BlockFactory::DestructorType
                        ,BlockFactory::VariableType
                        ,BlockFactory::EnumerationType
                        ,BlockFactory::ClassType});
}






/*!
 * Tests if this access block contains any function block that is virtual. 
 *
 * @return True if this access block contains any function block that is virtual or 
 *         false otherwise. 
 */
bool Access::hasVirtual() const
{
   // Iterate through all this block's children. 
   for (auto child : list())
   {
      // If the child block is a function and that function is virtual then return true. 
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isVirtual() ) return true;
      }
   }

   // No virtual functions was found so return false. 
   return false;
}






/*!
 * Tests if this access block contains any function block that is abstract. 
 *
 * @return True if this access block contains any function block that is abstract 
 *         or false otherwise. 
 */
bool Access::hasAbstract() const
{
   // Iterate through all this block's children. 
   for (auto child : list())
   {
      // If the child block is a function and that function is abstract then return 
      // true. 
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isAbstract() ) return true;
      }
   }

   // No abstract function was found so return false. 
   return false;
}






/*!
 * Returns the access type of this block. 
 *
 * @return The access type of this block. 
 */
Access::Type Access::accessType() const
{
   return _type;
}






/*!
 * Returns the string version of this block's access type. 
 *
 * @return String version of this block's access type. 
 */
QString Access::accessTypeString() const
{
   // Use the access type name list to return the string version of this block's 
   // access type. 
   return _typeNames.at(static_cast<int>(_type));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Access::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Access);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Access::version() const
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
QString Access::fieldTag(int index) const
{
   // Use this block's fields list to return the correct tag name from the given 
   // index. 
   return _fields.at(index);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param name See interface docs. 
 *
 * @return See interface docs. 
 */
int Access::fieldIndexOf(const QString& name) const
{
   // Use this block's fields list to return the correct index with the given field 
   // tag name. 
   return _fields.indexOf(name);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Access::fieldModified(int index)
{
   // Based off the given field index notify of this block's changes. If the given 
   // index is unknown then do nothing. 
   switch (index)
   {
   case Field::AccessType:
      notifyModified();
      notifyNameModified();
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
void Access::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. If the 
   // given index is unknown then do nothing. 
   switch (index)
   {
   case Field::AccessType:
      setAccessType(static_cast<Type>(_typeNames.indexOf(value.toString())));
      break;
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childNameModified(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Return true to pass this event up to its parent class block. 
   return true;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childAdded(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return true to pass this event up to its parent class block. 
   return true;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childRemoved(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return true to pass this event up to its parent class block. 
   return true;
}






/*!
 * Sets the access type for this block, making sure the given type is legal with 
 * the child blocks this access block contains. If the new type is not legal then 
 * an exception is thrown. 
 *
 * @param value The new access type field value for this block. 
 */
void Access::setAccessType(Type value)
{
   // If the new type is normal then make sure this block has no signals or slots. 
   if ( isNormal(value) && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Normal access types cannot contain signals and/or slots."));
      throw e;
   }

   // If the new type is a slot then make sure this block has no signals or regular 
   // methods. 
   if ( isSlot(value) && ( hasRegular() || hasSignals() ) )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Slot access types cannot contain functions, variables, and/or signals."));
      throw e;
   }

   // If the new type is signals then make sure this block has no slots or regular 
   // methods. 
   if ( value == Type::Signals && ( hasRegular() || hasSlots() ) )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Signals access type cannot contain functions, variables, and/or slots."));
      throw e;
   }

   // Set this block's access type field to the new type. 
   _type = value;
}
