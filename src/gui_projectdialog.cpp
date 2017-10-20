#include <QLineEdit>
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

#include "gui_projectdialog.h"
#include "exception.h"
#include "project.h"



using namespace Gui;






//@@
ProjectDialog::ProjectDialog(Project* project, QWidget* parent):
   QDialog(parent),
   _project(project)
{
   // make sure a valid project was given
   if ( !_project )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give nullptr as argument for project settings constructor."));
      throw e;
   }

   // create main layout and add form and buttons
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addStretch();
   layout->addLayout(createButtons());

   // set main layout to dialog and set title
   setLayout(layout);
   setWindowTitle(tr("Project Properties"));
}






//@@
void ProjectDialog::okClicked()
{
   // apply changes and accept
   applyClicked();
   accept();
}






//@@
void ProjectDialog::applyClicked()
{
   // set project settings from line edit widgets
   _project->setName(_nameEdit->text());
   _project->setScanDirectory(_scanDirectoryEdit->text());
   _project->setScanFilters(_filtersEdit->text());
}






//@@
void ProjectDialog::browseClicked()
{
   // create file dialog to query for new scan directory
   QFileDialog dialog(nullptr,tr("Scan Directory Selection"));
   dialog.setFileMode(QFileDialog::Directory);

   // modally execute and check for success
   if ( dialog.exec() )
   {
      // get first directory user selected
      QStringList directories = dialog.selectedFiles();
      QFileInfo info(directories.constFirst());

      // if selected path exists and is a directory set it to line edit widget
      if ( info.exists() && info.isDir() )
      {
         _scanDirectoryEdit->setText(info.filePath());
      }
   }
}






//@@
QFormLayout* ProjectDialog::createForm()
{
   // create needed layouts
   QFormLayout* ret {new QFormLayout};
   QHBoxLayout* directory {new QHBoxLayout};

   // create name line edit
   _nameEdit = new QLineEdit;
   _nameEdit->setText(_project->getName());

   // create scan directory line edit
   _scanDirectoryEdit = new QLineEdit;
   _scanDirectoryEdit->setReadOnly(true);
   _scanDirectoryEdit->setText(_project->getScanDirectory());

   // create additional browse button used for scan directory row
   QPushButton* button {new QPushButton(tr("Browse"))};
   connect(button,&QPushButton::clicked,this,&ProjectDialog::browseClicked);
   directory->addWidget(_scanDirectoryEdit);
   directory->addWidget(button);

   // create filters line edit
   _filtersEdit = new QLineEdit;
   _filtersEdit->setText(_project->getScanFilters());

   // add all rows to form
   ret->addRow(new QLabel(tr("Project Name:")),_nameEdit);
   ret->addRow(new QLabel(tr("Scan Directory:")),directory);
   ret->addRow(new QLabel(tr("Scan File Filters:")),_filtersEdit);

   // return form layout
   return ret;
}






//@@
QHBoxLayout* ProjectDialog::createButtons()
{
   // create layout and all buttons
   QHBoxLayout* ret {new QHBoxLayout};
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};

   // add buttons to layout
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // connect button clicked signals to respective slots
   connect(ok,&QPushButton::clicked,this,&ProjectDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&ProjectDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&ProjectDialog::reject);

   // return layout
   return ret;
}
