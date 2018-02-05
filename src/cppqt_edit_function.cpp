#include <QFormLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QRegExpValidator>
#include "cppqt_edit_function.h"
#include "cppqt_function.h"
#include "exception.h"
#include "cppqt_gui_typecombobox.h"



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
   ret->addRow(createTitle(tr("Return")));
   addReturn(ret);
   ret->addRow(createTitle(tr("Basic Information")));
   Base::addFields(ret);
   ret->addRow(createTitle(tr("Properties")));
   addProperties(ret);
   return ret;
}






void Function::addReturn(QFormLayout* layout)
{
   _returnEdit = new QPlainTextEdit;
   _returnEdit->setPlainText(_block->returnDescription());
   Variable::addCombo(layout);
   layout->addRow(new QLabel(tr("Description:")),_returnEdit);
}






void Function::addProperties(QFormLayout* layout)
{
   Variable::addProperties(layout);
   createPropertiesWidgets();
   layout->addRow(_virtualBox);
   layout->addRow(_constBox);
   layout->addRow(_noExceptBox);
   layout->addRow(_overrideBox);
   layout->addRow(_finalBox);
   layout->addRow(_abstractBox);
}






void Function::okClicked()
{
   applyClicked();
   emit finished();
}






void Function::applyClicked()
{
   Variable::applyClicked();
   _block->setReturnDescription(_returnEdit->toPlainText());
   _block->setVirtual(_virtualBox->isChecked());
   _block->setConst(_constBox->isChecked());
   _block->setNoExcept(_noExceptBox->isChecked());
   _block->setOverride(_overrideBox->isChecked());
   _block->setFinal(_finalBox->isChecked());
   _block->setAbstract(_abstractBox->isChecked());
}






void Function::cancelClicked()
{
   emit finished();
}






void Function::checkBoxChanged(int state)
{
   Q_UNUSED(state)
   updateProperties();
}






void Function::createReturnWidgets()
{
   _returnEdit = new QPlainTextEdit;
   _returnEdit->setPlainText(_block->returnDescription());
}






void Function::createPropertiesWidgets()
{
   _virtualBox = new QCheckBox(tr("Virtual"));
   _constBox = new QCheckBox(tr("Constant"));
   _noExceptBox = new QCheckBox(tr("No Exceptions"));
   _overrideBox = new QCheckBox(tr("Override"));
   _finalBox = new QCheckBox(tr("Final"));
   _abstractBox = new QCheckBox(tr("Abstract (Pure Virtual)"));
   connectProperties();
   fillProperties();
   updateProperties();
}






void Function::connectProperties()
{
   connect(_virtualBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   connect(_constBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   connect(_noExceptBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   connect(_overrideBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   connect(_finalBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
   connect(_abstractBox,&QCheckBox::stateChanged,this,&Function::checkBoxChanged);
}






void Function::fillProperties()
{
   _virtualBox->setChecked(_block->isVirtual());
   _constBox->setChecked(_block->isConst());
   _noExceptBox->setChecked(_block->isNoExcept());
   _overrideBox->setChecked(_block->isOverride());
   _finalBox->setChecked(_block->isFinal());
   _abstractBox->setChecked(_block->isAbstract());
}






void Function::updateProperties()
{
   _virtualBox->setCheckable( !staticBox()->isChecked() && !_block->hasTemplates() && _block->isMethod() );
   _constBox->setCheckable(_block->isMethod());
   _overrideBox->setCheckable( _virtualBox->isChecked() && !_abstractBox->isChecked() );
   _finalBox->setCheckable( _virtualBox->isChecked() && !_abstractBox->isChecked() );
   _abstractBox->setCheckable( _virtualBox->isChecked() && !_overrideBox->isChecked() && !_finalBox->isChecked() );
}
