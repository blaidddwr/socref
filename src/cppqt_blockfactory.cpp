#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_variable.h"
#include "cppqt_view_namespace.h"
#include "cppqt_view_variable.h"
#include "cppqt_edit_namespace.h"
#include "cppqt_edit_variable.h"
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
   default: return QString();
   }
}






QString BlockFactory::elementName(int type) const
{
   switch (type)
   {
   case NamespaceType: return QString("namespace");
   case VariableType: return QString("variable");
   default: return QString("unknown");
   }
}






QIcon BlockFactory::icon(int type) const
{
   static QIcon namespace_;
   static QIcon variable;
   if ( namespace_.isNull() )
   {
      namespace_ = QIcon(":/icons/namespace.svg");
   }
   if ( variable.isNull() )
   {
      variable = QIcon(":/icons/variable.svg");
   }
   switch (type)
   {
   case NamespaceType: return namespace_;
   case VariableType: return variable;
   default: return QIcon();
   }
}






const QList<int> BlockFactory::buildList(int type) const
{
   switch (type)
   {
   case NamespaceType: return QList<int>() << NamespaceType << VariableType;
   case VariableType: return QList<int>();
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
   case VariableType: return unique_ptr<AbstractBlock>(new Variable("unnamed_variable"));
   default: return nullptr;
   }
}






unique_ptr<QWidget> BlockFactory::makeView(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<QWidget>(new View::Namespace(block));
   case VariableType: return unique_ptr<QWidget>(new View::Variable(block));
   default: return nullptr;
   }
}






unique_ptr<AbstractEdit> BlockFactory::makeEdit(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<AbstractEdit>(new Edit::Namespace(block));
   case VariableType: return unique_ptr<AbstractEdit>(new Edit::Variable(block));
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
