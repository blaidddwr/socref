#include "gui_abstractedit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <exception.h>
#include "gui_blockview.h"
#include "common.h"



using namespace Gui;






AbstractEdit::AbstractEdit(QWidget* parent):
   PersistentDialog(nullptr,parent)
{}






AbstractEdit* AbstractEdit::initialize(BlockView* view)
{
   _view = view;
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
   if ( tryApply() ) accept();
}






void AbstractEdit::applyClicked()
{
   if ( tryApply() ) _view->updateView();
}






void AbstractEdit::cancelClicked()
{
   emit reject();
}






bool AbstractEdit::tryApply()
{
   try
   {
      apply();
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save changes to this block."),e);
      return false;
   }
   return true;
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
