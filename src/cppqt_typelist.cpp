#include "cppqt_typelist.h"
#include "cppqt_view_typelist.h"
#include "cppqt_edit_typelist.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






TypeList::TypeList(const QString& name):
   Base(name)
{}






int TypeList::type() const
{
   return BlockFactory::TypeListType;
}






QString TypeList::name() const
{
   return Base::name().append(" (").append(QString::number(size())).append(")");
}






QIcon TypeList::icon() const
{
   QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/typelist.svg");
   return ret;
}






QList<int> TypeList::buildList() const
{
   QList<int> ret {BlockFactory::TypeListType,BlockFactory::TypeType};
   return ret;
}






std::unique_ptr<QWidget> TypeList::makeView() const
{
   return unique_ptr<QWidget>(new View::TypeList(this));
}






std::unique_ptr<::Gui::AbstractEdit> TypeList::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::TypeList(this));
}






void TypeList::childNameChanged(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
}






void TypeList::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
}






void TypeList::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
}






std::unique_ptr<AbstractBlock> TypeList::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new TypeList);
}
