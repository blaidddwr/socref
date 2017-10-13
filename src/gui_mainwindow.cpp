#include <QAction>
#include <QMenuBar>

#include "gui_mainwindow.h"
#include "abstractprojectfactory.h"
#include "project.h"



using namespace Gui;






//@@
MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   createActions();
   createMenus();
   updateTitle();
}






//@@
void MainWindow::setProject(Project* o_project)
{
   if ( _project )
   {
      delete _project;
   }
   _project = o_project;
   o_project->setParent(this);
   connect(o_project,&Project::modified,this,&MainWindow::projectModified);
   connect(o_project,&Project::saved,this,&MainWindow::projectSaved);
   connect(o_project,&Project::fileChanged,this,&MainWindow::projectFileChanged);
   updateTitle();
}






//@@
void MainWindow::newTriggered()
{
}






//@@
void MainWindow::openTriggered()
{
}






//@@
void MainWindow::saveTriggered()
{
}






//@@
void MainWindow::saveAsTriggered()
{
}






//@@
void MainWindow::closeTriggered()
{
}






//@@
void MainWindow::projectSettingsTriggered()
{
}






//@@
void MainWindow::projectModified()
{
}






//@@
void MainWindow::projectSaved()
{
}






//@@
void MainWindow::projectFileChanged()
{
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
   _saveAction->setDisabled(true);
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);

   // create save as action
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   _saveAsAction->setDisabled(true);
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);

   // create close action
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   _closeAction->setDisabled(true);
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);

   // create project settings action
   _projectSettingsAction = new QAction(tr("&Project"),this);
   _projectSettingsAction->setStatusTip(tr("Edit basic settings of project"));
   _projectSettingsAction->setDisabled(true);
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






void MainWindow::updateTitle()
{
   if ( _project )
   {
      setWindowTitle(tr("%1[*] (%2) - Socrates' Reference").arg(_project->getName()).arg("type"));
   }
   else
   {
      setWindowTitle(tr("Socrates' Reference"));
   }
}
