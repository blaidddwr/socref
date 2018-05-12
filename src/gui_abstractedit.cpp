#include "gui_abstractedit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <exception.h>
#include "gui_blockview.h"
#include "common.h"



using namespace Gui;
//






/*!
 * Constructs a new abstract edit object with an optional parent. The new abstract 
 * edit is not initialized and not ready to be used as a dialog until that method 
 * is called. 
 *
 * @param parent Optional parent for this new abstract edit dialog. 
 */
AbstractEdit::AbstractEdit(QWidget* parent):
   PersistentDialog(nullptr,parent)
{}






/*!
 * Constructs and initializes the GUI for this abstract edit dialog, making it 
 * ready to be used as a GUI dialog window. 
 *
 * @param view  
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's block view to the one given, setup this object's buttons 
 *    saving the returned layout to _buttons_, and then create a new vertical box 
 *    layout _main_. 
 *
 * 2. Add the implementation's layout to _main_ by calling the layout interface, 
 *    then add the layout _buttons_, and then set this dialog's layout to _main_. 
 */
void AbstractEdit::initialize(BlockView* view)
{
   // 1
   _view = view;
   QLayout* buttons {setupButtons()};
   QVBoxLayout* main {new QVBoxLayout};

   // 2
   main->addLayout(layout());
   main->addLayout(buttons);
   setLayout(main);
}






/*!
 * Sets the state of the apply and OK buttons of this dialog to being disabled or 
 * enabled. 
 *
 * @param disabled True to disable apply and OK buttons or false to enable them. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set the disabled state of this object's apply and OK buttons based off the 
 *    given state. 
 */
void AbstractEdit::setDisabled(bool disabled)
{
   // 1
   _ok->setDisabled(disabled);
   _apply->setDisabled(disabled);
}






/*!
 * Called when this object's OK button is clicked. This tries to apply changes to 
 * the implementation's block and if successful closes this dialog. 
 */
void AbstractEdit::okClicked()
{
   if ( tryApply() ) accept();
}






/*!
 * Called when this object's apply button is clicked. This tries to apply changes 
 * to the implementation's block and if successful updates the block's view. 
 */
void AbstractEdit::applyClicked()
{
   if ( tryApply() ) _view->updateView();
}






/*!
 * Called when this object's cancel button is called. This simply closes the dialog 
 * without applying anything. 
 */
void AbstractEdit::cancelClicked()
{
   emit reject();
}






/*!
 * Calls this object's apply interface, returning true if it was successful or 
 * false if it caught an exception. 
 *
 * @return True is the implementation applied changes successfully or false if an 
 *         exception was thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call this object's apply interface and return true on success. 
 *
 * 2. If an exception is caught then report the error to the user and return false 
 *    on failure. 
 */
bool AbstractEdit::tryApply()
{
   // 1
   try
   {
      apply();
      return true;
   }

   // 2
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save changes to this block."),e);
      return false;
   }
}






/*!
 * Constructs the three buttons this abstract edit object provides to its 
 * implementation, returning the horizontal layout containing the setup buttons. 
 *
 * @return Horizontal layout containing this object's three buttons. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new push button, setting it to this object's OK button and 
 *    connecting its clicked signal. 
 *
 * 2. Create a new push button, setting it to this object's apply button and 
 *    connecting its clicked signal. 
 *
 * 3. Create a new push button _cancel_, connecting its clicked signal. 
 *
 * 4. Create a new horizontal layout _ret_, adding the OK button, apply button, 
 *    stretch, and _cancel_ button in that order. 
 *
 * 5. Return _ret_. 
 */
QLayout* AbstractEdit::setupButtons()
{
   // 1
   _ok = new QPushButton(tr("&Ok"));
   connect(_ok,&QPushButton::clicked,this,&AbstractEdit::okClicked);

   // 2
   _apply = new QPushButton(tr("&Apply"));
   connect(_apply,&QPushButton::clicked,this,&AbstractEdit::applyClicked);

   // 3
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,this,&AbstractEdit::cancelClicked);

   // 4
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_ok);
   ret->addWidget(_apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // 5
   return ret;
}
