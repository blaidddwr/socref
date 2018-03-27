#include "cppqt_edit_template.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_template.h"



using namespace CppQt::Edit;






Template::Template(CppQt::Template* block, QWidget* parent):
   Variable(block,parent)
{}






QLayout* Template::layout()
{
   saveSettings("cppqt.edit.template.geometry");
   QFormLayout* ret {new QFormLayout};
   addCombo(ret);
   Base::addFields(ret);
   return ret;
}
