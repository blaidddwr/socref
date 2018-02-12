#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_slot.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Slot::Slot(AbstractBlock* block, QWidget* parent):
   Function(block,parent)
{}






QLayout* Slot::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Basic Information"));
   Base::addDescription(ret);
   addTitle(ret,tr("Operations"));
   addOperations(ret);
   addTitle(ret,tr("Properties"));
   Function::addProperties(ret);
   return ret;
}
