#include "cppqt_factory.h"
#include "projectfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"
#include "cppqt_function.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_parent.h"
#include "cppqt_declaration.h"



using namespace CppQt;
//






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int Factory::type() const
{
   return ProjectFactory::CppQtType;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString Factory::elementName(int type) const
{
   // Based off the given block type return its element name, throwing an exception 
   // if the given type is unknown. 
   switch (type)
   {
   case NamespaceType: return QStringLiteral("namespace");
   case EnumerationType: return QStringLiteral("enumeration");
   case EnumValueType: return QStringLiteral("enumvalue");
   case VariableType: return QStringLiteral("variable");
   case FunctionType: return QStringLiteral("function");
   case TemplateType: return QStringLiteral("template");
   case ClassType: return QStringLiteral("class");
   case AccessType: return QStringLiteral("access");
   case ParentType: return QStringLiteral("parent");
   case DeclarationType: return QStringLiteral("declaration");
   default:
      Q_ASSERT(false);
   }
}






/*!
 * Constructs a new C++/Qt block factory. 
 */
Factory::Factory():
   BasicBlockFactory(QStringLiteral(":/cppqt/defs.xml"))
{}






/*!
 * Implements _BasicBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<BasicBlock> Factory::makeBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null 
   // pointer if that type does not have an implementation. 
   switch (type)
   {
   case NamespaceType: return Soc::Ut::QPtr<BasicBlock>(new Namespace);
   case EnumerationType: return Soc::Ut::QPtr<BasicBlock>(new Enumeration);
   case EnumValueType: return Soc::Ut::QPtr<BasicBlock>(new EnumValue);
   case VariableType: return Soc::Ut::QPtr<BasicBlock>(new Variable);
   case TemplateType: return Soc::Ut::QPtr<BasicBlock>(new Template);
   case FunctionType: return Soc::Ut::QPtr<BasicBlock>(new Function);
   case ClassType: return Soc::Ut::QPtr<BasicBlock>(new Class);
   case AccessType: return Soc::Ut::QPtr<BasicBlock>(new Access);
   case ParentType: return Soc::Ut::QPtr<BasicBlock>(new Parent);
   case DeclarationType: return Soc::Ut::QPtr<BasicBlock>(new Declaration);
   default: return nullptr;
   }
}
