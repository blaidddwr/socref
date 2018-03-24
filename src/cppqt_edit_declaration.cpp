#include "cppqt_edit_declaration.h"
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "cppqt_declaration.h"



using namespace CppQt::Edit;






Declaration::Declaration(CppQt::Declaration* block, QWidget* parent):
   ::Gui::AbstractEdit(parent),
   _block(block)
{
   saveSettings("cppqt.edit.declaration.geometry");
}






QLayout* Declaration::layout()
{
   QFormLayout* ret {new QFormLayout};
   addType(ret);
   addField(ret);
   return ret;
}






bool Declaration::apply()
{
   _block->setDeclareType(_typeBox->currentText());
   _block->setField(_fieldEdit->text());
   return true;
}






void Declaration::addType(QFormLayout* layout)
{
   _typeBox = new QComboBox;
   for (auto text: _block->declareTypes()) _typeBox->addItem(text);
   _typeBox->setCurrentIndex(_typeBox->findText(_block->declareTypeName()));
   layout->addRow(new QLabel(tr("Type:")),_typeBox);
}






void Declaration::addField(QFormLayout* layout)
{
   _fieldEdit = new QLineEdit;
   _fieldEdit->setText(_block->field());
   layout->addRow(new QLabel(tr("Field:")),_fieldEdit);
}
