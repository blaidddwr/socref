#include <memory>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

#include "gui_mainwindow.h"
#include "exception.h"
#include "abstractprojectfactory.h"
#include "project.h"
#include "gui_projectsettingsdialog.h"



using namespace std;
using namespace Gui;






//@@
MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   // create all GUI elements
   createActions();
   createMenus();

   // update window title and actions
   updateTitle();
   updateActions();
}






//@@
void MainWindow::setProject(Project* o_project)
{
   // if window already had project then delete it
   if ( _project )
   {
      delete _project;
   }

   // assign new project pointer and update title and actions
   _project = o_project;
   updateTitle();
   updateActions();

   // check to see if is not nullptr
   if ( o_project )
   {
      // set new project's parent and update window modified state
      o_project->setParent(this);
      setWindowModified(o_project->isModified());

      // connect all new project's signals
      connect(o_project,&Project::nameChanged,this,&MainWindow::projectNameChanged);
      connect(o_project,&Project::modified,this,&MainWindow::projectModified);
      connect(o_project,&Project::saved,this,&MainWindow::projectSaved);
      connect(o_project,&Project::fileChanged,this,&MainWindow::projectFileChanged);
   }
}






//@@
void MainWindow::newTriggered()
{
   // get type to use for new project and make it
   QAction* from {qobject_cast<QAction*>(sender())};
   unique_ptr<Project> project {new Project(from->data().toInt())};
   project->setName(tr("Untitled Project"));

   // if there is no project set then set it to new one
   if ( !_project )
   {
      setProject(project.release());
   }

   // else project already set
   else
   {
      // make new window and set its project to new one
      MainWindow* window {new MainWindow};
      window->setProject(project.release());
      window->show();
   }
}






//@@
void MainWindow::openTriggered()
{
   // setup file dialog to get file location
   QFileDialog fileDialog(nullptr,tr("Open Project"),""
                          ,tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

   // make sure user chose acceptable file
   if ( fileDialog.exec() )
   {
      // get first path user selected
      QStringList files = fileDialog.selectedFiles();
      const QString path = files.constFirst();
      try
      {
         // open project with first file user selected
         unique_ptr<Project> project {new Project(path)};

         // set this window's project to opened project if it has none
         if ( !_project )
         {
            setProject(project.release());
         }

         // else project already set
         else
         {
            // create a new window and set its project to opened project
            MainWindow* window = new MainWindow;
            window->setProject(project.release());
            window->show();
         }
      }

      // inform user of anything that went wrong
      catch (Exception::Base e)
      {
         showException(e);
      }
   }
}






//@@
void MainWindow::closeTriggered()
{
   // if is ok to continue close project
   if ( isOkToContinue() )
   {
      setProject(nullptr);
   }
}






//@@
void MainWindow::projectSettingsTriggered()
{
   ProjectSettingsDialog settings(_project,this);
   settings.exec();
}






//@@
void MainWindow::projectFileChanged()
{
   // create message box informing user of file change and asking what to do
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::ResetRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);

   // modally execute message box and get answer
   int answer = notice.exec();

   // check if answer is to reload file
   if ( answer == QMessageBox::ResetRole )
   {
      try
      {
         // open new project object form same file and set as project
         unique_ptr<Project> project {new Project(_project->getPath())};
         setProject(project.release());
      }

      // inform user of anything that went wrong
      catch (Exception::Base e)
      {
         showException(e);
      }
   }
}






//@@
void MainWindow::createActions()
{
   // create new actions for each project type
   AbstractProjectFactory& factory = AbstractProjectFactory::getInstance();
   for (int i = 0; i < factory.getSize() ;++i)
   {
      _newActions.append(new QAction(factory.getName(i),this));
      _newActions.back()->setData(i);
      connect(_newActions.back(),&QAction::triggered,this,&MainWindow::newTriggered);
   }

   // create open action
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);

   // create save action
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);

   // create save as action
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);

   // create close action
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);

   // create project settings action
   _projectSettingsAction = new QAction(tr("&Project"),this);
   _projectSettingsAction->setStatusTip(tr("Edit basic settings of project"));
   connect(_projectSettingsAction,&QAction::triggered,this,&MainWindow::projectSettingsTriggered);
}






