#include "cppqt_edit_constructor.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "cppqt_constructor.h"



using namespace CppQt::Edit;






Constructor::Constructor(CppQt::Constructor* block, QWidget* parent):
   Function(block,parent)
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
