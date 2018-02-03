#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_variable.h"
#include "cppqt_function.h"
#include "cppqt_template.h"
#include "cppqt_view_namespace.h"
#include "cppqt_view_variable.h"
#include "cppqt_view_function.h"
#include "cppqt_view_template.h"
#include "cppqt_edit_namespace.h"
#include "cppqt_edit_variable.h"
#include "cppqt_edit_function.h"
//#include "cppqt_edit_template.h"
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
   default: return QString("unknown");
   }
}






QIcon BlockFactory::icon(int type) const
{
   static bool isLoaded {false};
   static QIcon namespace_;
   static QIcon variable;
   static QIcon function_;
   static QIcon template_;
   if ( !isLoaded )
   {
      namespace_ = QIcon(":/icons/namespace.svg");
      variable = QIcon(":/icons/variable.svg");
      function_ = QIcon(":/icons/function.svg");
      template_ = QIcon(":/icons/template.svg");
      isLoaded = true;
   }
   switch (type)
   {
   case NamespaceType: return namespace_;
   case VariableType: return variable;
   case FunctionType: return function_;
   case TemplateType: return template_;
   default: return QIcon();
   }
}






QList<int> BlockFactory::buildList(int type) const
{
   switch (type)
   {
   case NamespaceType: return QList<int>() << NamespaceType << VariableType << FunctionType;
   case VariableType: return QList<int>();
   case FunctionType: return QList<int>() << VariableType << TemplateType;
   case TemplateType: return QList<int>();
   default: return QList<int>();
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
   default: return nullptr;
   }
}






unique_ptr<QWidget> BlockFactory::makeView(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<QWidget>(new View::Namespace(block));
   case VariableType: return unique_ptr<QWidget>(new View::Variable(block));
   case FunctionType: return unique_ptr<QWidget>(new View::Function(block));
   case TemplateType: return unique_ptr<QWidget>(new View::Template(block));
   default: return nullptr;
   }
}






unique_ptr<AbstractEdit> BlockFactory::makeEdit(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<AbstractEdit>(new Edit::Namespace(block));
   case VariableType: return unique_ptr<AbstractEdit>(new Edit::Variable(block));
   case FunctionType: return unique_ptr<AbstractEdit>(new Edit::Function(block));
   case TemplateType: return unique_ptr<AbstractEdit>();
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
