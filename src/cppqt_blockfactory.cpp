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



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns the project type of this block factory. 
 *
 * @return Project type of this block factory. 
 */
int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}






/*!
 * Implements the interface that returns the number of block types that exist for 
 * this factory. 
 *
 * @return Number of block types that exist for this factory. 
 */
int BlockFactory::size() const
{
   return Total;
}






/*!
 * Implements the interface that returns the display name for the given block type. 
 *
 * @param type Block type whose display name is returned. 
 *
 * @return Display name for the given block type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given block type return its display name. 
 */
QString BlockFactory::name(int type) const
{
   // 1
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
 * Implements the interface that returns the element name for the given block type. 
 *
 * @param type Block type whose element name is returned. 
 *
 * @return Element name for the given block type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given block type return its element name. 
 */
QString BlockFactory::elementName(int type) const
{
   // 1
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
 * Implements the interface that makes a new block of the given type and returns 
 * its pointer. 
 *
 * @param type Block type which is made and returned. 
 *
 * @return Pointer to new block with the given type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new block of the given type with a default value and return its 
 *    smart pointer. 
 */
std::unique_ptr<AbstractBlock> BlockFactory::makeBlock(int type) const
{
   // 1
   switch (type)
   {
   case NamespaceType: return unique_ptr<AbstractBlock>(new Namespace);
   case VariableType: return unique_ptr<AbstractBlock>(new Variable);
   case FunctionType: return unique_ptr<AbstractBlock>(new Function);
   case TemplateType: return unique_ptr<AbstractBlock>(new Template);
   case ClassType: return unique_ptr<AbstractBlock>(new Class);
   case AccessType: return unique_ptr<AbstractBlock>(new Access);
   case OperatorType: return unique_ptr<AbstractBlock>(new Operator);
   case SlotType: return unique_ptr<AbstractBlock>(new Slot);
   case SignalType: return unique_ptr<AbstractBlock>(new Signal);
   case ConstructorType: return unique_ptr<AbstractBlock>(new Constructor);
   case DestructorType: return unique_ptr<AbstractBlock>(new Destructor);
   case EnumerationType: return unique_ptr<AbstractBlock>(new Enumeration);
   case EnumValueType: return unique_ptr<AbstractBlock>(new EnumValue);
   case ParentType: return unique_ptr<AbstractBlock>(new Parent);
   case UsingType: return unique_ptr<AbstractBlock>(new Using);
   case TypeListType: return unique_ptr<AbstractBlock>(new TypeList);
   case TypeType: return unique_ptr<AbstractBlock>(new Type);
   case FriendType: return unique_ptr<AbstractBlock>(new Friend);
   default: return nullptr;
   }
}






/*!
 * Implements the interface that makes a new root block and returns its pointer. 
 *
 * @return Pointer to new root block for this project type. 
 */
std::unique_ptr<AbstractBlock> BlockFactory::makeRootBlock() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}
