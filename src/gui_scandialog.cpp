#include "gui_scandialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include "gui_mainwindow.h"
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
 */
ScanDialog::ScanDialog(ScanThread* scanner, QWidget* parent):
   PersistentDialog("gui.scandialog.geometry",parent),
   _scanner(scanner)
{
   // If the given scan thread pointer is null then throw an excpetion, else setup 
   // this GUI and window title of this new dialog. 
   if ( !scanner )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give null pointer as scanner argument for scan dialog."));
      throw e;
   }
   setupGui();
   setWindowTitle("Scanning Files");

   // Connect the progress changed and finished signals to this new dialog. 
   connect(_scanner,&ScanThread::progressChanged,this,&ScanDialog::progressChanged);
   connect(_scanner,&ScanThread::finished,this,&ScanDialog::scanFinished);
}






/*!
 * Starts execution of the scan thread of this dialog and then beings execution of 
 * this dialog in modal mode. 
 *
 * @return Always returns Qt dialog accept. 
 */
int ScanDialog::exec()
{
   // Start the separate thread for scanning in this object's scan thread object and 
   // then begin execution of this dialog in modal mode returning the exit state. 
   _scanner->start();
   return QDialog::exec();
}






/*!
 * Implements the Qt interface for handling a close event on this dialog. This 
 * implementation requests its scan thread to finish and ignores the close event so 
 * the scan thread exits cleanly. 
 *
 * @param event Pointer to the Qt close event handler. 
 */
void ScanDialog::closeEvent(QCloseEvent* event)
{
   // Request the scan thread for this dialog interrupts itself and ignore the given 
   // close event. 
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
 */
void ScanDialog::scanFinished()
{
   // If this object's scan thread contained an exception then show it to the user. 
   if ( _scanner->hasException() )
   {
      MainWindow::showException(tr("An error occured while scanning and parsing files.")
                                ,_scanner->exception());
   }

   // Exit the modal execution of this dialog with accept. 
   done(QDialog::Accepted);
}






/*!
 * Constructs and initializes the GUI for this new dialog. 
 */
void ScanDialog::setupGui()
{
   // Create and initialize the progress bar for this new dialog, using the total 
   // number of files this object's scan thread will parse as the progress bar's 
   // maximum value. 
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(_scanner->size());

   // Create a new vertical layout _layout_, adding this object's progress bar then 
   // the bottom GUI. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_bar);
   layout->addLayout(setupBottom());

   // Set the layout of this dialog to _layout_. 
   setLayout(layout);
}






/*!
 * Constructs and initializes the bottom GUI for this new dialog, returning its 
 * layout. 
 *
 * @return Pointer to layout of the bottom GUI for this new element. 
 */
QLayout* ScanDialog::setupBottom()
{
   // Create a push button for this object's cancel button, connecting its signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,this,&ScanDialog::cancelClicked);

   // Create a new horizontal layout _ret_, adding a stretch then this object's 
   // cancel button then a another stretch. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(cancel);
   ret->addStretch();

   // Return _ret_. 
   return ret;
}
