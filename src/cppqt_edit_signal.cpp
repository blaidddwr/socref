#include "cppqt_edit_signal.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_common.h"



using namespace CppQt::Edit;






Signal::Signal(AbstractBlock* block, QWidget* parent):
   Slot(block,parent)
{}






QLayout* Signal::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   return ret;
}
