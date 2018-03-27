#include "cppqt_edit_signal.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_signal.h"



using namespace CppQt::Edit;






Signal::Signal(CppQt::Signal* block, QWidget* parent):
   Slot(block,parent)
{}






QLayout* Signal::layout()
{
   saveSettings("cppqt.edit.signal.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   return ret;
}
