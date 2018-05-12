#include "cppqt_edit_typelist.h"
#include <QFormLayout>
#include "cppqt_typelist.h"



using namespace CppQt::Edit;






TypeList::TypeList(CppQt::TypeList* block, QWidget* parent):
   Base(block,parent)
{}






QLayout* TypeList::layout()
{
   saveSettings("cppqt.edit.typelist.geometry");
   QFormLayout* ret {new QFormLayout};
   addName(ret);
   return ret;
}
