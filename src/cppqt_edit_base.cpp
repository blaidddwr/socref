#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QRegExpValidator>
#include "cppqt_edit_base.h"
#include "exception.h"
#include "cppqt_base.h"




using namespace Gui;
using namespace CppQt::Edit;






Base::Base(AbstractBlock* block, QWidget* parent):
   AbstractEdit(parent),
   _block(qobject_cast<CppQt::Base*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   createNameEdit();
   createDescriptionEdit();
   QFormLayout* form {new QFormLayout};
   form->addRow(new QLabel(tr("Name:")),_nameEdit);
   form->addRow(new QLabel(tr("Description:")),_descriptionEdit);
   _layout = form;
}






QLayout* Base::layout()
{
   return _layout;
}






void Base::applyClicked()
{
   _block->setName(_nameEdit->text());
   _block->setDescription(_descriptionEdit->toPlainText());
}






void Base::createNameEdit()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_block->name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*"),this));
}






void Base::createDescriptionEdit()
{
   _descriptionEdit = new QPlainTextEdit;
   _descriptionEdit->setPlainText(_block->description());
}
