#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include "cppqt_edit_variable.h"
#include "exception.h"
#include "cppqt_variable.h"
#include "cppqt_gui_typecombobox.h"
#include "cppqt_common.h"



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
}






QLayout* Variable::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Type"));
   addCombo(ret);
   addTitle(ret,tr("Basic Information"));
   Base::addFields(ret);
   addTitle(ret,tr("Initializer"));
   addInitializer(ret);
   addTitle(ret,tr("Properties"));
   addProperties(ret);
   return ret;
}






bool Variable::isConstExprCheckable() const
{
   return !_block->isFunctionArgument();
}






bool Variable::isStaticCheckable() const
{
   return _block->isClassMember();
}






void Variable::updateProperties()
{
   if ( _constExprBox ) _constExprBox->setCheckable(isConstExprCheckable());
   if ( _staticBox ) _staticBox->setCheckable(isStaticCheckable());
}






void Variable::addCombo(QFormLayout* layout)
{
   _type = new TypeComboBox(_block);
   _type->setCurrentIndex(_block->variableType());
   layout->addRow(new QLabel(tr("Type:")),_type);
}






void Variable::addProperties(QFormLayout* layout)
{
   addConstExpr(layout);
   addStatic(layout);
}






void Variable::addConstExpr(QFormLayout* layout)
{
   _constExprBox = new QCheckBox(tr("Constant Expression"));
   updateProperties();
   _constExprBox->setChecked(_block->isConstExpr());
   connect(_constExprBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   layout->addRow(_constExprBox);
}






void Variable::addStatic(QFormLayout* layout)
{
   _staticBox = new QCheckBox(tr("Static"));
   updateProperties();
   _staticBox->setChecked(_block->isStatic());
   connect(_constExprBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   layout->addRow(_staticBox);
}






void Variable::addInitializer(QFormLayout* layout)
{
   _initializerEdit = new QLineEdit;
   _initializerEdit->setText(_block->initializer());
   layout->addRow(new QLabel(tr("Value:")),_initializerEdit);
}






const QCheckBox* Variable::constExprBox() const
{
   return _constExprBox;
}






const QCheckBox* Variable::staticBox() const
{
   return _staticBox;
}






void Variable::applyClicked()
{
   Base::applyClicked();
   if ( _type ) _block->setVariableType(_type->value());
   if ( _constExprBox ) _block->setConstExpr(_constExprBox->isChecked());
   if ( _staticBox ) _block->setStatic(_staticBox->isChecked());
   if ( _initializerEdit ) _block->setInitializer(_initializerEdit->text());
}






void Variable::checkBoxChanged(int state)
{
   Q_UNUSED(state)
   updateProperties();
}
