#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_view_namespace.h"



using namespace CppQt;






//@@
QString BlockFactory::getName(int type) const
{
   // determine which type and return name
   switch (type)
   {
   case NamespaceType: return QString("Namespace");
   default: return QString();
   }
}






//@@
QString BlockFactory::getElementName(int type) const
{
   // determine which type and return name
   switch (type)
   {
   case NamespaceType: return QString("namespace");
   default: return QString("unknown");
   }
}






//@@
QIcon BlockFactory::getIcon(int type) const
{
   //TODO; need to do in the future
   Q_UNUSED(type);
   return QIcon();
}






//@@
const QList<int> BlockFactory::getBuildList(int type) const
{
   // determine which type and return build list
   switch (type)
   {
   case NamespaceType: return QList<int>() << NamespaceType;
   default: return QList<int>();
   }
}






//@@
AbstractBlock* BlockFactory::makeRootBlock() const
{
   // create global namespace with no name and return
   Namespace* ret {new Namespace(*this,NamespaceType)};
   ret->setName("");
   return ret;
}






//@@
AbstractBlock* BlockFactory::makeBlock(int type) const
{
   // determine which type and return pointer to created block
   switch (type)
   {
   case NamespaceType: return new Namespace(*this,NamespaceType);
   default: return nullptr;
   }
}






//@@
QWidget* BlockFactory::makeView(int type, AbstractBlock* block) const
{
   // determine which type and return pointer to created view
   switch (type)
   {
   case NamespaceType: return new View::Namespace(block);
   default: return nullptr;
   }
}






//@@
Gui::AbstractEdit* BlockFactory::makeEdit(int type) const
{
   //TODO; not yet complete
   Q_UNUSED(type);
   return nullptr;
}
