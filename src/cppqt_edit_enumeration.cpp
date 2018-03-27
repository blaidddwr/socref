#include "cppqt_edit_enumeration.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <exception.h>
#include "cppqt_enumeration.h"



using namespace CppQt::Edit;






Enumeration::Enumeration(CppQt::Enumeration* block, QWidget* parent):
   Base(block,parent),
   _block(block)
{}






QLayout* Enumeration::layout()
{
   saveSettings("cppqt.edit.enumeration.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addClass(ret);
   return ret;
}






void Enumeration::apply()
{
   Base::apply();
   _block->setClass(_classBox->isChecked());
}






void Enumeration::addClass(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupClass());
}






QCheckBox* Enumeration::setupClass()
{
   _classBox = new QCheckBox(tr("Class"));
   _classBox->setChecked(_block->isClass());
   return _classBox;
}
