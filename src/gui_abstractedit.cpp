#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "gui_abstractedit.h"



using namespace Gui;






//@@
void AbstractEdit::initialize()
{
   // create form and buttons
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addLayout(createButtons());
   setLayout(layout);
}






//@@
QLayout* AbstractEdit::createButtons()
{
   // create buttons
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* apply {new QPushButton(tr("Apply"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};

   // connect clicked signals for all buttons
   connect(ok,&QPushButton::clicked,this,&AbstractEdit::okClicked);
   connect(apply,&QPushButton::clicked,this,&AbstractEdit::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&AbstractEdit::cancelClicked);

   // create layout and add all buttons
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // return created layout
   return ret;
}
