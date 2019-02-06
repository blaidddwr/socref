#include "gui_projectdialog.h"
#include <QLineEdit>
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QFileDialog>
#include <QSettings>
#include <QAction>
#include "project.h"
#include "dictionarymodel.h"
#include "application.h"



using namespace Gui;
//






/*!
 * Constructs a new project dialog with the given project and optional parent. If 
 * the given project pointer is null then an exception is thrown. 
 *
 * @param project Pointer to the project that this new dialog presents to the user 
 *                for viewing and editing. 
 *
 * @param parent Optional parent for this new dialog. 
 */
ProjectDialog::ProjectDialog(Project* project, QWidget* parent):
   PersistentDialog("gui.projectdialog.geometry",parent),
   _project(project)
{
   // Make sure the given project pointer is not null. 
   Q_CHECK_PTR(project);

   // Create the GUI of this new dialog and set its window title. 
   setupGui();
   setWindowTitle(tr("Project Properties"));
}






/*!
 * Called when this object's OK button is clicked. This applies its project 
 * property settings and closes this dialog window. 
 */
void ProjectDialog::okClicked()
{
   // Apply project property settings and close the dialog with accept. 
   applyClicked();
   done(QDialog::Accepted);
}






/*!
 * Called when this object's apply button is clicked. This applies its property 
 * settings to its underlying project. 
 */
void ProjectDialog::applyClicked()
{
   // Apply all properties this object's project. 
   _project->setName(_nameEdit->text());
   _project->setScanDirectory(_scanDirectoryEdit->text());
   _project->setScanFilters(_filtersEdit->text());
}






/*!
 * Called when this object's browse button is clicked. This is used with the scan 
 * directory property of a project. This opens a file dialog to find a new 
 * directory. 
 */
void ProjectDialog::browseClicked()
{
   // Create a new file dialog to query the user for a new scan directory location. 
   QFileDialog dialog(nullptr,tr("Scan Directory Selection"));
   dialog.setFileMode(QFileDialog::Directory);

   // Execute the file dialog and make sure it returned accepted. 
   if ( !dialog.exec() ) return;

   // Get the first directory path the user selected from the file dialog. 
   QStringList directories = dialog.selectedFiles();
   QFileInfo info(directories.constFirst());

   // If the path exists and it is a directory then set the text of this object's 
   // scan directory edit widget to it's value. 
   if ( info.exists() && info.isDir() ) _scanDirectoryEdit->setText(info.filePath());
}






/*!
 * Constructs and initializes all GUI elements for this new dialog. 
 */
void ProjectDialog::setupGui()
{
   // Create a new vertical layout, adding the form layout then a stretch and then 
   // the button layout. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupForm());
   layout->addStretch();
   layout->addLayout(setupButtons());

   // Set the layout of this new dialog. 
   setLayout(layout);
}






/*!
 * Constructs and initializes the form GUI for this new dialog, returning its 
 * layout. 
 *
 * @return Pointer to layout for the form GUI of this new dialog. 
 */
QLayout* ProjectDialog::setupForm()
{
   // Create and initialize the name edit widget for this dialog. 
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_project->name());

   // Create and initialize the filters edit widget for this dialog. 
   _filtersEdit = new QLineEdit;
   _filtersEdit->setText(_project->scanFilters());

   // Create a new form layout, adding the name edit widget then scan directory 
   // layout and then the filters edit widget. 
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("Project Name:")),_nameEdit);
   ret->addRow(new QLabel(tr("Scan Directory:")),setupDirectory());
   ret->addRow(new QLabel(tr("Scan File Filters:")),_filtersEdit);

   // Return the form layout. 
   return ret;
}






/*!
 * Constructs and initializes the scan directory form element for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to layout of the scan directory form element for this new 
 *         dialog. 
 */
QLayout* ProjectDialog::setupDirectory()
{
   // Create and initialize the scan directory edit widget for this new dialog. 
   _scanDirectoryEdit = new QLineEdit;
   _scanDirectoryEdit->setReadOnly(true);
   _scanDirectoryEdit->setText(_project->scanDirectory());

   // Create the scan directory browse button for this new dialog, connecting its 
   // clicked signal. 
   QPushButton* button {new QPushButton(tr("Browse"))};
   connect(button,&QPushButton::clicked,this,&ProjectDialog::browseClicked);

   // Create a new horizontal layout, adding the scan directory edit widget and then 
   // the scan directory browse button.  
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_scanDirectoryEdit);
   ret->addWidget(button);

   // Return the scan directory layout. 
   return ret;
}






/*!
 * Constructs and initializes the buttons for this new dialog, return the layout 
 * for the new buttons. 
 *
 * @return Pointer to the layout of buttons for this new dialog. 
 */
QLayout* ProjectDialog::setupButtons()
{
   // Create the OK button for this dialog, connecting its clicked signal. 
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   connect(ok,&QPushButton::clicked,this,&ProjectDialog::okClicked);

   // Create the apply button for this dialog, connecting its clicked signal. 
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   connect(apply,&QPushButton::clicked,this,&ProjectDialog::applyClicked);

   // Create the cancel button for this dialog, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout, adding the OK button then the apply button then 
   // a stretch and then the cancel button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return the buttons layout. 
   return ret;
}
