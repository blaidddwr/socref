#include "cppqt_edit_variable.h"
#include <QLabel>
#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <exception.h>
#include "cppqt_gui_typeselection.h"
#include "cppqt_variable.h"



using namespace CppQt::Edit;
using namespace CppQt::Gui;






Variable::Variable(CppQt::Variable* block, QWidget* parent):
   Base(block,parent),
   _block(block)
{}






QLayout* Variable::layout()
{
   saveSettings("cppqt.edit.variable.geometry");
   QFormLayout* ret {new QFormLayout};
   addCombo(ret);
   Base::addFields(ret);
   addInitializer(ret);
   addProperties(ret);
   return ret;
}






void Variable::apply()
{
   Base::apply();
   if ( _type ) _block->setVariableType(_type->value());
   if ( _constExprBox ) _block->setConstExpr(_constExprBox->isChecked());
   if ( _staticBox ) _block->setStatic(_staticBox->isChecked());
   if ( _mutableBox ) _block->setMutable(_mutableBox->isChecked());
   if ( _initializerEdit ) _block->setInitializer(_initializerEdit->text());
}






bool Variable::isConstExprCheckable() const
{
   return !_block->isFunctionArgument() && !isMutableChecked();
}






bool Variable::isStaticCheckable() const
{
   return _block->isClassMember();
}






bool Variable::isMutableCheckable() const
{
   return _block->isClassMember() && !isConstExprChecked();
}






void Variable::updateProperties()
{
   if ( _constExprBox ) _constExprBox->setCheckable(isConstExprCheckable());
   if ( _staticBox ) _staticBox->setCheckable(isStaticCheckable());
   if ( _mutableBox ) _mutableBox->setCheckable(isMutableCheckable());
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






bool Variable::isMutableChecked() const
{
   return _mutableBox && _mutableBox->isChecked();
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
   connect(_staticBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   return _staticBox;
}






QWidget* Variable::setupMutable()
{
   _mutableBox = new QCheckBox(tr("Mutable"));
   updateProperties();
   _mutableBox->setChecked(_block->isMutable());
   connect(_mutableBox,&QCheckBox::stateChanged,this,&Variable::checkBoxChanged);
   return _mutableBox;
}






void Variable::checkBoxChanged(int state)
{
   Q_UNUSED(state)
   updateProperties();
}






void Variable::setupCombo()
{
   _type = new TypeSelection(_block);
   _type->setValue(_block->variableType());
}






QLayout*Variable::setupProperties()
{
   QGridLayout* ret {new QGridLayout};
   ret->addWidget(setupConstExpr(),0,0);
   ret->addWidget(setupStatic(),1,0);
   ret->addWidget(setupMutable(),2,0);
   return ret;
}






void Variable::setupInitializer()
{
   _initializerEdit = new QLineEdit;
   _initializerEdit->setText(_block->initializer());
}