//@@
void MainWindow::createMenus()
{
   // make file menu
   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

   // make new submenu for each project type
   QMenu* newMenu = fileMenu->addMenu(tr("&New"));
   AbstractProjectFactory& factory = AbstractProjectFactory::getInstance();
   for (int i = 0; i < factory.getSize() ;++i)
   {
      newMenu->addAction(_newActions.at(i));
   }

   // finish making file menu
   fileMenu->addAction(_openAction);
   fileMenu->addAction(_saveAction);
   fileMenu->addAction(_saveAsAction);
   fileMenu->addAction(_closeAction);

   // make settings menu
   QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
   settingsMenu->addAction(_projectSettingsAction);
}






//@@
void MainWindow::updateTitle()
{
   // if there is a project update the title accordingly
   if ( _project )
   {
      setWindowTitle(tr("%1[*] (%2) - Socrates' Reference").arg(_project->getName())
                     .arg(AbstractProjectFactory::getInstance().getName(_project->getType())));
   }

   // else there is no project so make basic title
   else
   {
      setWindowTitle(tr("Socrates' Reference"));
   }
}






//@@
void MainWindow::updateActions()
{
   // if there is a project set save action accordingly
   if ( _project )
   {
      _saveAction->setDisabled(_project->isNew());
   }

   // else there is no project so disable save action
   else
   {
      _saveAction->setDisabled(true);
   }

   // enable or disable remaining actions
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _projectSettingsAction->setDisabled(!_project);
}






//@@
bool MainWindow::isOkToContinue()
{
   // if window has no project or its project is not modified then it is ok
   if ( !_project || !_project->isModified() )
   {
      return true;
   }

   // create message box informing the user closing the current project will result in loss of data
   // and query to save, cancel, or discard.
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Unsaved Project Changes"));
   confirm.setText(tr("The currently open project has unsaved changes. Closing the project will"
                      " cause all unsaved changes to be lost!"));
   confirm.setIcon(QMessageBox::Warning);
   confirm.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard);

   // modally execute message box and determine answer
   int answer = confirm.exec();
   switch (answer)
   {
   case QMessageBox::Cancel:
      // user choose to cancel so it is not ok
      return false;
   case QMessageBox::Save:
      // if project is new call save as
      if ( _project->isNew() )
      {
         return saveAs();
      }

      // else project is not new so call save
      else
      {
         return save();
      }
      break;
   }

   // user chose discard so it is ok
   return true;
}






//@@
bool MainWindow::saveAs()
{
   // make sure window has project
   if ( !_project )
   {
      return false;
   }

   // get path to project file
   QFileDialog fileDialog(nullptr,tr("Save Project"),""
                          ,tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);

   // make sure user choose acceptable file
   if ( !fileDialog.exec() )
   {
      return false;
   }

   // get first path in list of user selected paths
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();

   // try to save project to given path
   try
   {
      _project->saveAs(path);
   }

   // inform user of anything that went wrong
   catch (Exception::Base e)
   {
      showException(e);
      return false;
   }

   // no exception occured update actions and return success
   updateActions();
   return true;
}






//@@
bool MainWindow::save()
{
   // make sure this window has a project
   if ( !_project )
   {
      return false;
   }

   // save project
   try
   {
      _project->save();
   }

   // inform user of anything that went wrong
   catch (Exception::Base e)
   {
      showException(e);
      return false;
   }

   // no exception occured return success
   return true;
}






//@@
void MainWindow::showException(const Exception::Base& e)
{
   // create simple modal message box informing user of warning exception that occured
   QMessageBox warning;
   warning.setWindowTitle(e.getTitle());
   warning.setText(e.getDetails());
   warning.setIcon(QMessageBox::Critical);
   warning.exec();
}
