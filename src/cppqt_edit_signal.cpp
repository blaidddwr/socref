#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_signal.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Signal::Signal(AbstractBlock* block, QWidget* parent):
   Slot(block,parent)
{}






QLayout* Signal::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Basic Information"));
   Base::addDescription(ret);
   return ret;
}
