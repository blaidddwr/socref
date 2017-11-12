#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "gui_abstractedit.h"



using namespace Gui;






void AbstractEdit::initialize()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());
   setLayout(layout);
}






QLayout* AbstractEdit::createButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(createOkButton());
   ret->addWidget(createApplyButton());
   ret->addStretch();
   ret->addWidget(createCancelButton());
   return ret;
}






QPushButton*AbstractEdit::createOkButton()
{
   QPushButton* ret {new QPushButton(tr("&Ok"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::okClicked);
   return ret;
}






QPushButton*AbstractEdit::createApplyButton()
{
   QPushButton* ret {new QPushButton(tr("&Apply"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::applyClicked);
   return ret;
}






QPushButton*AbstractEdit::createCancelButton()
{
   QPushButton* ret {new QPushButton(tr("&Cancel"))};
   connect(ret,&QPushButton::clicked,this,&AbstractEdit::cancelClicked);
   return ret;
}
