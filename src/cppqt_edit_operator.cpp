#include "cppqt_edit_operator.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <exception.h>
#include "cppqt_operator.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Operator::Operator(AbstractBlock* block, QWidget* parent):
   Function(block,parent,false),
   _block(qobject_cast<CppQt::Operator*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   saveSettings("cppqt.edit.operator.geometry");
}






QLayout* Operator::layout()
{
   QFormLayout* ret {new QFormLayout};
   addOperation(ret);
   Base::addDescription(ret);
   Function::addReturn(ret);
   Function::addOperations(ret);
   Function::addProperties(ret);
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
