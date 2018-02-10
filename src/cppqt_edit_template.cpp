#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_template.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Template::Template(AbstractBlock* block, QWidget* parent):
   Variable(block,parent)
{}






QLayout* Template::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Type"));
   addCombo(ret);
   addTitle(ret,tr("Basic Information"));
   Base::addFields(ret);
   return ret;
}
