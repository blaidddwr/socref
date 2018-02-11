#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <exception.h>
#include "cppqt_edit_operator.h"
#include "cppqt_operator.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Operator::Operator(AbstractBlock* block, QWidget* parent):
   Function(block,parent),
   _block(qobject_cast<CppQt::Operator*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Operator::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Return"));
   addReturn(ret);
   addTitle(ret,tr("Basic Information"));
   addOperation(ret);
   Base::addDescription(ret);
   addTitle(ret,tr("Operations"));
   addOperations(ret);
   addTitle(ret,tr("Properties"));
   addProperties(ret);
   return ret;
}






void Operator::applyClicked()
{
   Function::applyClicked();
   _block->setOperation(_operationEdit->text());
}






void Operator::addOperation(QFormLayout* layout)
{
   _operationEdit = new QLineEdit;
   _operationEdit->setText(_block->operation());
   layout->addRow(new QLabel(tr("Operator:")),_operationEdit);
}
