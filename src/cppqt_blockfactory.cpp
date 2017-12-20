#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_view_namespace.h"
#include "cppqt_edit_namespace.h"



using namespace CppQt;






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
   Q_UNUSED(type);
   return QIcon();
}






const QList<int> BlockFactory::buildList(int type) const
{
   switch (type)
   {
   case NamespaceType: return QList<int>() << NamespaceType;
   default: return QList<int>();
   }
}






AbstractBlock* BlockFactory::makeRootBlock() const
{
   Namespace* ret {new Namespace(*this,NamespaceType)};
   ret->setName("");
   return ret;
}






AbstractBlock* BlockFactory::makeBlock(int type) const
{
   switch (type)
   {
   case NamespaceType: return new Namespace(*this,NamespaceType);
   default: return nullptr;
   }
}






QWidget* BlockFactory::makeView(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return new View::Namespace(block);
   default: return nullptr;
   }
}






Gui::AbstractEdit* BlockFactory::makeEdit(int type, AbstractBlock* block) const
{
   switch (type)
   {
   case NamespaceType: return new Edit::Namespace(block);
   default: return nullptr;
   }
}
