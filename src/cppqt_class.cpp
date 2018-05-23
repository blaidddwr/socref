#include "cppqt_class.h"
#include <exception.h>
#include "cppqt_view_class.h"
#include "cppqt_edit_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_template.h"
#include "cppqt_constructor.h"
#include "cppqt_parent.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Class::_fields {"qtobject"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Class::type() const
{
   return BlockFactory::ClassType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Class::name() const
{
   QString ret;
   if ( hasTemplates() ) ret.append("<> ");
   ret.append(Base::name());
   if ( containsType(BlockFactory::ParentType) ) ret.append(" :");
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Class::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/class.svg");
      virtual_ = QIcon(":/icons/vclass.svg");
      abstract = QIcon(":/icons/aclass.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Class::buildList() const
{
   QList<int> ret {BlockFactory::AccessType,BlockFactory::ParentType,BlockFactory::TypeListType};
   if ( !isVirtual() && !_qtObject ) ret << BlockFactory::TemplateType;
   return ret;
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Class::makeView() const
{
   return unique_ptr<QWidget>(new View::Class(this));
}






/*!
 */
int Class::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Class::fieldType(int index) const
{
   switch (index)
   {
   case Field::QtObject: return AbstractBlock::Field::Boolean;
   default: return Base::fieldType(index);
   }
}






/*!
 *
 * @param index  
 */
QVariant Class::field(int index) const
{
   switch (index)
   {
   case Field::QtObject: return _qtObject;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Class::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Class(this));
}






/*!
 */
QList<AbstractBlock*> Class::realChildren() const
{
   QList<AbstractBlock*> ret;
   for (auto child : makeListOfType<Access>(BlockFactory::AccessType))
   {
      ret << child << child->list();
   }
   return ret;
}






/*!
 *
 * @param name  
 */
Class::Class(const QString& name):
   Namespace(name)
{}






/*!
 */
bool Class::isQtObject() const
{
   return _qtObject;
}






/*!
 */
bool Class::isVirtual() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasVirtual() ) return true;
   }
   return false;
}






/*!
 */
bool Class::isAbstract() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasAbstract() ) return true;
   }
   return false;
}






/*!
 */
bool Class::hasSignalsOrSlots() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasSignalsOrSlots() ) return true;
   }
   return false;
}






/*!
 */
bool Class::hasTemplates() const
{
   return containsType(BlockFactory::TemplateType);
}






/*!
 */
bool Class::hasAnyTemplates() const
{
   if ( hasTemplates() ) return true;
   AbstractBlock* back {parent()};
   while ( back && back->type() != BlockFactory::NamespaceType )
   {
      if ( Class* valid = back->cast<Class>(BlockFactory::ClassType) )
      {
         if ( valid->hasTemplates() ) return true;
      }
      back = back->parent();
   }
   return false;
}






/*!
 */
QList<Template*> Class::templates() const
{
   QList<Template*> ret;
   for (auto child : list())
   {
      if ( Template* valid = child->cast<Template>(BlockFactory::TemplateType) ) ret.append(valid);
   }
   return ret;
}






/*!
 */
QList<Parent*> Class::parents() const
{
   return makeListOfType<Parent>(BlockFactory::ParentType);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Class::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Class);
}






/*!
 *
 * @param index  
 */
void Class::fieldModified(int index)
{
   switch (index)
   {
   case Field::QtObject:
      notifyModified();
      notifyBodyModified();
      break;
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 *
 * @param index  
 *
 * @param value  
 */
void Class::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::QtObject:
      setQtObject(value.toBool());
      break;
   default:
      Base::quietlySetField(index,value);
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
bool Class::childNameModified(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   return false;
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
bool Class::childAdded(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      connect(this,&Class::nameChanged,constructor,&Constructor::classNameChanged);
   }
   return false;
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
bool Class::childRemoved(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      disconnect(constructor);
   }
   return false;
}






/*!
 */
QStringList Class::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }
   return ret;
}






/*!
 */
QList<Access*> Class::accessChildren() const
{
   QList<Access*> ret;
   for (auto child : list())
   {
      if ( Access* valid = child->cast<Access>(BlockFactory::AccessType) ) ret << valid;
   }
   return ret;
}






/*!
 *
 * @param state  
 */
void Class::setQtObject(bool state)
{
   if ( !state && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Class must be a Qt Object because it has slots and/or signals."));
      throw e;
   }
   _qtObject = state;
}
