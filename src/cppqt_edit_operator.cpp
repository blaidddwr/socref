#include "cppqt_edit_operator.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <exception.h>
#include "cppqt_operator.h"



using namespace CppQt::Edit;






Operator::Operator(CppQt::Operator* block, QWidget* parent):
   Function(block,parent),
   _block(block)
{}






QLayout* Operator::layout()
{
   saveSettings("cppqt.edit.operator.geometry");
   QFormLayout* ret {new QFormLayout};
   addOperation(ret);
   Base::addDescription(ret);
   Function::addReturn(ret);
   Function::addOperations(ret);
   addProperties(ret);
   return ret;
}






void Operator::apply()
{
   Function::apply();
   _block->setOperation(_operationEdit->text());
}






void Operator::addOperation(QFormLayout* layout)
{
   setupOperation();
   layout->addRow(new QLabel(tr("Operator:")),_operationEdit);
}






void Operator::setupOperation()
{
   _operationEdit = new QLineEdit;
   _operationEdit->setText(_block->operation());
}






void Operator::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Propreties:")),setupProperties());
}






QGridLayout* Operator::setupProperties()
{
   QGridLayout* ret {Function::setupProperties()};
   ret->addWidget(setupDefault(),4,0);
   return ret;
}
