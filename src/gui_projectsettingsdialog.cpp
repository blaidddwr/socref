#include <QLineEdit>
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>

#include "gui_projectsettingsdialog.h"
#include "exception.h"
#include "project.h"



using namespace Gui;






ProjectSettingsDialog::ProjectSettingsDialog(Project* project, QWidget* parent):
   QDialog(parent),
   _project(project)
{
   if ( !_project )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Cannot give nullptr as argument for project settings constructor."));
      throw e;
   }
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addStretch();
   layout->addLayout(createButtons());
   setLayout(layout);
   setWindowTitle(tr("Project Settings"));
}






void ProjectSettingsDialog::okClicked()
{
}






void ProjectSettingsDialog::applyClicked()
{
}






void ProjectSettingsDialog::browseClicked()
{
}






QFormLayout* ProjectSettingsDialog::createForm()
{
   QFormLayout* ret {new QFormLayout};
   QHBoxLayout* directory {new QHBoxLayout};
   _nameEdit = new QLineEdit;
   _scanDirectoryEdit = new QLineEdit;
   _filtersEdit = new QLineEdit;
   _nameEdit->setText(_project->getName());
   _scanDirectoryEdit->setReadOnly(true);
   _scanDirectoryEdit->setText(_project->getScanDirectory());
   _filtersEdit->setText(_project->getScanFilters());
   QPushButton* button {new QPushButton(tr("Browse"))};
   connect(button,&QPushButton::clicked,this,&ProjectSettingsDialog::browseClicked);
   directory->addWidget(_scanDirectoryEdit);
   directory->addWidget(button);
   ret->addRow(new QLabel(tr("Project Name:")),_nameEdit);
   ret->addRow(new QLabel(tr("Scan Directory:")),directory);
   ret->addRow(new QLabel(tr("Scan File Filters:")),_filtersEdit);
   return ret;
}






QHBoxLayout* ProjectSettingsDialog::createButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);
   connect(ok,&QPushButton::clicked,this,&ProjectSettingsDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&ProjectSettingsDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&ProjectSettingsDialog::reject);
   return ret;
}
