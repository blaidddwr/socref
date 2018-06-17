#include "gui_scandialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include "scanthread.h"
#include "application.h"
#include "common.h"



using namespace Gui;
//






/*!
 * Constructs a new scan dialog with the given scan thread and optional parent. 
 * This does not start the scanning process but only prepares the dialog to start 
 * execution. If the given scan thread pointer is null then an exception is thrown. 
 *
 * @param scanner Pointer to the scan thread this new dialog will use for 
 *                execution. This dialog does not take ownership of the scan 
 *                thread. The pointer cannot be null. 
 *
 * @param parent Optional parent for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given scan thread pointer is null then throw an excpetion, else setup 
 *    this GUI and window title of this new dialog. 
 *
 * 2. Connect the progress changed and finished signals to this new dialog. 
 */
ScanDialog::ScanDialog(ScanThread* scanner, QWidget* parent):
   PersistentDialog("gui.scandialog.geometry",parent),
   _scanner(scanner)
{
   // 1
   if ( !scanner )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give null pointer as scanner argument for scan dialog."));
      throw e;
   }
   setupGui();
   setWindowTitle("Scanning Files");

   // 2
   connect(_scanner,&ScanThread::progressChanged,this,&ScanDialog::progressChanged);
   connect(_scanner,&ScanThread::finished,this,&ScanDialog::scanFinished);
}






/*!
 * Starts execution of the scan thread of this dialog and then beings execution of 
 * this dialog in modal mode. 
 *
 * @return Always returns Qt dialog accept. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Start the separate thread for scanning in this object's scan thread object 
 *    and then begin execution of this dialog in modal mode returning the exit 
 *    state. 
 */
int ScanDialog::exec()
{
   // 1
   _scanner->start();
   return QDialog::exec();
}






/*!
 * Implements the Qt interface for handling a close event on this dialog. This 
 * implementation requests its scan thread to finish and ignores the close event so 
 * the scan thread exits cleanly. 
 *
 * @param event Pointer to the Qt close event handler. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Request the scan thread for this dialog interrupts itself and ignore the 
 *    given close event. 
 */
void ScanDialog::closeEvent(QCloseEvent* event)
{
   // 1
   _scanner->requestInterruption();
   event->ignore();
}






/*!
 * Called when the cancel button for this dialog is clicked, requesting the scan 
 * thread for this dialog to interrupt itself and exit. 
 */
void ScanDialog::cancelClicked()
{
   _scanner->requestInterruption();
}






/*!
 * Called when the scan thread's progress for this dialog has changed. This updates 
 * the progress bar for this dialog. 
 *
 * @param index The current file index that is being parsed by the scan thread of 
 *              this dialog. 
 */
void ScanDialog::progressChanged(int index)
{
   _bar->setValue(index);
}






/*!
 * Called when the scan thread for this dialog has finished scanning all files. 
 * This closes this dialog on accept. If the scan thread has an exception from its 
 * separate thread then it is shown to the user. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's scan thread contained an exception then show it to the user. 
 *
 * 2. Exit the modal execution of this dialog with accept. 
 */
void ScanDialog::scanFinished()
{
   // 1
   if ( _scanner->hasException() )
   {
      showException(tr("An error occured while scanning and parsing files."),_scanner->exception());
   }

   // 2
   done(QDialog::Accepted);
}






/*!
 * Constructs and initializes the GUI for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize the progress bar for this new dialog, using the total 
 *    number of files this object's scan thread will parse as the progress bar's 
 *    maximum value. 
 *
 * 2. Create a new vertical layout _layout_, adding this object's progress bar then 
 *    the bottom GUI. 
 *
 * 3. Set the layout of this dialog to _layout_. 
 */
void ScanDialog::setupGui()
{
   // 1
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(_scanner->size());

   // 2
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_bar);
   layout->addLayout(setupBottom());

   // 3
   setLayout(layout);
}






/*!
 * Constructs and initializes the bottom GUI for this new dialog, returning its 
 * layout. 
 *
 * @return Pointer to layout of the bottom GUI for this new element. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a push button for this object's cancel button, connecting its signal. 
 *
 * 2. Create a new horizontal layout _ret_, adding a stretch then this object's 
 *    cancel button then a another stretch. 
 *
 * 3. Return _ret_. 
 */
QLayout* ScanDialog::setupBottom()
{
   // 1
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,this,&ScanDialog::cancelClicked);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(cancel);
   ret->addStretch();

   // 3
   return ret;
}
