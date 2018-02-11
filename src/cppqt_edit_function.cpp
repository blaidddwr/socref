#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "cppqt_edit_function.h"
#include "cppqt_function.h"
#include "exception.h"
#include "gui_textedit.h"
#include "cppqt_gui_listdialog.h"
#include "cppqt_common.h"



using namespace CppQt::Gui;
using namespace CppQt::Edit;






Function::Function(AbstractBlock* block, QWidget* parent):
   Variable(block,parent),
   _block(qobject_cast<CppQt::Function*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Function::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Return"));
   addReturn(ret);
   addTitle(ret,tr("Basic Information"));
   Base::addFields(ret);
   addTitle(ret,tr("Operations"));
   addOperations(ret);
   addTitle(ret,tr("Properties"));
   Variable::addProperties(ret);
   addProperties(ret);
   return ret;
}






bool Function::isConstExprCheckable() const
{
   return Variable::isConstExprCheckable() && ( !_virtualBox || !_virtualBox->isChecked() );
}






bool Function::isStaticCheckable() const
{
   return Variable::isStaticCheckable() &&  ( !_virtualBox || !_virtualBox->isChecked() );
}






void Function::updateProperties()
{
   Variable::updateProperties();
   if ( _virtualBox ) _virtualBox->setCheckable( !isConstExprChecked() && !isStaticChecked() && !_block->hasTemplates() && _block->isMethod() );
   if ( _constBox ) _constBox->setCheckable(_block->isMethod());
   if ( _overrideBox ) _overrideBox->setCheckable( _virtualBox->isChecked() && !_abstractBox->isChecked() );
   if ( _finalBox ) _finalBox->setCheckable( _virtualBox->isChecked() && !_abstractBox->isChecked() );
   if ( _abstractBox ) _abstractBox->setCheckable( _virtualBox->isChecked() && !_overrideBox->isChecked() && !_finalBox->isChecked() );
}






void Function::addReturn(QFormLayout* layout)
{
   _returnEdit = new ::Gui::TextEdit;
   _returnEdit->setPlainText(_block->returnDescription());
   Variable::addCombo(layout);
   layout->addRow(new QLabel(tr("Description:")),_returnEdit);
}






void Function::addProperties(QFormLayout* layout)
{
   addConst(layout);
   addNoExcept(layout);
   addVirtual(layout);
   addAbstract(layout);
   addOverride(layout);
   addFinal(layout);
}






void Function::addConst(QFormLayout* layout)
{
   _constBox = new QCheckBox(tr("Constant"));
   connect(_constBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _constBox->setChecked(_block->isConst());
   layout->addRow(_constBox);
}






void Function::addNoExcept(QFormLayout* layout)
{
   _noExceptBox = new QCheckBox(tr("No Exceptions"));
   connect(_noExceptBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _noExceptBox->setChecked(_block->isNoExcept());
   layout->addRow(_noExceptBox);
}






void Function::addVirtual(QFormLayout* layout)
{
   _virtualBox = new QCheckBox(tr("Virtual"));
   connect(_virtualBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _virtualBox->setChecked(_block->isVirtual());
   layout->addRow(_virtualBox);
}






void Function::addOverride(QFormLayout* layout)
{
   _overrideBox = new QCheckBox(tr("Override"));
   connect(_overrideBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _overrideBox->setChecked(_block->isOverride());
   layout->addRow(_overrideBox);
}






void Function::addFinal(QFormLayout* layout)
{
   _finalBox = new QCheckBox(tr("Final"));
   connect(_finalBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _finalBox->setChecked(_block->isFinal());
   layout->addRow(_finalBox);
}






void Function::addAbstract(QFormLayout* layout)
{
   _abstractBox = new QCheckBox(tr("Abstract (Pure Virtual)"));
   connect(_abstractBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   updateProperties();
   _abstractBox->setChecked(_block->isAbstract());
   layout->addRow(_abstractBox);
}






void Function::addOperations(QFormLayout* layout)
{
   QPushButton* edit {new QPushButton(tr("Edit"))};
   connect(edit,&QPushButton::clicked,this,&Function::operationsClicked);
   layout->addRow(new QLabel(tr("List:")),edit);
}






void Function::applyClicked()
{
   Variable::applyClicked();
   _block->setReturnDescription(_returnEdit->toPlainText());
   if ( _virtualBox ) _block->setVirtual(_virtualBox->isChecked());
   if ( _constBox ) _block->setConst(_constBox->isChecked());
   if ( _noExceptBox ) _block->setNoExcept(_noExceptBox->isChecked());
   if ( _overrideBox ) _block->setOverride(_overrideBox->isChecked());
   if ( _finalBox ) _block->setFinal(_finalBox->isChecked());
   if ( _abstractBox ) _block->setAbstract(_abstractBox->isChecked());
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
