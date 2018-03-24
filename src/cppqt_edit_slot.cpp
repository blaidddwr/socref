#include "cppqt_edit_slot.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_common.h"



using namespace CppQt::Edit;






Slot::Slot(AbstractBlock* block, QWidget* parent, bool isFinal):
   Function(block,parent,false)
{
   if ( isFinal ) saveSettings("cppqt.edit.slot.geometry");
}






QLayout* Slot::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addOperations(ret);
   Function::addProperties(ret);
   return ret;
}
