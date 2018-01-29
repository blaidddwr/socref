#include <QGridLayout>
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
   QGridLayout* layout {new QGridLayout};
   layout->addWidget(createLabel(tr("Name:")),0,0);
   layout->addWidget(_nameEdit,0,1);
   layout->addWidget(createLabel(tr("Description:")),1,0);
   layout->addWidget(_descriptionEdit,1,1);
   _layout = layout;
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
   _nameEdit->setText(_block->Base::name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*"),this));
}






void Base::createDescriptionEdit()
{
   _descriptionEdit = new QPlainTextEdit;
   _descriptionEdit->setPlainText(_block->description());
}






QLabel* Base::createLabel(const QString& name)
{
   QLabel* ret {new QLabel(name)};
   ret->setAlignment(Qt::AlignTop|Qt::AlignRight);
   return ret;
}
