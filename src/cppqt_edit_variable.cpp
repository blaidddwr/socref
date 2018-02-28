#include "cppqt_edit_variable.h"
#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <exception.h>
#include "cppqt_gui_typecombobox.h"
#include "cppqt_variable.h"
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
   addCombo(ret);
   Base::addFields(ret);
   addInitializer(ret);
   addProperties(ret);
   return ret;
}






bool Variable::apply()
{
   if ( !Base::apply() ) return false;
   if ( _type ) _block->setVariableType(_type->value());
   if ( _constExprBox ) _block->setConstExpr(_constExprBox->isChecked());
   if ( _staticBox ) _block->setStatic(_staticBox->isChecked());
   if ( _initializerEdit ) _block->setInitializer(_initializerEdit->text());
   return true;
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
   setupCombo();
   layout->addRow(new QLabel(tr("Type:")),_type);
}






void Variable::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupProperties());
}






void Variable::addInitializer(QFormLayout* layout)
{
   setupInitializer();
   layout->addRow(new QLabel(tr("Initializer:")),_initializerEdit);
}






bool Variable::isConstExprChecked() const
{
   return _constExprBox && _constExprBox->isChecked();
}






bool Variable::isStaticChecked() const
{
   return _staticBox && _staticBox->isChecked();
}






QWidget* Variable::setupConstExpr()
{
   _constExprBox = new QCheckBox(tr("Constant Expression"));
   updateProperties();
   _constExprBox->setChecked(_block->isConstExpr());
   connect(_constExprBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   return _constExprBox;
}






QWidget* Variable::setupStatic()
{
   _staticBox = new QCheckBox(tr("Static"));
   updateProperties();
   _staticBox->setChecked(_block->isStatic());
   connect(_constExprBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   return _staticBox;
}






void Variable::checkBoxChanged(int state)
{
   Q_UNUSED(state)
   updateProperties();
}






void Variable::setupCombo()
{
   _type = new TypeComboBox(_block);
   _type->setCurrentIndex(_block->variableType());
}






QLayout*Variable::setupProperties()
{
   QGridLayout* ret {new QGridLayout};
   ret->addWidget(setupConstExpr(),0,0);
   ret->addWidget(setupStatic(),1,0);
   return ret;
}






void Variable::setupInitializer()
{
   _initializerEdit = new QLineEdit;
   _initializerEdit->setText(_block->initializer());
}
