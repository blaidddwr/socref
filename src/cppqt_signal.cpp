#include "cppqt_signal.h"
#include "cppqt_view_signal.h"
#include "cppqt_edit_signal.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






Signal::Signal(const QString& name):
   Slot(name)
{}






int Signal::type() const
{
   return BlockFactory::SignalType;
}






QIcon Signal::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/signal.svg");
   return ret;
}






std::unique_ptr<QWidget> Signal::makeView() const
{
   return unique_ptr<QWidget>(new View::Signal(this));
}






std::unique_ptr<::Gui::AbstractEdit> Signal::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Signal(this));
}






std::unique_ptr<AbstractBlock> Signal::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Signal);
}
