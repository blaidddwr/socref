#include "cppqt_edit_constructor.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "cppqt_constructor.h"



using namespace CppQt::Edit;






Constructor::Constructor(CppQt::Constructor* block, QWidget* parent):
   Function(block,parent),
   _block(block)
{}






QLayout* Constructor::layout()
{
   saveSettings("cppqt.edit.constructor.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addDescription(ret);
   addOperations(ret);
   addProperties(ret);
   return ret;
}






void Constructor::apply()
{
   Function::apply();
   _block->setExplicit(_explicitBox->isChecked());
}






void Constructor::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupProperties());
}






QLayout* Constructor::setupProperties()
{
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(setupNoExcept());
   ret->addWidget(setupExplicit());
   ret->addWidget(setupDefault());
   return ret;
}






QWidget*Constructor::setupExplicit()
{
   _explicitBox = new QCheckBox(tr("Explicit"));
   _explicitBox->setChecked(_block->isExplicit());
   return _explicitBox;
}
