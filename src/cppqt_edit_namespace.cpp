#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRegExpValidator>
#include <QListWidget>
#include <QPushButton>
#include "cppqt_namespace.h"
#include "cppqt_edit_namespace.h"
#include "exception.h"
#include "cppqt_gui_typelistdialog.h"



using namespace std;
using namespace Gui;
using namespace CppQt::Edit;
using namespace CppQt::Gui;






Namespace::Namespace(AbstractBlock* block, QWidget *parent):
   AbstractEdit(parent),
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






void Namespace::cancelClicked()
{
   emit finished();
}






void Namespace::editGlobalTypesClicked()
{
   TypeListDialog dialog(_block->root());
   dialog.setWindowTitle(tr("Edit Global Type List"));
   dialog.exec();
}






void Namespace::editLocalTypesClicked()
{
   TypeListDialog dialog(_block);
   dialog.setWindowTitle(tr("Edit Local Type List"));
   dialog.exec();
}






unique_ptr<QLayout> Namespace::createForm()
{
   createNameEdit();
   createDescriptionEdit();
   unique_ptr<QFormLayout> form {new QFormLayout};
   form->addRow(new QLabel(tr("Name:")),_nameEdit);
   form->addRow(new QLabel(tr("Description:")),_descriptionEdit);
   form->addRow(new QLabel(tr("Types:")),createTypeButtons());
   return form;
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






QLayout* Namespace::createTypeButtons()
{
   QVBoxLayout* ret {new QVBoxLayout};
   QPushButton* global {new QPushButton(tr("Global"))};
   QPushButton* local {new QPushButton(tr("Local"))};
   ret->addWidget(global);
   ret->addWidget(local);
   connect(global,&QPushButton::clicked,this,&Namespace::editGlobalTypesClicked);
   connect(local,&QPushButton::clicked,this,&Namespace::editLocalTypesClicked);
   return ret;
}
