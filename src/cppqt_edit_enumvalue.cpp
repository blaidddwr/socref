#include "cppqt_edit_enumvalue.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QLabel>
#include <exception.h>
#include "cppqt_enumvalue.h"



using namespace CppQt::Edit;






EnumValue::EnumValue(AbstractBlock* block, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::EnumValue*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   saveSettings("cppqt.edit.enumvalue.geometry");
}






QLayout* EnumValue::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addValue(ret);
   return ret;
}






bool EnumValue::apply()
{
   if ( !Base::apply() ) return false;
   if ( _valueEdit->hasAcceptableInput() ) _block->setValue(_valueEdit->text().toInt());
   else _block->clearValue();
   return true;
}






void EnumValue::addValue(QFormLayout* layout)
{
   _valueEdit = new QLineEdit;
   _valueEdit->setValidator(new QIntValidator);
   if ( _block->hasValue() ) _valueEdit->setText(QString::number(_block->value()));
   layout->addRow(new QLabel(tr("Value:")),_valueEdit);
}
