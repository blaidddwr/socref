#include "cppqt_edit_parent.h"
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <exception.h>
#include "cppqt_type.h"
#include "cppqt_parent.h"



using namespace CppQt::Edit;






Parent::Parent(CppQt::Parent* block, QWidget* parent):
   ::Gui::AbstractEdit(parent),
   _block(block)
{}






QLayout* Parent::layout()
{
   saveSettings("cppqt.edit.parent.geometry");
   QFormLayout* ret {new QFormLayout};
   addAccess(ret);
   addClass(ret);
   return ret;
}






void Parent::apply()
{
   if ( !Type::isValidTypeString(_classEdit->text()) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given template string is not a valid C++ template."));
      throw e;
   }
   _block->setAccess(_accessBox->currentText());
   _block->setClassName(_classEdit->text());
}






void Parent::classChanged(const QString& text)
{
   Q_UNUSED(text)
   setDisabled(!Type::isValidTypeString(_classEdit->text()));
}






void Parent::addAccess(QFormLayout* layout)
{
   setupAccess();
   layout->addRow(new QLabel(tr("Access:")),_accessBox);
}






void Parent::addClass(QFormLayout* layout)
{
   setupClass();
   layout->addRow(new QLabel(tr("Class:")),_classEdit);
}






void Parent::setupAccess()
{
   _accessBox = new QComboBox;
   for (auto item : _block->accessNames()) _accessBox->addItem(item);
   _accessBox->setCurrentIndex(_accessBox->findText(_block->accessName()));
}






void Parent::setupClass()
{
   _classEdit = new QLineEdit;
   connect(_classEdit,&QLineEdit::textChanged,this,&Parent::classChanged);
   _classEdit->setText(_block->className());
}
