#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "gui_abstractedit.h"



using namespace Gui;






AbstractEdit::AbstractEdit(QWidget* parent):
   QWidget(parent)
{}






AbstractEdit* AbstractEdit::initialize()
{
   QVBoxLayout* main {new QVBoxLayout};
   main->addLayout(layout());
   main->addLayout(setupButtons());
   setLayout(main);
   return this;
}






QLayout* AbstractEdit::setupButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(setupOkButton());
   ret->addWidget(setupApplyButton());
   ret->addStretch();
   ret->addWidget(setupCancelButton());
   return ret;
}






QPushButton*AbstractEdit::setupOkButton()
{
   QPushButton* ret {new QPushButton(tr("&Ok"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::okClicked);
   return ret;
}






QPushButton*AbstractEdit::setupApplyButton()
{
   QPushButton* ret {new QPushButton(tr("&Apply"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::applyClicked);
   return ret;
}






QPushButton*AbstractEdit::setupCancelButton()
{
   QPushButton* ret {new QPushButton(tr("&Cancel"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::cancelClicked);
   return ret;
}
