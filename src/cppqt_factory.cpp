#include "cppqt_factory.h"
#include <socutil/sut_exceptions.h>
#include <projectfactory.h>
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



using namespace Sut;
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
      {
         Exception::InvalidArgument e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Unknown C++/Qt Block type %1.").arg(type));
         throw e;
      }
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
Sut::QPtr<BasicBlock> Factory::makeBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null 
   // pointer if that type does not have an implementation. 
   switch (type)
   {
   case NamespaceType: return QPtr<BasicBlock>(new Namespace);
   case EnumerationType: return QPtr<BasicBlock>(new Enumeration);
   case EnumValueType: return QPtr<BasicBlock>(new EnumValue);
   case VariableType: return QPtr<BasicBlock>(new Variable);
   case TemplateType: return QPtr<BasicBlock>(new Template);
   case FunctionType: return QPtr<BasicBlock>(new Function);
   case ClassType: return QPtr<BasicBlock>(new Class);
   case AccessType: return QPtr<BasicBlock>(new Access);
   case ParentType: return QPtr<BasicBlock>(new Parent);
   case DeclarationType: return QPtr<BasicBlock>(new Declaration);
   default: return nullptr;
   }
}
