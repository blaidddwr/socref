#include "cppqt_typelist.h"
#include "cppqt_view_typelist.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace CppQt;






int TypeList::type() const
{
   return BlockFactory::TypeListType;
}






const AbstractBlockFactory& TypeList::factory() const
{
   return BlockFactory::instance();
}






QString TypeList::name() const
{
   return QString("Types ").append("(").append(QString::number(size())).append(")");
}






QIcon TypeList::icon() const
{
   QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/typelist.svg");
   return ret;
}






QList<int> TypeList::buildList() const
{
   QList<int> ret {BlockFactory::TypeType};
   return ret;
}






std::unique_ptr<QWidget> TypeList::makeView() const
{
   return unique_ptr<QWidget>(new View::TypeList(this));
}






std::unique_ptr<::Gui::AbstractEdit> TypeList::makeEdit()
{
   return nullptr;
}






void TypeList::childNameChanged(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void TypeList::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void TypeList::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void TypeList::readData(const QDomElement& element, int version)
{
   Q_UNUSED(element)
   Q_UNUSED(version)
}






int TypeList::writeVersion() const
{
   return _version;
}






QDomElement TypeList::writeData(QDomDocument& document) const
{
   return document.createElement("na");
}






std::unique_ptr<AbstractBlock> TypeList::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new TypeList);
}






void TypeList::copyDataFrom(const AbstractBlock* other)
{
   Q_UNUSED(other)
}
