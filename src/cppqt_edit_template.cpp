#include "cppqt_edit_template.h"
#include <QFormLayout>
#include <QLabel>



using namespace CppQt::Edit;






Template::Template(AbstractBlock* block, QWidget* parent):
   Variable(block,parent,false)
{
   saveSettings("cppqt.edit.template.geometry");
}






QLayout* Template::layout()
{
   QFormLayout* ret {new QFormLayout};
   addCombo(ret);
   Base::addFields(ret);
   return ret;
}
