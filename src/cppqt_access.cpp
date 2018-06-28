#include "cppqt_access.h"
#include <exception.h>
#include "cppqt_access_view.h"
#include "cppqt_access_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Access::_fields {"type"};
/*!
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
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Access::type() const
{
   return BlockFactory::AccessType;
}






/*!
 * Implements the interface that returns a reference to this block's factory which 
 * produces all block types for this project type. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Access::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Access::name() const
{
   QString ret {_typeNames.at(static_cast<int>(_type))};
   ret.append(" (").append(QString::number(size())).append(")");
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Access::icon() const
{
   static bool isLoaded {false};
   static QIcon public_;
   static QIcon signals_;
   static QIcon publicSlots;
   static QIcon protected_;
   static QIcon protectedSlots;
   static QIcon private_;
   static QIcon privateSlots;
   if ( !isLoaded )
   {
      public_ = QIcon(":/icons/public.svg");
      signals_ = QIcon(":/icons/signals.svg");
      publicSlots = QIcon(":/icons/pubslots.svg");
      protected_ = QIcon(":/icons/protected.svg");
      protectedSlots = QIcon(":/icons/proslots.svg");
      private_ = QIcon(":/icons/private.svg");
      privateSlots = QIcon(":/icons/prislots.svg");
      isLoaded = true;
   }
   switch (_type)
   {
   case Type::Public: return public_;
   case Type::Signals: return signals_;
   case Type::PublicSlots: return publicSlots;
   case Type::Protected: return protected_;
   case Type::ProtectedSlots: return protectedSlots;
   case Type::Private: return private_;
   case Type::PrivateSlots: return privateSlots;
   default: return public_;
   }
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Access::buildList() const
{
   QList<int> ret;
   if ( isSlot(_type) ) ret << BlockFactory::SlotType;
   else if ( _type == Type::Signals ) ret << BlockFactory::SignalType;
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
   return ret;
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Access::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Access::fieldSize() const
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
AbstractBlock::Field Access::fieldType(int index) const
{
   switch (index)
   {
   case Field::AccessType: return AbstractBlock::Field::String;
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
QVariant Access::field(int index) const
{
   switch (index)
   {
   case Field::AccessType: return accessTypeString();
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
std::unique_ptr<::Gui::AbstractEdit> Access::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 *
 * @param value  
 */
bool Access::isSlot(Type value)
{
   return ( value == Type::PublicSlots
            || value == Type::ProtectedSlots
            || value == Type::PrivateSlots );
}






/*!
 *
 * @param value  
 */
bool Access::isNormal(Type value)
{
   return ( value == Type::Public || value == Type::Protected || value == Type::Private );
}






/*!
 *
 * @param type  
 */
Access::Access(Type type):
   _type(type)
{}






/*!
 */
bool Access::hasSignals() const
{
   return containsType(BlockFactory::SignalType);
}






/*!
 */
bool Access::hasSlots() const
{
   return containsType(BlockFactory::SlotType);
}






/*!
 */
bool Access::hasSignalsOrSlots() const
{
   return containsType(QList<int>() << BlockFactory::SignalType << BlockFactory::SlotType);
}






/*!
 */
bool Access::hasRegular() const
{
   QList<int> types
   {
      BlockFactory::FunctionType
      ,BlockFactory::OperatorType
      ,BlockFactory::ConstructorType
      ,BlockFactory::DestructorType
      ,BlockFactory::VariableType
      ,BlockFactory::EnumerationType
      ,BlockFactory::ClassType
   };
   return containsType(types);
}






/*!
 */
bool Access::hasVirtual() const
{
   for (auto child : children())
   {
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isVirtual() ) return true;
      }
   }
   return false;
}






/*!
 */
bool Access::hasAbstract() const
{
   for (auto child : children())
   {
      if ( Function* func = qobject_cast<Function*>(child) )
      {
         if ( func->isAbstract() ) return true;
      }
   }
   return false;
}






/*!
 */
Access::Type Access::accessType() const
{
   return _type;
}






/*!
 */
QString Access::accessTypeString() const
{
   return _typeNames.at(static_cast<int>(_type));
}






/*!
 *
 * @param value  
 */
void Access::setAccessType(Type value)
{
   if ( value != _type )
   {
      notifyModified();
      notifyNameModified();
      quietlySetAccessType(value);
   }
}






/*!
 *
 * @param value  
 */
void Access::setAccessType(const QString& value)
{
   setField(Field::AccessType,value);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Access::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Access);
}






/*!
 * Implements the interface that returns the current data version for this block 
 * type. 
 *
 * @return Current data version. 
 */
int Access::version() const
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
QString Access::fieldTag(int index) const
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
int Access::fieldIndexOf(const QString& name) const
{
   return _fields.indexOf(name);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Access::fieldModified(int index)
{
   switch (index)
   {
   case Field::AccessType:
      notifyModified();
      notifyNameModified();
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
void Access::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::AccessType:
      quietlySetAccessType(static_cast<Type>(_typeNames.indexOf(value.toString())));
      break;
   }
}






/*!
 * Implements the interface that is called whenever a child below this block has 
 * modified its name and keeps calling this interface on the next block parent 
 * until this returns false. 
 *
 * @param child Pointer to the child block that has modified its name. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool Access::childNameModified(AbstractBlock* child)
{
   Q_UNUSED(child)
   return true;
}






/*!
 * Implements the interface that is called whenever a new child below this block 
 * has been added and keeps calling this interface on the next block parent until 
 * this returns false. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool Access::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return true;
}






/*!
 * Implements the interface that is called whenever an existing child below this 
 * block has been removed and keeps calling this interface on the next block parent 
 * until this returns false. 
 *
 * @param child Pointer to the child block that has been removed from its former 
 *              parent block. This object can be deleted right after this call. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool Access::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return true;
}






/*!
 *
 * @param value  
 */
void Access::quietlySetAccessType(Type value)
{
   if ( isNormal(value) && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Normal access types cannot contain signals and/or slots."));
      throw e;
   }
   if ( isSlot(value) && ( hasRegular() || hasSignals() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Slot access types cannot contain functions, variables, and/or signals."));
      throw e;
   }
   if ( value == Type::Signals && ( hasRegular() || hasSlots() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Signals access type cannot contain functions, variables, and/or slots."));
      throw e;
   }
   _type = value;
}
