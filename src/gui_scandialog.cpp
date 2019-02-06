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
   // Make sure the given scanner pointer is not null. 
   Q_CHECK_PTR(scanner);

   // Create the GUI for this dialog and set its title. 
   setupGui();
   setWindowTitle("Scanning Files");

   // Connect the given scanner signals to this new dialog. 
   connect(_scanner,&ScanThread::progressChanged,this,&ScanDialog::progressChanged);
   connect(_scanner,&ScanThread::finished,this,&ScanDialog::scanFinished);
}






/*!
 * Implements _QDialog_ interface. This implementation starts execution of the scan 
 * thread of this dialog and then passes execution to the dialog. 
 *
 * @return See Qt docs. 
 */
int ScanDialog::exec()
{
   // Start the scanner thread and then pass execution to the dialog. 
   _scanner->start();
   return QDialog::exec();
}






/*!
 * Implements _QWidget_ interface. This implementation requests its scan thread to 
 * finish and ignores the close event so the scan thread can exit cleanly. 
 *
 * @param event See Qt docs. 
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
 * This closes this dialog on accept. 
 */
void ScanDialog::scanFinished()
{
   // Close this dialog with accepted. 
   done(QDialog::Accepted);
}






/*!
 * Constructs and initializes the GUI for this new dialog. 
 */
void ScanDialog::setupGui()
{
   // Create and initialize the progress bar for this new dialog. 
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(_scanner->size());

   // Create a new vertical layout, adding the progress bar and then the bottom 
   // layout. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_bar);
   layout->addLayout(setupBottom());

   // Set the layout of this new dialog. 
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
   // Create the push button for this dialog, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,this,&ScanDialog::cancelClicked);

   // Create a new horizontal layout, adding a stretch then the cancel button and 
   // then another stretch. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(cancel);
   ret->addStretch();

   // Return the bottom layout. 
   return ret;
}
