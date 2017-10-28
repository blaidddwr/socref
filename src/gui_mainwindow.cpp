#include <memory>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

#include "gui_mainwindow.h"
#include "exception.h"
#include "abstractprojectfactory.h"
#include "project.h"
#include "gui_projectdialog.h"
#include "abstractblock.h"
#include "gui_blockview.h"



using namespace std;
using namespace Gui;






//@@
MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   // create all GUI elements
   createView();
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

   // assign new project pointer and update everything
   _project = o_project;
   updateTitle();
   updateActions();

   // check to see if is not nullptr
   if ( o_project )
   {
      // set parent, update model, and update window modified state
      o_project->setParent(this);
      _view->setModel(o_project->getModel());
      setWindowModified(o_project->isModified());

      // connect all new project's signals
      connect(o_project,&Project::nameChanged,this,&MainWindow::projectNameChanged);
      connect(o_project,&Project::modified,this,&MainWindow::projectModified);
      connect(o_project,&Project::saved,this,&MainWindow::projectSaved);
      connect(o_project,&Project::changed,this,&MainWindow::projectFileChanged);
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
         e.show(tr("An error occured while attempting to open the project.")
                ,Exception::Icon::Warning);
      }
   }
}






//@@
void MainWindow::propertiesTriggered()
{
   // create project settings dialog and modally execute
   ProjectDialog settings(_project,this);
   settings.exec();
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
void MainWindow::projectFileChanged()
{
   // create message box informing user of file change and asking what to do
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::AcceptRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);

   // modally execute message box and get answer
   int answer = notice.exec();

   // check if answer is to reload file
   if ( answer == QMessageBox::AcceptRole )
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
         e.show(tr("An error occured while attempting to reload the project.")
                ,Exception::Icon::Warning);
      }
   }
}






//@@
void MainWindow::selectionChanged()
{
   // update actions and populate add menu
   updateActions();
   _view->populateAddMenu(_addMenu);
}






void MainWindow::closeEvent(QCloseEvent* event)
{
   // if is ok to continue accept close event
   if ( isOkToContinue() )
   {
      event->accept();
   }

   // else not ok to continue reject close event
   else
   {
      event->ignore();
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

   // create exit action
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&MainWindow::close);

   // create project settings action
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);

   // create remove action
   _removeAction = new QAction(tr("&Remove"),this);
   _removeAction->setStatusTip(tr("Remove a block."));
   connect(_removeAction,&QAction::triggered,_view,&BlockView::removeTriggered);

   // create edit action
   _editAction = new QAction(tr("&Edit"),this);
   _editAction->setStatusTip(tr("Edit currently selected block."));
   connect(_removeAction,&QAction::triggered,_view,&BlockView::editTriggered);

   // create cut action
   _cutAction = new QAction(tr("&Cut"),this);
   _cutAction->setStatusTip(tr("Cut currently selected block."));
   connect(_cutAction,&QAction::triggered,_view,&BlockView::cutTriggered);

   // create copy action
   _copyAction = new QAction(tr("C&opy"),this);
   _copyAction->setStatusTip(tr("Copy currently selected block."));
   connect(_copyAction,&QAction::triggered,_view,&BlockView::copyTriggered);

   // create paste action
   _pasteAction = new QAction(tr("&Paste"),this);
   _pasteAction->setStatusTip(tr("Paste block into selected block as child."));
   connect(_pasteAction,&QAction::triggered,_view,&BlockView::pasteTriggered);

   // create move up action
   _moveUpAction = new QAction(tr("Move &Up"),this);
   _moveUpAction->setStatusTip(tr("Move currently selected block up by one."));
   connect(_moveUpAction,&QAction::triggered,_view,&BlockView::moveUpTriggered);

   // create move down action
   _moveDownAction = new QAction(tr("Move &Down"),this);
   _moveDownAction->setStatusTip(tr("Move currently selected block down by one."));
   connect(_moveDownAction,&QAction::triggered,_view,&BlockView::moveDownTriggered);
}






//@@
void MainWindow::createMenus()
{
   // make file menu
   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

   // make new submenu for each project type
   QMenu* newMenu = fileMenu->addMenu(tr("&New"));
   for (auto i = _newActions.constBegin(); i != _newActions.constEnd() ;++i)
   {
      newMenu->addAction(*i);
   }

   // finish making file menu
   fileMenu->addAction(_openAction);
   fileMenu->addAction(_saveAction);
   fileMenu->addAction(_saveAsAction);
   fileMenu->addAction(_closeAction);
   fileMenu->addAction(_propertiesAction);
   fileMenu->addAction(_exitAction);

   // make edit file menu
   QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
   _addMenu = editMenu->addMenu(tr("&Add"));
   editMenu->addAction(_removeAction);
   editMenu->addAction(_editAction);
   editMenu->addAction(_cutAction);
   editMenu->addAction(_copyAction);
   editMenu->addAction(_pasteAction);
   editMenu->addAction(_moveUpAction);
   editMenu->addAction(_moveDownAction);
}






//@@
void MainWindow::createView()
{
   // create view, set to central widget, and connect selection signal
   _view = new BlockView(this);
   setCentralWidget(_view);
   connect(_view,&BlockView::selectionChanged,this,&MainWindow::selectionChanged);
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

   // determine if view has selection
   bool hasSelection {_view->hasSelection()};

   // enable or disable remaining actions
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _propertiesAction->setDisabled(!_project);
   _removeAction->setDisabled(!hasSelection);
   _editAction->setDisabled(!hasSelection);
   _copyAction->setDisabled(!hasSelection);
   _cutAction->setDisabled(!hasSelection);
   _moveUpAction->setDisabled(!hasSelection);
   _moveDownAction->setDisabled(!hasSelection);
   _pasteAction->setDisabled(!_view->canPaste());

   // enable or disable add menu
   _addMenu->setDisabled(!_project);
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
   confirm.setIcon(QMessageBox::Question);
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
      e.show(tr("An error occured while attempting to save the project."),Exception::Icon::Warning);
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
      e.show(tr("An error occured while attempting to save the project."),Exception::Icon::Warning);
      return false;
   }

   // no exception occured return success
   return true;
}
