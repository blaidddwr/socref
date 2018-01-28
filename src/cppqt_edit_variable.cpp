#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include "cppqt_edit_variable.h"
#include "exception.h"
#include "cppqt_variable.h"
#include "cppqt_gui_typecombobox.h"



using namespace CppQt::Edit;
using namespace CppQt::Gui;






Variable::Variable(AbstractBlock* block, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Variable*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   QVBoxLayout* layout {new QVBoxLayout};
   QGroupBox* type {new QGroupBox(tr("Type"))};
   QGroupBox* basic {new QGroupBox(tr("Basic Information"))};
   type->setLayout(createTypeCombo());
   basic->setLayout(Base::layout());
   layout->addWidget(type);
   layout->addWidget(basic);
   layout->addStretch();
   _layout = layout;
}







QLayout* Variable::layout()
{
   return _layout;
}






void Variable::okClicked()
{
   applyClicked();
   finished();
}






void Variable::applyClicked()
{
   Base::applyClicked();
   _block->setVariableType(_type->value());
}






void Variable::cancelClicked()
{
   finished();
}






QLayout* Variable::createTypeCombo()
{
   QGridLayout* ret {new QGridLayout};
   QLabel* label {new QLabel(tr("Value:"))};
   label->setAlignment(Qt::AlignTop|Qt::AlignRight);
   _type = new TypeComboBox(_block);
   _type->setCurrentIndex(_block->variableType());
   ret->addWidget(label,0,0);
   ret->addWidget(_type,0,1);
   return ret;
}
