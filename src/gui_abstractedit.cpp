#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "gui_abstractedit.h"



using namespace Gui;






AbstractEdit::AbstractEdit(QWidget* parent):
   QDialog(parent)
{}






AbstractEdit* AbstractEdit::initialize()
{
   setupOkButton();
   setupApplyButton();
   QVBoxLayout* main {new QVBoxLayout};
   main->addLayout(layout());
   main->addLayout(setupButtons());
   setLayout(main);
   return this;
}






void AbstractEdit::setDisabled(bool disabled)
{
   _ok->setDisabled(disabled);
   _apply->setDisabled(disabled);
}






void AbstractEdit::okClicked()
{
   if ( apply() ) accept();
}






void AbstractEdit::applyClicked()
{
   apply();
}






void AbstractEdit::cancelClicked()
{
   emit reject();
}






QLayout* AbstractEdit::setupButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_ok);
   ret->addWidget(_apply);
   ret->addStretch();
   ret->addWidget(setupCancelButton());
   return ret;
}






void AbstractEdit::setupOkButton()
{
   _ok = new QPushButton(tr("&Ok"));
   connect(_ok,&QPushButton::clicked,this,&AbstractEdit::okClicked);
}






void AbstractEdit::setupApplyButton()
{
   _apply = new QPushButton(tr("&Apply"));
   connect(_apply,&QPushButton::clicked,this,&AbstractEdit::applyClicked);
}






QPushButton*AbstractEdit::setupCancelButton()
{
   QPushButton* ret {new QPushButton(tr("&Cancel"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::cancelClicked);
   return ret;
}
