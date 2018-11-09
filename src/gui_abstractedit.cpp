#include "gui_abstractedit.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <socutil/sut_exceptions.h>
#include "gui_mainwindow.h"



using namespace Sut;
using namespace Gui;
//






/*!
 * Constructs a new abstract edit object with an optional parent. The new abstract 
 * edit is not initialized and not ready to be used as a dialog until the 
 * initialize method is called. 
 *
 * @param parent Optional parent for this new abstract edit dialog. 
 */
AbstractEdit::AbstractEdit(QWidget* parent):
   PersistentDialog(nullptr,parent)
{}






/*!
 * Constructs and initializes the GUI for this abstract edit dialog, making it 
 * ready to be used as a GUI dialog window. 
 */
void AbstractEdit::initialize()
{
   // Setup this object's buttons. This must be done before the layout interface is 
   // called because its buttons may be accessed in that interface. 
   QLayout* buttons {setupButtons()};

   // Create a vertical layout, adding the abstract layout and then the buttons. 
   QVBoxLayout* main {new QVBoxLayout};
   main->addLayout(layout());
   main->addLayout(buttons);

   // Set the main layout of this dialog. 
   setLayout(main);
}






/*!
 * Sets the state of the apply and OK buttons of this dialog to being disabled or 
 * enabled. 
 *
 * @param disabled True to disable apply and OK buttons or false to enable them. 
 */
void AbstractEdit::setDisabled(bool disabled)
{
   // Set the disabled state of this object's apply and OK buttons based off the 
   // given state. 
   _ok->setDisabled(disabled);
   _apply->setDisabled(disabled);
}






/*!
 * Called when this object's OK button is clicked. This tries to apply changes to 
 * the implementation's block and if successful closes this dialog. 
 */
void AbstractEdit::okClicked()
{
   // Try to apply changes and if it works close this dialog with accept. 
   if ( tryApply() ) done(QDialog::Accepted);
}






/*!
 * Called when this object's apply button is clicked. This tries to apply changes 
 * to the implementation's block and if successful updates the block's view. 
 */
void AbstractEdit::applyClicked()
{
   tryApply();
}






/*!
 * Calls this object's apply interface, returning true if it was successful or 
 * false if it caught an exception. 
 *
 * @return True is the implementation applied changes successfully or false if an 
 *         exception was thrown. 
 */
bool AbstractEdit::tryApply()
{
   try
   {
      // Call this object's apply interface and return true on success. 
      apply();
      return true;
   }

   // Catch any exception thrown from the apply interface. 
   catch (Exception::InvalidArgument e)
   {
      // Report the exception to the user and return false on failure. 
      MainWindow::showException(e,tr("Cannot save changes to this block."));
      return false;
   }
}






/*!
 * Constructs the three buttons this abstract edit object provides to its 
 * implementation, returning the horizontal layout containing the setup buttons. 
 *
 * @return Horizontal layout containing this object's three buttons. 
 */
QLayout* AbstractEdit::setupButtons()
{
   // Create the OK button for this dialog, connecting its clicked signal. 
   _ok = new QPushButton(tr("&Ok"));
   connect(_ok,&QPushButton::clicked,this,&AbstractEdit::okClicked);

   // Create the apply button for this dialog, connecting its clicked signal. 
   _apply = new QPushButton(tr("&Apply"));
   connect(_apply,&QPushButton::clicked,this,&AbstractEdit::applyClicked);

   // Create the cancel button for this dialog, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout, adding the OK button then the apply button then 
   // a stretch and then the cancel button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_ok);
   ret->addWidget(_apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return the buttons layout. 
   return ret;
}
