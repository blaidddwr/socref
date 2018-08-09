#include "cppqt_class.h"
#include <exception.h>
#include "cppqt_class_view.h"
#include "cppqt_class_edit.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_template.h"
#include "cppqt_constructor.h"
#include "cppqt_parent.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. This is in addition to the base fields this block 
 * inherits. 
 */
const QStringList Class::_fields {"qtobject"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Class::type() const
{
   return BlockFactory::ClassType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Class::name() const
{
   // Create an empty return string. 
   QString ret;

   // If this class has templates then append an indicator. 
   if ( hasTemplates() ) ret.append("<> ");

   // Append the base name of this class. 
   ret.append(Base::name());

   // If this class inherits from any other classes append an indicator. 
   if ( containsType(BlockFactory::ParentType) ) ret.append(" :");

   // Return the display name string. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Class::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon regularIcon(":/icons/class.svg");
   static QIcon virtualIcon(":/icons/vclass.svg");
   static QIcon abstractIcon(":/icons/aclass.svg");

   // Return the appropriate icon based off this class block's current properties. 
   if ( isAbstract() ) return abstractIcon;
   else if ( isVirtual() ) return virtualIcon;
   else return regularIcon;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Class::buildList() const
{
   // Create the build list for this block type. 
   QList<int> ret {BlockFactory::AccessType,BlockFactory::ParentType,BlockFactory::TypeListType};

   // If this class is not virtual and is not a qt object then append templates to 
   // the build list. 
   if ( !isVirtual() && !_qtObject ) ret << BlockFactory::TemplateType;

   // Return the build list. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Class::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Class::fieldSize() const
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
AbstractBlock::Field Class::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::QtObject: return AbstractBlock::Field::Boolean;

   // If the given index is unknown to this block then call its base interface. 
   default: return Base::fieldType(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QVariant Class::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::QtObject: return _qtObject;

   // If the given index is unknown to this block then call its base interface. 
   default: return Base::field(index);
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Class::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Implements _CppQt::Namespace_ interface. 
 *
 * @return See interface docs. 
 */
QList<AbstractBlock*> Class::realChildren() const
{
   // Create an empty block pointer return list. 
   QList<AbstractBlock*> ret;

   // Iterate through all access block children of this class block. 
   for (auto child : makeListOfType<Access>(BlockFactory::AccessType))
   {
      // Append the access block's pointer and then all of its children block pointers. 
      ret << child << child->list();
   }

   // Return the pointer list of real children. 
   return ret;
}






/*!
 * Constructs a new class block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Class::Class(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setName(QStringLiteral("object"));
}






/*!
 * Tests of this class block is a qt object. 
 *
 * @return True if this class is a qt object or false otherwise. 
 */
bool Class::isQtObject() const
{
   return _qtObject;
}






/*!
 * Tests if any methods of this class are virtual. 
 *
 * @return True if any methods of this class are virtual or false otherwise. 
 */
bool Class::isVirtual() const
{
   // Iterate through all access block children of this class block, returning true 
   // if any of them has any virtual functions. 
   for (auto access : makeListOfType<Access>(BlockFactory::AccessType))
   {
      if ( access->hasVirtual() ) return true;
   }

   // If this is reached then this class has no virtual methods so return false. 
   return false;
}






/*!
 * Tests if this class has any methods that are abstract. 
 *
 * @return True if this class has any abstract methods or false otherwise. 
 */
bool Class::isAbstract() const
{
   // Iterate through all access block children of this class block, returning true 
   // if any of them has any abstract functions. 
   for (auto access : makeListOfType<Access>(BlockFactory::AccessType))
   {
      if ( access->hasAbstract() ) return true;
   }

   // If this is reached then this class has no abstract methods so return false. 
   return false;
}






/*!
 * Tests if this class has any signals or slots. 
 *
 * @return True if this class has any signals and/or slots, otherwise false is 
 *         returned. 
 */
bool Class::hasSignalsOrSlots() const
{
   // Iterate through all access block children of this class block, returning true 
   // if any of them has any signals or slots. 
   for (auto access : makeListOfType<Access>(BlockFactory::AccessType))
   {
      if ( access->hasSignalsOrSlots() ) return true;
   }

   // If this is reached then this class has no signals or slots so return false. 
   return false;
}






/*!
 * Tests if this class has any template arguments. 
 *
 * @return True if this class has any template arguments or false otherwise. 
 */
bool Class::hasTemplates() const
{
   return containsType(BlockFactory::TemplateType);
}






/*!
 * Tests if this class or any parent classes contain any template arguments. 
 *
 * @return True if this class or any parent class contains any template arguments, 
 *         otherwise false is returned. 
 */
bool Class::hasAnyTemplates() const
{
   // Make sure this class block does not have any template arguments. 
   if ( hasTemplates() ) return true;

   // Iterate up the chain of block parents starting with this block's parent until a 
   // namespace or null pointer is reached. 
   AbstractBlock* back {parent()};
   while ( back && back->type() != BlockFactory::NamespaceType )
   {
      // If this parent is a class and it has template arguments then return true. 
      if ( Class* valid = back->cast<Class>(BlockFactory::ClassType) )
      {
         if ( valid->hasTemplates() ) return true;
      }

      // Move up to the next parent block. 
      back = back->parent();
   }

   // If this is reached then no template arguments were found so return false. 
   return false;
}






/*!
 * Returns a pointer list of all children template argument blocks for this class 
 * block. 
 *
 * @return Pointer list of all children template blocks of this class block. 
 */
QList<Template*> Class::templates() const
{
   return makeListOfType<Template>(BlockFactory::TemplateType);
}






/*!
 * Returns a pointer list of all children parent inheritance blocks of this class 
 * block. 
 *
 * @return Pointer list of all parent block children of this class block. 
 */
QList<Parent*> Class::parents() const
{
   return makeListOfType<Parent>(BlockFactory::ParentType);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Class::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Class);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Class::fieldModified(int index)
{
   // Based off the given field index notify of changes to this block. 
   switch (index)
   {
   case Field::QtObject:
      notifyModified();
      notifyBodyModified();
      break;

   // If the given index is unknown to this block then call its base interface. 
   default:
      Base::fieldModified(index);
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
void Class::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value to the new one given. 
   switch (index)
   {
   case Field::QtObject:
      setQtObject(value.toBool());
      break;

   // If the given index is unknown to this block then call its base interface. 
   default:
      Base::quietlySetField(index,value);
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
bool Class::childNameModified(AbstractBlock* child)
{
   // If the child modified is a template, function, or parent then notify the name 
   // of this block has changed. 
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }

   // Return false to stop any additional calls. 
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Class::childAdded(AbstractBlock* child)
{
   // If the child added is a template, function, or parent then notify the name of 
   // this block has changed. 
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }

   // If the child added is a constructor then connect this object's name changed 
   // signal to the constructor. 
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      connect(this,&Class::nameChanged,constructor,&Constructor::classNameChanged);
   }

   // Return false to stop any additional calls. 
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Class::childRemoved(AbstractBlock* child)
{
   // If the child removed is a template, function, or parent then notify the name of 
   // this block has changed. 
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }

   // If the child removed is a constructor then disconnect all signals between it 
   // and this object. 
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      disconnect(constructor);
   }

   // Return false to stop any additional calls. 
   return false;
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @return See interface docs. 
 */
QStringList Class::fields() const
{
   // Initialize an empty static string list. 
   static QStringList ret;

   // If the string list is empty then populate it. 
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }

   // Return the combined fields string list. 
   return ret;
}






/*!
 * Sets the state of this class block's qt object property to the given state. If 
 * the new state is illegal then an exception is thrown. 
 *
 * @param state The new state this class block's property is set to. 
 */
void Class::setQtObject(bool state)
{
   // Make sure the new state is valid based off this class block having signals or 
   // slots. 
   if ( !state && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Class must be a Qt Object because it has slots and/or signals."));
      throw e;
   }

   // Set this block's state to the new one given. 
   _qtObject = state;
}
