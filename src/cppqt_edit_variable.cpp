#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
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
   QGroupBox* type {new QGroupBox(tr("Type"),this)};
   QGroupBox* basic {new QGroupBox(tr("Basic Information"),this)};
   type->setLayout(createTypeCombo());
   basic->setLayout(Base::layout());
   layout->addWidget(type);
   layout->addWidget(basic);
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
   QHBoxLayout* ret {new QHBoxLayout};
   QLabel* label {new QLabel(tr("Value:"),this)};
   _type = new TypeComboBox(_block,this);
   _type->setCurrentIndex(_block->variableType());
   ret->addStretch();
   ret->addWidget(label);
   ret->addWidget(_type);
   ret->addStretch();
   return ret;
}
