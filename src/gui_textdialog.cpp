#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "gui_textdialog.h"
#include "gui_textedit.h"



using namespace Gui;






TextDialog::TextDialog(QWidget* parent):
   QDialog(parent)
{
   setLayout(setupLayout());
}






QString TextDialog::text() const
{
   return _edit->toPlainText();
}






void TextDialog::setText(const QString& text)
{
   _edit->setPlainText(text);
}






QLayout* TextDialog::setupLayout()
{
   _edit = new TextEdit;
   _edit->setDialogPopupEnabled(false);
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_edit);
   ret->addLayout(setupButtons());
   return ret;
}






QLayout* TextDialog::setupButtons()
{
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&QDialog::accept);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(cancel);
   return ret;
}
