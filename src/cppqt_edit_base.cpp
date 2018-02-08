#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include "cppqt_edit_base.h"
#include "exception.h"
#include "cppqt_base.h"
#include "gui_textedit.h"




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
   setupNameEdit();
   setupDescriptionEdit();
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






void Base::setupNameEdit()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_block->Base::name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*")));
}






void Base::setupDescriptionEdit()
{
   _descriptionEdit = new Gui::TextEdit;
   _descriptionEdit->setPlainText(_block->description());
}
