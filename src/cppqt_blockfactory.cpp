#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_variable.h"
#include "cppqt_function.h"
#include "cppqt_template.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_operator.h"
#include "cppqt_slot.h"
#include "cppqt_signal.h"
#include "cppqt_constructor.h"
#include "cppqt_destructor.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_parent.h"
#include "cppqt_using.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"
#include "cppqt_friend.h"
#include "projectfactory.h"



using namespace Sut;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int BlockFactory::size() const
{
   // Use the total enumeration to return the total size of defined blocks for this 
   // block factory. 
   return Total;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BlockFactory::name(int type) const
{
   // Based off the given block type return its display name. 
   switch (type)
   {
   case NamespaceType: return QObject::tr("Namespace");
   case VariableType: return QObject::tr("Variable");
   case FunctionType: return QObject::tr("Function");
   case TemplateType: return QObject::tr("Template");
   case ClassType: return QObject::tr("Class");
   case AccessType: return QObject::tr("Access");
   case OperatorType: return QObject::tr("Operator");
   case SlotType: return QObject::tr("Slot");
   case SignalType: return QObject::tr("Signal");
   case ConstructorType: return QObject::tr("Constructor");
   case DestructorType: return QObject::tr("Destructor");
   case EnumerationType: return QObject::tr("Enumeration");
   case EnumValueType: return QObject::tr("Enumeration Value");
   case ParentType: return QObject::tr("Parent");
   case UsingType: return QObject::tr("Using");
   case TypeListType: return QObject::tr("Type List");
   case TypeType: return QObject::tr("Type");
   case FriendType: return QObject::tr("Friend");
   default: return QString();
   }
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BlockFactory::elementName(int type) const
{
   // Based off the given block type return its element name. 
   switch (type)
   {
   case NamespaceType: return "namespace";
   case VariableType: return "variable";
   case FunctionType: return "function";
   case TemplateType: return "template";
   case ClassType: return "class";
   case AccessType: return "access";
   case OperatorType: return "operator";
   case SlotType: return "slot";
   case SignalType: return "signal";
   case ConstructorType: return "constructor";
   case DestructorType: return "destructor";
   case EnumerationType: return "enumeration";
   case EnumValueType: return "enumval";
   case ParentType: return "parent";
   case UsingType: return "using";
   case TypeListType: return "typeList";
   case TypeType: return "type";
   case FriendType: return "friend";
   default: return "unknown";
   }
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @param isDefault See interface docs. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BlockFactory::makeBlock(int type, bool isDefault) const
{
   // Based off the given block type create a new block of that type and return its 
   // smart pointer. 
   switch (type)
   {
   case NamespaceType: return QPtr<AbstractBlock>(new Namespace(isDefault));
   case VariableType: return QPtr<AbstractBlock>(new Variable(isDefault));
   case FunctionType: return QPtr<AbstractBlock>(new Function(isDefault));
   case TemplateType: return QPtr<AbstractBlock>(new Template(isDefault));
   case ClassType: return QPtr<AbstractBlock>(new Class(isDefault));
   case AccessType: return QPtr<AbstractBlock>(new Access);
   case OperatorType: return QPtr<AbstractBlock>(new Operator(isDefault));
   case SlotType: return QPtr<AbstractBlock>(new Slot(isDefault));
   case SignalType: return QPtr<AbstractBlock>(new Signal(isDefault));
   case ConstructorType: return QPtr<AbstractBlock>(new Constructor);
   case DestructorType: return QPtr<AbstractBlock>(new Destructor);
   case EnumerationType: return QPtr<AbstractBlock>(new Enumeration(isDefault));
   case EnumValueType: return QPtr<AbstractBlock>(new EnumValue(isDefault));
   case ParentType: return QPtr<AbstractBlock>(new Parent(isDefault));
   case UsingType: return QPtr<AbstractBlock>(new Using(isDefault));
   case TypeListType: return QPtr<AbstractBlock>(new TypeList(isDefault));
   case TypeType: return QPtr<AbstractBlock>(new Type(isDefault));
   case FriendType: return QPtr<AbstractBlock>(new Friend(isDefault));
   default: return nullptr;
   }
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BlockFactory::makeRootBlock() const
{
   // Create a new namespace block which is used as this project type's root block 
   // and return its pointer. 
   return QPtr<AbstractBlock>(new Namespace);
}
