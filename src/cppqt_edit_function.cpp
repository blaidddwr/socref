#include "cppqt_edit_function.h"
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <exception.h>
#include "cppqt_gui_listdialog.h"
#include "cppqt_function.h"
#include "gui_textedit.h"



using namespace CppQt::Gui;
using namespace CppQt::Edit;






Function::Function(AbstractBlock* block, QWidget* parent, bool isFinal):
   Variable(block,parent,false),
   _block(qobject_cast<CppQt::Function*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   if ( isFinal ) saveSettings("cppqt.edit.function.geometry");
}






QLayout* Function::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addReturn(ret);
   addOperations(ret);
   addProperties(ret);
   return ret;
}






void Function::apply()
{
   Variable::apply();
   if ( _returnEdit ) _block->setReturnDescription(_returnEdit->toPlainText());
   if ( _defaultBox ) _block->setDefault(_defaultBox->isChecked());
   if ( _virtualBox ) _block->setVirtual(_virtualBox->isChecked());
   if ( _constBox ) _block->setConst(_constBox->isChecked());
   if ( _noExceptBox ) _block->setNoExcept(_noExceptBox->isChecked());
   if ( _overrideBox ) _block->setOverride(_overrideBox->isChecked());
   if ( _finalBox ) _block->setFinal(_finalBox->isChecked());
   if ( _abstractBox ) _block->setAbstract(_abstractBox->isChecked());
}






bool Function::isConstExprCheckable() const
{
   return Variable::isConstExprCheckable() && ( !_virtualBox || !_virtualBox->isChecked() );
}






bool Function::isStaticCheckable() const
{
   return Variable::isStaticCheckable() &&  ( !_virtualBox || !_virtualBox->isChecked() );
}






bool Function::isDefaultCheckable()
{
   return true;
}






void Function::updateProperties()
{
   Variable::updateProperties();
   if ( _virtualBox ) _virtualBox->setCheckable( !isConstExprChecked()
                                                 && !isStaticChecked()
                                                 && !_block->hasAnyTemplates()
                                                 && _block->isMethod() );
   if ( _constBox ) _constBox->setCheckable(_block->isMethod());
   if ( _overrideBox ) _overrideBox->setCheckable( isVirtualChecked() && !isAbstractChecked() );
   if ( _finalBox ) _finalBox->setCheckable( isVirtualChecked() && !isAbstractChecked() );
   if ( _abstractBox ) _abstractBox->setCheckable( isVirtualChecked()
                                                   && !isOverrideChecked()
                                                   && !isFinalChecked() );
}






bool Function::isDefaultChecked() const
{
   return _defaultBox && _defaultBox->isChecked();
}






bool Function::isConstChecked() const
{
   return _constBox && _constBox->isChecked();
}






bool Function::isNoExceptChecked() const
{
   return _noExceptBox && _noExceptBox->isChecked();
}






bool Function::isVirtualChecked() const
{
   return _virtualBox && _virtualBox->isChecked();
}






bool Function::isOverrideChecked() const
{
   return _overrideBox && _overrideBox->isChecked();
}






bool Function::isFinalChecked() const
{
   return _finalBox && _finalBox->isChecked();
}






bool Function::isAbstractChecked() const
{
   return _abstractBox && _abstractBox->isChecked();
}






void Function::addReturn(QFormLayout* layout)
{
   setupReturn();
   Variable::addCombo(layout);
   layout->addRow(new QLabel(tr("Return Description:")),_returnEdit);
}






void Function::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Propreties:")),setupProperties());
}






void Function::addOperations(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Operations:")),setupOperations());
}






QGridLayout* Function::setupProperties()
{
   QGridLayout* ret {new QGridLayout};
   ret->addWidget(setupConstExpr(),0,0);
   ret->addWidget(setupStatic(),1,0);
   ret->addWidget(setupConst(),2,0);
   ret->addWidget(setupNoExcept(),3,0);
   ret->addWidget(setupVirtual(),0,1);
   ret->addWidget(setupAbstract(),1,1);
   ret->addWidget(setupOverride(),2,1);
   ret->addWidget(setupFinal(),3,1);
   return ret;
}






QWidget* Function::setupDefault()
{
   _defaultBox = new QCheckBox(tr("Default"));
   connect(_defaultBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _defaultBox->setChecked(_block->isDefault());
   return _defaultBox;
}






QWidget* Function::setupConst()
{
   _constBox = new QCheckBox(tr("Constant"));
   connect(_constBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _constBox->setChecked(_block->isConst());
   return _constBox;
}






QWidget* Function::setupNoExcept()
{
   _noExceptBox = new QCheckBox(tr("No Exceptions"));
   connect(_noExceptBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _noExceptBox->setChecked(_block->isNoExcept());
   return _noExceptBox;
}






QWidget* Function::setupVirtual()
{
   _virtualBox = new QCheckBox(tr("Virtual"));
   connect(_virtualBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _virtualBox->setChecked(_block->isVirtual());
   return _virtualBox;
}






QWidget* Function::setupOverride()
{
   _overrideBox = new QCheckBox(tr("Override"));
   connect(_overrideBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _overrideBox->setChecked(_block->isOverride());
   return _overrideBox;
}






QWidget* Function::setupFinal()
{
   _finalBox = new QCheckBox(tr("Final"));
   connect(_finalBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _finalBox->setChecked(_block->isFinal());
   return _finalBox;
}






QWidget* Function::setupAbstract()
{
   _abstractBox = new QCheckBox(tr("Abstract (Pure Virtual)"));
   connect(_abstractBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _abstractBox->setChecked(_block->isAbstract());
   return _abstractBox;
}






void Function::checkBoxChanged(int state)
{
   Q_UNUSED(state)
   updateProperties();
}






void Function::operationsClicked()
{
   ListDialog dialog;
   dialog.setWindowTitle(tr("Edit Function Operations"));
   dialog.setValue(_block->operations());
   if ( dialog.exec() )
   {
      _block->setOperations(dialog.value());
   }
}






void Function::setupReturn()
{
   _returnEdit = new ::Gui::TextEdit;
   _returnEdit->setPlainText(_block->returnDescription());
}






QPushButton*Function::setupOperations()
{
   QPushButton* ret {new QPushButton(tr("Edit"))};
   connect(ret,&QPushButton::clicked,this,&Function::operationsClicked);
   return ret;
}
