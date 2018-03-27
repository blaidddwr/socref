#include "cppqt_edit_slot.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_slot.h"



using namespace CppQt::Edit;






Slot::Slot(CppQt::Slot* block, QWidget* parent):
   Function(block,parent)
{}






QLayout* Slot::layout()
{
   saveSettings("cppqt.edit.slot.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addOperations(ret);
   Function::addProperties(ret);
   return ret;
}
