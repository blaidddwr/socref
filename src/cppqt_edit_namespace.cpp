#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRegExpValidator>

#include "cppqt_edit_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::Edit;






//@@
Namespace::Namespace(AbstractBlock* block, QWidget *parent):
   Gui::AbstractEdit(parent),
   _block(qobject_cast<CppQt::Namespace*>(block))
{
   // make sure block is correct type
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






//@@
QLayout* Namespace::createForm()
{
   // create and initialize name and description
   _nameEdit = new QLineEdit;
   _descriptionEdit = new QPlainTextEdit;
   _nameEdit->setText(_block->getName());
   _descriptionEdit->setPlainText(_block->getDescription());

   // initialize regular expression for name
   QRegExp expression {"[a-zA-Z_]+[a-zA-Z0-9_]*"};
   _nameEdit->setValidator(new QRegExpValidator(expression,this));

   // create form layout and add edit widgets to it
   QFormLayout* form {new QFormLayout};
   form->addRow(new QLabel(tr("Name:")),_nameEdit);
   form->addRow(new QLabel(tr("Description:")),_descriptionEdit);

   // return form
   return form;
}






//@@
void Namespace::okClicked()
{
   // apply any changes and emit finished signal
   applyClicked();
   finished();
}






//@@
void Namespace::applyClicked()
{
   // set name and description of block to values in edit widgets
   _block->setName(_nameEdit->text());
   _block->setDescription(_descriptionEdit->toPlainText());
}
