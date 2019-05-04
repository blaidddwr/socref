#include "cppqt_class.h"
#include "cppqt_classview.h"
#include "cppqt_factory.h"
#include "cppqt_template.h"
#include "cppqt_function.h"
#include "cppqt_access.h"
#include "cppqt_parent.h"



namespace CppQt
{






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QString Class::name() const
{
   // Create an empty return string.
   QString ret;

   // If this class has templates then append an indicator.
   if ( hasTemplates() ) ret += QStringLiteral("<> ");

   // Append the base name of this class.
   ret += baseName();

   // If this class inherits from any other classes append an indicator.
   if ( containsType(Factory::ParentType) ) ret += QStringLiteral(" :");

   // Return the display name string.
   return ret;
}






/*!
 * Implements _Abstract::Block_ interface.
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
 * Tests of this class block is a qt object.
 *
 * @return True if this class is a qt object or false otherwise.
 */
bool Class::isQtObject() const
{
   return getBool("qtobject");
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
   for (auto access : createListOfType<Access>(Factory::AccessType))
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
   for (auto access : createListOfType<Access>(Factory::AccessType))
   {
      if ( access->hasAbstract() ) return true;
   }

   // If this is reached then this class has no abstract methods so return false.
   return false;
}






/*!
 * Tests if this class has any template arguments.
 *
 * @return True if this class has any template arguments or false otherwise.
 */
bool Class::hasTemplates() const
{
   return containsType(Factory::TemplateType);
}






/*!
 * Tests if this class or any parent classes contain any template arguments.
 *
 * @return True if this class or any parent class contains any template
 *         arguments, otherwise false is returned.
 */
bool Class::hasAnyTemplates() const
{
   // Make sure this class block does not have any template arguments.
   if ( hasTemplates() ) return true;

   // Iterate up the chain of block parents starting with this block's parent until a
   // namespace or null pointer is reached.
   Abstract::Block* back {parent()};
   while ( back && back->type() != Factory::NamespaceType )
   {
      // If this parent is a class and it has template arguments then return true.
      if ( Class* valid = back->cast<Class>(Factory::ClassType) )
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
 * Returns a pointer list of all children template argument blocks for this
 * class block.
 *
 * @return Pointer list of all children template blocks of this class block.
 */
QList<Template*> Class::templates() const
{
   return createListOfType<Template>(Factory::TemplateType);
}






/*!
 * Returns a pointer list of all children parent inheritance blocks of this
 * class block.
 *
 * @return Pointer list of all parent block children of this class block.
 */
QList<Parent*> Class::parents() const
{
   return createListOfType<Parent>(Factory::ParentType);
}






/*!
 * Implements _Basic::Block_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Basic::BlockView> Class::makeBasicView() const
{
   return new ClassView(this);
}






/*!
 * Implements _AbstractBlock_ interface.
 *
 * @param child See interface docs.
 *
 * @return See interface docs.
 */
bool Class::childIsUpdated(Abstract::Block* child)
{
   // If the child modified is a template, parent, or function then notify this block
   // requires updating.
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) || qobject_cast<Function*>(child) ) update();

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
bool Class::childAdded(Abstract::Block* child)
{
   // If the child added is a template or parent then notify this block requires
   // updating.
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) ) update();

   // Else if the child added is a function then connect this object's name changed
   // signal to the function and notify this block requires updating.
   else if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
   {
      connect(this,&Class::nameChanged,valid,&Function::classNameChanged);
      update();
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
bool Class::childRemoved(Abstract::Block* child)
{
   // If the child added is a template or parent then notify this block requires
   // updating.
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) ) update();

   // Else if the child added is a function then connect this object's name changed
   // signal to the function and notify this block requires updating.
   else if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
   {
      disconnect(valid);
      update();
   }

   // Return false to stop any additional calls.
   return false;
}

}
