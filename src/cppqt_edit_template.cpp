#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_template.h"



using namespace CppQt::Edit;






Template::Template(AbstractBlock* block, QWidget* parent):
   Variable(block,parent)
{}






QLayout* Template::layout()
{
   QFormLayout* ret {new QFormLayout};
   ret->addRow(createTitle(tr("Type")));
   addCombo(ret);
   ret->addRow(createTitle(tr("Basic Information")));
   Base::addFields(ret);
   return ret;
}






void Template::okClicked()
{
   applyClicked();
   emit finished();
}






void Template::cancelClicked()
{
   emit finished();
}
