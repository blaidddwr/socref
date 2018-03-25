#include "cppqt_slot.h"
#include "cppqt_view_slot.h"
#include "cppqt_edit_slot.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






Slot::Slot(const QString& name):
   Function(name)
{}






QString Slot::name() const
{
   return fullName(false,Base::name());
}






int Slot::type() const
{
   return BlockFactory::SlotType;
}






QIcon Slot::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/slot.svg");
      virtual_ = QIcon(":/icons/vslot.svg");
      abstract = QIcon(":/icons/aslot.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






QList<int> Slot::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






std::unique_ptr<QWidget> Slot::makeView() const
{
   return unique_ptr<QWidget>(new View::Slot(this));
}






std::unique_ptr<::Gui::AbstractEdit> Slot::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Slot(this));
}






std::unique_ptr<AbstractBlock> Slot::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Slot);
}
