#include "gui_projectdialog.h"
#include <QLineEdit>
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>
#include <exception.h>
#include "project.h"
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given project pointer is null then throw an exception. 
 *
 * 2. Setup the GUI of this new dialog and set its window title. 
 */
ProjectDialog::ProjectDialog(Project* project, QWidget* parent):
   PersistentDialog("gui.projectdialog.geometry",parent),
   _project(project)
{
   // 1
   if ( !_project )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give nullptr as argument for project settings constructor."));
      throw e;
   }

   // 2
   setupGui();
   setWindowTitle(tr("Project Properties"));
}






/*!
 * Called when this object's OK button is clicked. This applies its project 
 * property settings and closes this dialog window. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Apply project property settings and close the dialog with accept. 
 */
void ProjectDialog::okClicked()
{
   // 1
   applyClicked();
   close();
}






/*!
 * Called when this object's apply button is clicked. This applies its property 
 * settings to its underlying project. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Apply all of this object's properties to its project. 
 */
void ProjectDialog::applyClicked()
{
   // 1
   _project->setName(_nameEdit->text());
   _project->setScanDirectory(_scanDirectoryEdit->text());
   _project->setScanFilters(_filtersEdit->text());
}






/*!
 * Called when this object's browse button is clicked. This is used with the scan 
 * directory property of a project. This opens a file dialog to find a new 
 * directory. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new file dialog _dialog_ and execute it in modal mode. If execution 
 *    fails then return. 
 *
 * 2. Get the first directory path the user selected from _dialog_. If the path 
 *    exists and is a directory then set the text of this object's scan directory 
 *    edit widget. 
 */
void ProjectDialog::browseClicked()
{
   // 1
   QFileDialog dialog(nullptr,tr("Scan Directory Selection"));
   dialog.setFileMode(QFileDialog::Directory);
   if ( !dialog.exec() ) return;

   // 2
   QStringList directories = dialog.selectedFiles();
   QFileInfo info(directories.constFirst());
   if ( info.exists() && info.isDir() ) _scanDirectoryEdit->setText(info.filePath());
}






/*!
 * Constructs and initializes all GUI elements for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new vertical layout _layout_, adding this new object's form GUI then 
 *    a stretch then this new object's buttons. 
 *
 * 2. Set the layout of this dialog to _layout_. 
 */
void ProjectDialog::setupGui()
{
   // 1
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupForm());
   layout->addStretch();
   layout->addLayout(setupButtons());

   // 2
   setLayout(layout);
}






/*!
 * Constructs and initializes the form GUI for this new dialog, returning its 
 * layout. 
 *
 * @return Pointer to layout for the form GUI of this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create this new object's name edit widget and set its text to its project's 
 *    name. 
 *
 * 2. Create this new object's filters edit widget and set its text to its 
 *    project's scan filters. 
 *
 * 3. Create a new form layout _ret_, adding this object's name edit widget then 
 *    scan directory layout then filters edit widget. 
 *
 * 4. Return _ret_. 
 */
QLayout* ProjectDialog::setupForm()
{
   // 1
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_project->name());

   // 2
   _filtersEdit = new QLineEdit;
   _filtersEdit->setText(_project->scanFilters());

   // 3
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("Project Name:")),_nameEdit);
   ret->addRow(new QLabel(tr("Scan Directory:")),setupDirectory());
   ret->addRow(new QLabel(tr("Scan File Filters:")),_filtersEdit);

   // 4
   return ret;
}






/*!
 * Constructs and initializes the scan directory form element for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to layout of the scan directory form element for this new 
 *         dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize the scan directory edit widget for this new dialog. 
 *
 * 2. Create a push button _button_ for the browse button of the scan directory 
 *    form element of this new dialog, connecting its clicked signal. 
 *
 * 3. Create a new horizontal layout _ret_, adding this object's scan directory 
 *    edit widget then the browse button _button_.  
 *
 * 4. Return _ret_. 
 */
QLayout* ProjectDialog::setupDirectory()
{
   // 1
   _scanDirectoryEdit = new QLineEdit;
   _scanDirectoryEdit->setReadOnly(true);
   _scanDirectoryEdit->setText(_project->scanDirectory());

   // 2
   QPushButton* button {new QPushButton(tr("Browse"))};
   connect(button,&QPushButton::clicked,this,&ProjectDialog::browseClicked);

   // 3
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_scanDirectoryEdit);
   ret->addWidget(button);

   // 4
   return ret;
}






/*!
 * Constructs and initializes the buttons for this new dialog, return the layout 
 * for the new buttons. 
 *
 * @return Pointer to the layout of buttons for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the OK, apply, and cancel buttons for this new dialog, connecting 
 *    their clicked signals. The cancel signal is connected to this Qt dialog 
 *    reject slot. 
 *
 * 2. Create a new horizontal layout _ret_, adding OK button then apply button then 
 *    stretch then cancel button. 
 *
 * 3. Return _ret_. 
 */
QLayout* ProjectDialog::setupButtons()
{
   // 1
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(ok,&QPushButton::clicked,this,&ProjectDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&ProjectDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&ProjectDialog::close);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // 3
   return ret;
}
