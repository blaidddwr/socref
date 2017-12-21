#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRegExpValidator>

#include "cppqt_edit_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace std;
using namespace CppQt::Edit;






Namespace::Namespace(AbstractBlock* block, QWidget *parent):
   Gui::AbstractEdit(parent),
   _block(qobject_cast<CppQt::Namespace*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






unique_ptr<QLayout> Namespace::createForm()
{
   createNameEdit();
   createDescriptionEdit();
   unique_ptr<QFormLayout> form {new QFormLayout};
   form->addRow(new QLabel(tr("Name:")),_nameEdit);
   form->addRow(new QLabel(tr("Description:")),_descriptionEdit);
   return form;
}






void Namespace::okClicked()
{
   applyClicked();
   finished();
}






void Namespace::applyClicked()
{
   _block->setName(_nameEdit->text());
   _block->setDescription(_descriptionEdit->toPlainText());
}






void Namespace::createNameEdit()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_block->name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*"),this));
}






void Namespace::createDescriptionEdit()
{
   _descriptionEdit = new QPlainTextEdit;
   _descriptionEdit->setPlainText(_block->description());
}
