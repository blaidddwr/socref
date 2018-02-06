#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_variable.h"
#include "cppqt_function.h"
#include "cppqt_template.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_view_namespace.h"
#include "cppqt_view_variable.h"
#include "cppqt_view_function.h"
#include "cppqt_view_template.h"
#include "cppqt_view_class.h"
#include "cppqt_view_access.h"
#include "cppqt_edit_namespace.h"
#include "cppqt_edit_variable.h"
#include "cppqt_edit_function.h"
#include "cppqt_edit_template.h"
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
   case NamespaceType: return QString("Namespace");
   case VariableType: return QString("Variable");
   case FunctionType: return QString("Function");
   case TemplateType: return QString("Template");
   case ClassType: return QString("Class");
   case AccessType: return QString("Access");
   default: return QString();
   }
}






QString BlockFactory::elementName(int type) const
{
   switch (type)
   {
   case NamespaceType: return QString("namespace");
   case VariableType: return QString("variable");
   case FunctionType: return QString("function");
   case TemplateType: return QString("template");
   case ClassType: return QString("class");
   case AccessType: return QString("access");
   default: return QString("unknown");
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
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
