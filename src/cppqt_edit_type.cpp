#include "cppqt_edit_type.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include "cppqt_type.h"



using namespace CppQt::Edit;






Type::Type(CppQt::Type* block, QWidget* parent):
   ::Gui::AbstractEdit(parent),
   _block(block)
{}






QLayout* Type::layout()
{
   saveSettings("cppqt.edit.type.geometry");
   QFormLayout* ret {new QFormLayout};
   addType(ret);
   return ret;
}






void Type::apply()
{
   _block->setType(_typeEdit->text());
}






void Type::textChanged(const QString& text)
{
   Q_UNUSED(text)
   setDisabled(!CppQt::Type::isValidTypeString(text));
}






void Type::addType(QFormLayout* layout)
{
   _typeEdit = new QLineEdit;
   connect(_typeEdit,&QLineEdit::textChanged,this,&Type::textChanged);
   _typeEdit->setText(_block->cppType());
   layout->addRow(new QLabel(tr("Type:")),_typeEdit);
}
