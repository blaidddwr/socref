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
   Base::addFields(ret);
   addOperations(ret);
   Function::addProperties(ret);
   return ret;
}
