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






ProjectDialog::ProjectDialog(Project* project, QWidget* parent):
   QDialog(parent),
   _project(project)
{
   if ( !_project )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot give nullptr as argument for project settings constructor."));
      throw e;
   }
   createDialog();
   setWindowTitle(tr("Project Properties"));
}






void ProjectDialog::okClicked()
{
   applyClicked();
   accept();
}






void ProjectDialog::applyClicked()
{
   _project->setName(_nameEdit->text());
   _project->setScanDirectory(_scanDirectoryEdit->text());
   _project->setScanFilters(_filtersEdit->text());
}






void ProjectDialog::browseClicked()
{
   QFileDialog dialog(nullptr,tr("Scan Directory Selection"));
   dialog.setFileMode(QFileDialog::Directory);
   if ( dialog.exec() )
   {
      QStringList directories = dialog.selectedFiles();
      QFileInfo info(directories.constFirst());
      if ( info.exists() && info.isDir() )
      {
         _scanDirectoryEdit->setText(info.filePath());
      }
   }
}






void ProjectDialog::createDialog()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createForm());
   layout->addStretch();
   layout->addLayout(createButtons());
   setLayout(layout);
}






QFormLayout* ProjectDialog::createForm()
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






QHBoxLayout* ProjectDialog::createButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(createButton(tr("&Ok"),&ProjectDialog::okClicked));
   ret->addWidget(createButton(tr("&Apply"),&ProjectDialog::applyClicked));
   ret->addStretch();
   ret->addWidget(createButton(tr("&Cancel"),&ProjectDialog::reject));
   return ret;
}






QPushButton*ProjectDialog::createButton(const QString& text, void (ProjectDialog::*pointer)())
{
   QPushButton* ret {new QPushButton(text)};
   connect(ret,&QPushButton::clicked,this,pointer);
   return ret;
}
