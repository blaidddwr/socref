#include "cppqt_edit_enumvalue.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QLabel>
#include <exception.h>
#include "cppqt_enumvalue.h"



using namespace CppQt::Edit;






EnumValue::EnumValue(CppQt::EnumValue* block, QWidget* parent):
   Base(block,parent),
   _block(block)
{}






QLayout* EnumValue::layout()
{
   saveSettings("cppqt.edit.enumvalue.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addValue(ret);
   return ret;
}






void EnumValue::apply()
{
   Base::apply();
   if ( _valueEdit->hasAcceptableInput() ) _block->setValue(_valueEdit->text().toInt());
   else _block->clearValue();
}






void EnumValue::addValue(QFormLayout* layout)
{
   _valueEdit = new QLineEdit;
   _valueEdit->setValidator(new QIntValidator);
   if ( _block->hasValue() ) _valueEdit->setText(QString::number(_block->value()));
   layout->addRow(new QLabel(tr("Value:")),_valueEdit);
}
