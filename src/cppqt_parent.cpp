#include "cppqt_parent.h"
#include <exception.h>
#include "cppqt_parent_view.h"
#include "cppqt_parent_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Parent::_fields {"access","class"};
/*!
 */
const QStringList Parent::_accessNames {"public","protected","private"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Parent::type() const
{
   return BlockFactory::ParentType;
}






/*!
 * Implements the interface that returns a reference to this block's factory which 
 * produces all block types for this project type. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Parent::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Parent::name() const
{
   return accessString();
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Parent::icon() const
{
   static bool isLoaded {false};
   static QIcon public_;
   static QIcon protected_;
   static QIcon private_;
   if ( !isLoaded )
   {
      public_ = QIcon(":/icons/parent.svg");
      protected_ = QIcon(":/icons/proparent.svg");
      private_ = QIcon(":/icons/priparent.svg");
      isLoaded = true;
   }
   switch (_access)
   {
   case Access::Public: return public_;
   case Access::Protected: return protected_;
   case Access::Private: return private_;
   default: return public_;
   }
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Parent::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Parent::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Parent::fieldSize() const
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
AbstractBlock::Field Parent::fieldType(int index) const
{
   switch (index)
   {
   case Field::AccessType:
   case Field::ClassName: return AbstractBlock::Field::String;
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
QVariant Parent::field(int index) const
{
   switch (index)
   {
   case Field::AccessType: return accessString();
   case Field::ClassName: return _className;
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
std::unique_ptr<::Gui::AbstractEdit> Parent::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 *
 * @param className  
 */
Parent::Parent(const QString& className):
   _className(className)
{
   checkClassName(_className);
}






/*!
 */
Parent::Access Parent::access() const
{
   return _access;
}






/*!
 */
QString Parent::accessString() const
{
   return _accessNames.at(static_cast<int>(_access));
}






/*!
 */
QString Parent::className() const
{
   return _className;
}






/*!
 *
 * @param value  
 */
void Parent::setAccess(Access value)
{
   if ( _access != value )
   {
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
      _access = value;
   }
}






/*!
 *
 * @param value  
 */
void Parent::setAccess(const QString& value)
{
   setField(Field::AccessType,value);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Parent::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Parent);
}






/*!
 * Implements the interface that returns the current data version for this block 
 * type. 
 *
 * @return Current data version. 
 */
int Parent::version() const
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
QString Parent::fieldTag(int index) const
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
int Parent::fieldIndexOf(const QString& name) const
{
   return _fields.indexOf(name);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 */
void Parent::fieldModified(int index)
{
   switch (index)
   {
   case Field::AccessType:
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
      break;
   case Field::ClassName:
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
void Parent::quietlySetField(int index, const QVariant& value)
{
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
 *
 * @param value  
 */
void Parent::checkClassName(const QString& value)
{
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Class name '%1' is not valid.").arg(value));
      throw e;
   }
}






/*!
 *
 * @param value  
 */
void Parent::setClassName(const QString& value)
{
   checkClassName(value);
   _className = value;
}
