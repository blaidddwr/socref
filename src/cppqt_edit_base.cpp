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
}






QLayout* Base::layout()
{
   QFormLayout* ret {new QFormLayout};
   addFields(ret);
   return ret;
}






void Base::addFields(QFormLayout* layout)
{
   createNameEdit();
   createDescriptionEdit();
   layout->addRow(new QLabel(tr("Name:")),_nameEdit);
   layout->addRow(new QLabel(tr("Description:")),_descriptionEdit);
}






QLabel* Base::createTitle(const QString& title)
{
   QLabel* ret {new QLabel(QString("<h3>").append(title).append("</h3>"))};
   ret->setStyleSheet("margin-top:16px;");
   return ret;
}






void Base::applyClicked()
{
   _block->setName(_nameEdit->text());
   _block->setDescription(_descriptionEdit->toPlainText());
}






void Base::createNameEdit()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_block->Base::name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*")));
}






void Base::createDescriptionEdit()
{
   _descriptionEdit = new QPlainTextEdit;
   _descriptionEdit->setPlainText(_block->description());
}
