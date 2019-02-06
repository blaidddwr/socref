#include "cppqt_class.h"
#include "cppqt_class_view.h"
#include "cppqt_factory.h"
#include "cppqt_template.h"
#include "cppqt_function.h"
#include "cppqt_access.h"
#include "cppqt_parent.h"



using namespace CppQt;
//






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
   if ( hasTemplates() ) ret += QStringLiteral("<> ");

   // Append the base name of this class. 
   ret += baseName();

   // If this class inherits from any other classes append an indicator. 
   if ( containsType(Factory::ParentType) ) ret += QStringLiteral(" :");

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
 * Implements _CppQt::Namespace_ interface. 
 *
 * @return See interface docs. 
 */
QList<AbstractBlock*> Class::realChildren() const
{
   // Create an empty block pointer return list. 
   QList<AbstractBlock*> ret;

   // Iterate through all access block children of this class block. 
   for (auto child : makeListOfType<Access>(Factory::AccessType))
   {
      // Append the access block's pointer and then all of its children block pointers. 
      ret << child << child->list();
   }

   // Return the pointer list of real children. 
   return ret;
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
   for (auto access : makeListOfType<Access>(Factory::AccessType))
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
   for (auto access : makeListOfType<Access>(Factory::AccessType))
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
 * Returns a pointer list of all children template argument blocks for this class 
 * block. 
 *
 * @return Pointer list of all children template blocks of this class block. 
 */
QList<Template*> Class::templates() const
{
   return makeListOfType<Template>(Factory::TemplateType);
}






/*!
 * Returns a pointer list of all children parent inheritance blocks of this class 
 * block. 
 *
 * @return Pointer list of all parent block children of this class block. 
 */
QList<Parent*> Class::parents() const
{
   return makeListOfType<Parent>(Factory::ParentType);
}






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<BasicBlock::View> Class::makeBasicView() const
{
   return new View(this);
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
   // If the child modified is a template or parent then notify the name and body of 
   // this block has changed. 
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
      notifyBodyModified();
   }

   // Else if the child modified is a function then notify the name of this block has 
   // changed.  
   else if ( qobject_cast<Function*>(child) )
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
   // If the child added is a template or parent then notify the name and body of 
   // this block has changed. 
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
      notifyBodyModified();
   }

   // If the child added is a function then connect this object's name changed signal 
   // to the function and notify the name of this block has changed. 
   if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
   {
      notifyNameModified();
      connect(this,&Class::nameChanged,valid,&Function::classNameChanged);
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
   // If the child added is a template or parent then notify the name and body of 
   // this block has changed. 
   if ( qobject_cast<Template*>(child) || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
      notifyBodyModified();
   }

   // If the child added is a function then connect this object's name changed signal 
   // to the function and notify the name of this block has changed. 
   if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
   {
      notifyNameModified();
      disconnect(valid);
   }

   // Return false to stop any additional calls. 
   return false;
}
