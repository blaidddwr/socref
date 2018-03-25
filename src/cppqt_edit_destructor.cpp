#include "cppqt_edit_destructor.h"
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include "cppqt_common.h"



using namespace CppQt::Edit;






Destructor::Destructor(AbstractBlock* block, QWidget* parent):
   Function(block,parent,false)
{
   saveSettings("cppqt.edit.destructor.geometry");
}






QLayout* Destructor::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addDescription(ret);
   addOperations(ret);
   addProperties(ret);
   return ret;
}






void Destructor::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupProperties());
}






QLayout* Destructor::setupProperties()
{
   QGridLayout* ret {new QGridLayout};
   ret->addWidget(setupVirtual(),0,0);
   ret->addWidget(setupAbstract(),1,0);
   ret->addWidget(setupOverride(),2,0);
   ret->addWidget(setupFinal(),3,0);
   ret->addWidget(setupNoExcept(),0,1);
   ret->addWidget(setupDefault(),1,1);
   return ret;
}
