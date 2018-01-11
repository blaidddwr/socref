#include "cppqt_blockfactory.h"
#include "cppqt_definition.h"
#include "cppqt_namespace.h"
#include "cppqt_view_definition.h"
#include "cppqt_view_namespace.h"
#include "cppqt_edit_definition.h"
#include "cppqt_edit_namespace.h"
#include "projectfactory.h"



using namespace std;
using namespace CppQt;






QString BlockFactory::name(int type) const
{
   switch (type)
   {
   case DefinitionType: return QString("Definition");
   case NamespaceType: return QString("Namespace");
   default: return QString();
   }
}






QString BlockFactory::elementName(int type) const
{
   switch (type)
   {
   case DefinitionType: return QString("definition");
   case NamespaceType: return QString("namespace");
   default: return QString("unknown");
   }
}






QIcon BlockFactory::icon(int type) const
{
   static QIcon namespace_;
   static QIcon definition;
   if ( namespace_.isNull() )
   {
      namespace_ = QIcon(":/icons/namespace.svg");
   }
   if ( definition.isNull() )
   {
      definition = QIcon(":/icons/definition.svg");
   }
   switch (type)
   {
   case DefinitionType: return definition;
   case NamespaceType: return namespace_;
   default: return QIcon();
   }
}






const QList<int> BlockFactory::buildList(int type) const
{
   switch (type)
   {
   case DefinitionType: return QList<int>();
   case NamespaceType: return QList<int>() << DefinitionType << NamespaceType;
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
   case DefinitionType: return unique_ptr<AbstractBlock>(new Definition("unnamed_type"));
   case NamespaceType: return unique_ptr<AbstractBlock>(new Namespace("unnamed_namespace"));
   default: return nullptr;
   }
}






unique_ptr<QWidget> BlockFactory::makeView(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case DefinitionType: return unique_ptr<QWidget>(new View::Definition(block));
   case NamespaceType: return unique_ptr<QWidget>(new View::Namespace(block));
   default: return nullptr;
   }
}






unique_ptr<Gui::AbstractEdit> BlockFactory::makeEdit(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case DefinitionType: return unique_ptr<Gui::AbstractEdit>(new Edit::Definition(block));
   case NamespaceType: return unique_ptr<Gui::AbstractEdit>(new Edit::Namespace(block));
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
