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
 *
 * @param project  
 *
 * @param parent  
 */
ProjectDialog::ProjectDialog(Project* project, QWidget* parent):
   PersistentDialog("gui.projectdialog.geometry",parent),
   _project(project)
{
   if ( !_project )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give nullptr as argument for project settings constructor."));
      throw e;
   }
   setupGui();
   setWindowTitle(tr("Project Properties"));
}






/*!
 */
void ProjectDialog::okClicked()
{
   applyClicked();
   accept();
}






/*!
 */
void ProjectDialog::applyClicked()
{
   _project->setName(_nameEdit->text());
   _project->setScanDirectory(_scanDirectoryEdit->text());
   _project->setScanFilters(_filtersEdit->text());
}






/*!
 */
void ProjectDialog::browseClicked()
{
   QFileDialog dialog(nullptr,tr("Scan Directory Selection"));
   dialog.setFileMode(QFileDialog::Directory);
   if ( dialog.exec() )
   {
      QStringList directories = dialog.selectedFiles();
      QFileInfo info(directories.constFirst());
      if ( info.exists() && info.isDir() ) _scanDirectoryEdit->setText(info.filePath());
   }
}






/*!
 */
void ProjectDialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupForm());
   layout->addStretch();
   layout->addLayout(setupButtons());
   setLayout(layout);
}






/*!
 */
QLayout* ProjectDialog::setupForm()
{
   QFormLayout* ret {new QFormLayout};
   QHBoxLayout* directory {new QHBoxLayout};
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_project->name());
   _scanDirectoryEdit = new QLineEdit;
   _scanDirectoryEdit->setReadOnly(true);
   _scanDirectoryEdit->setText(_project->scanDirectory());
   QPushButton* button {new QPushButton(tr("Browse"))};
   connect(button,&QPushButton::clicked,this,&ProjectDialog::browseClicked);
   directory->addWidget(_scanDirectoryEdit);
   directory->addWidget(button);
   _filtersEdit = new QLineEdit;
   _filtersEdit->setText(_project->scanFilters());
   ret->addRow(new QLabel(tr("Project Name:")),_nameEdit);
   ret->addRow(new QLabel(tr("Scan Directory:")),directory);
   ret->addRow(new QLabel(tr("Scan File Filters:")),_filtersEdit);
   return ret;
}






/*!
 */
QLayout* ProjectDialog::setupButtons()
{
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(ok,&QPushButton::clicked,this,&ProjectDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&ProjectDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&ProjectDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);
   return ret;
}
