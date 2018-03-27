#include "cppqt_edit_base.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include <exception.h>
#include "cppqt_base.h"
#include "gui_textedit.h"




using namespace Gui;
using namespace CppQt::Edit;






Base::Base(CppQt::Base* block, QWidget* parent):
   AbstractEdit(parent),
   _block(block)
{}






void Base::addFields(QFormLayout* layout)
{
   addName(layout);
   addDescription(layout);
}






void Base::addName(QFormLayout* layout)
{
   setupName();
   layout->addRow(new QLabel(tr("Name:")),_nameEdit);
}






void Base::addDescription(QFormLayout* layout)
{
   setupDescription();
   layout->addRow(new QLabel(tr("Description:")),_descriptionEdit);
}






void Base::apply()
{
   if ( _nameEdit ) _block->setName(_nameEdit->text());
   if ( _descriptionEdit ) _block->setDescription(_descriptionEdit->toPlainText());
}






void Base::setupName()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_block->Base::name());
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*")));
}






void Base::setupDescription()
{
   _descriptionEdit = new Gui::TextEdit;
   _descriptionEdit->setPlainText(_block->description());
}
