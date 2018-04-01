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
#include "cppqt_declaration.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"
#include "projectfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






int BlockFactory::size() const
{
   return Total;
}






QString BlockFactory::name(int type) const
{
   switch (type)
   {
   case NamespaceType: return "Namespace";
   case VariableType: return "Variable";
   case FunctionType: return "Function";
   case TemplateType: return "Template";
   case ClassType: return "Class";
   case AccessType: return "Access";
   case OperatorType: return "Operator";
   case SlotType: return "Slot";
   case SignalType: return "Signal";
   case ConstructorType: return "Constructor";
   case DestructorType: return "Destructor";
   case EnumerationType: return "Enumeration";
   case EnumValueType: return "Enum Value";
   case ParentType: return "Parent";
   case DeclarationType: return "Declaration";
   case TypeListType: return "Type List";
   case TypeType: return "Type";
   default: return QString();
   }
}






QString BlockFactory::elementName(int type) const
{
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
   case DeclarationType: return "declaration";
   case TypeListType: return "typeList";
   case TypeType: return "type";
   default: return "unknown";
   }
}






unique_ptr<AbstractBlock> BlockFactory::makeRootBlock() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}






unique_ptr<AbstractBlock> BlockFactory::makeBlock(int type) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<AbstractBlock>(new Namespace("unnamed_namespace"));
   case VariableType: return unique_ptr<AbstractBlock>(new Variable("int","unnamed_variable"));
   case FunctionType: return unique_ptr<AbstractBlock>(new Function("void","unnamed_function"));
   case TemplateType: return unique_ptr<AbstractBlock>(new Template("class","unnamed_template"));
   case ClassType: return unique_ptr<AbstractBlock>(new Class("unnamed_class"));
   case AccessType: return unique_ptr<AbstractBlock>(new Access(Access::Type::Public));
   case OperatorType: return unique_ptr<AbstractBlock>(new Operator("void"));
   case SlotType: return unique_ptr<AbstractBlock>(new Slot("unnamed_slot"));
   case SignalType: return unique_ptr<AbstractBlock>(new Signal("unnamed_signal"));
   case ConstructorType: return unique_ptr<AbstractBlock>(new Constructor);
   case DestructorType: return unique_ptr<AbstractBlock>(new Destructor);
   case EnumerationType: return unique_ptr<AbstractBlock>(new Enumeration("unnamed_enumeration"));
   case EnumValueType: return unique_ptr<AbstractBlock>(new EnumValue("unnamed_value"));
   case ParentType: return unique_ptr<AbstractBlock>(new Parent("unnamed_parent"));
   case DeclarationType: return unique_ptr<AbstractBlock>(new Declaration(Declaration::Type::Using,"this"));
   case TypeListType: return unique_ptr<AbstractBlock>(new TypeList);
   case TypeType: return unique_ptr<AbstractBlock>(new Type("unnamed_type"));
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
