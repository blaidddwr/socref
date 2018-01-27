#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_view_namespace.h"
#include "cppqt_edit_namespace.h"
#include "projectfactory.h"



using namespace std;
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
   default: return QString();
   }
}






QString BlockFactory::elementName(int type) const
{
   switch (type)
   {
   case NamespaceType: return QString("namespace");
   default: return QString("unknown");
   }
}






QIcon BlockFactory::icon(int type) const
{
   static QIcon namespace_;
   if ( namespace_.isNull() )
   {
      namespace_ = QIcon(":/icons/namespace.svg");
   }
   switch (type)
   {
   case NamespaceType: return namespace_;
   default: return QIcon();
   }
}






const QList<int> BlockFactory::buildList(int type) const
{
   switch (type)
   {
   case NamespaceType: return QList<int>() << NamespaceType;
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
   default: return nullptr;
   }
}






unique_ptr<QWidget> BlockFactory::makeView(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<QWidget>(new View::Namespace(block));
   default: return nullptr;
   }
}






unique_ptr<Gui::AbstractEdit> BlockFactory::makeEdit(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return unique_ptr<Gui::AbstractEdit>(new Edit::Namespace(block));
   default: return nullptr;
   }
}






int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}
