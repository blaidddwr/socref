#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

#include "gui_mainwindow.h"
#include "abstractprojectfactory.h"
#include "gui_projectdialog.h"



using namespace std;
using namespace Gui;






MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   static QIcon icon;
   if ( icon.isNull() )
   {
      icon = QIcon(":/icons/main.svg");
   }
   setWindowIcon(icon);
   createView();
   createActions();
   createMenus();
   updateTitle();
   updateActions();
}






MainWindow& MainWindow::setProject(unique_ptr<Project>&& project)
{
   if ( _project )
   {
      delete _project;
   }
   _project = project.release();
   updateTitle();
   updateActions();
   if ( _project )
   {
      _view->setModel(_project->model());
      setWindowModified(_project->isModified());
      connect(_project,&Project::nameChanged,this,&MainWindow::projectNameChanged);
      connect(_project,&Project::modified,this,&MainWindow::projectModified);
      connect(_project,&Project::saved,this,&MainWindow::projectSaved);
      connect(_project,&Project::changed,this,&MainWindow::projectFileChanged);
      _project->setParent(this);
   }
   return *this;
}






void MainWindow::newTriggered()
{
   QAction* from {qobject_cast<QAction*>(sender())};
   unique_ptr<Project> project {new Project(from->data().toInt())};
   project->setName(tr("Untitled Project"));
   if ( !_project )
   {
      setProject(std::move(project));
   }
   else
   {
      MainWindow* window {new MainWindow};
      window->setProject(std::move(project));
      window->show();
   }
}






void MainWindow::openTriggered()
{
   QFileDialog fileDialog(nullptr,tr("Open Project"),"",tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
   if ( fileDialog.exec() )
   {
      QStringList files = fileDialog.selectedFiles();
      const QString path = files.constFirst();
      try
      {
         unique_ptr<Project> project {new Project(path)};
         if ( !_project )
         {
            setProject(std::move(project));
         }
         else
         {
            MainWindow* window = new MainWindow;
            window->setProject(std::move(project));
            window->show();
         }
      }
      catch (Exception::Base e)
      {
         e.show(tr("An error occured while attempting to open the project."),Exception::Icon::Warning);
      }
   }
}






void MainWindow::propertiesTriggered()
{
   ProjectDialog settings(_project,this);
   settings.exec();
}






void MainWindow::closeTriggered()
{
   if ( isOkToContinue() )
   {
      setProject(nullptr);
   }
}






void MainWindow::projectFileChanged()
{
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::AcceptRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);
   int answer = notice.exec();
   if ( answer == QMessageBox::AcceptRole )
   {
      try
      {
         unique_ptr<Project> project {new Project(_project->path())};
         setProject(std::move(project));
      }
      catch (Exception::Base e)
      {
         e.show(tr("An error occured while attempting to reload the project."),Exception::Icon::Warning);
      }
   }
}






void MainWindow::closeEvent(QCloseEvent* event)
{
   if ( isOkToContinue() )
   {
      event->accept();
   }
   else
   {
      event->ignore();
   }
}






void MainWindow::createActions()
{
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _newActions.append(new QAction(factory.name(i),this));
      _newActions.back()->setData(i);
      connect(_newActions.back(),&QAction::triggered,this,&MainWindow::newTriggered);
   }
   createOpenAction();
   createSaveAction();
   createSaveAsAction();
   createPropertiesAction();
   createCloseAction();
   createExitAction();
}






void MainWindow::createOpenAction()
{
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);
}






void MainWindow::createSaveAction()
{
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);
}






void MainWindow::createSaveAsAction()
{
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setShortcut(QKeySequence::SaveAs);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);
}






void MainWindow::createPropertiesAction()
{
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);
}






void MainWindow::createCloseAction()
{
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);
}






void MainWindow::createExitAction()
{
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&QWidget::close);
}






void MainWindow::createMenus()
{
   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
   QMenu* newMenu = fileMenu->addMenu(tr("&New"));
   for (const auto& i : _newActions)
   {
      newMenu->addAction(i);
   }
   fileMenu->addAction(_openAction);
   fileMenu->addAction(_saveAction);
   fileMenu->addAction(_saveAsAction);
   fileMenu->addAction(_propertiesAction);
   fileMenu->addAction(_closeAction);
   fileMenu->addAction(_exitAction);
   menuBar()->addMenu(_view->contextMenu());
}






void MainWindow::createView()
{
   _view = new BlockView(this);
   setCentralWidget(_view);
}






void MainWindow::updateTitle()
{
   if ( _project )
   {
      setWindowTitle(tr("%1[*] (%2) - Socrates' Reference").arg(_project->name()).arg(AbstractProjectFactory::instance().name(_project->type())));
   }
   else
   {
      setWindowTitle(tr("Socrates' Reference"));
   }
}






void MainWindow::updateActions()
{
   if ( _project )
   {
      _saveAction->setDisabled(_project->isNew());
   }
   else
   {
      _saveAction->setDisabled(true);
   }
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _propertiesAction->setDisabled(!_project);
}






bool MainWindow::isOkToContinue()
{
   if ( !_project || !_project->isModified() )
   {
      return true;
   }
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Unsaved Project Changes"));
   confirm.setText(tr("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!"));
   confirm.setIcon(QMessageBox::Question);
   confirm.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard);
   int answer = confirm.exec();
   switch (answer)
   {
   case QMessageBox::Cancel:
      return false;
   case QMessageBox::Save:
      if ( _project->isNew() )
      {
         return saveAs();
      }
      else
      {
         return save();
      }
      break;
   }
   return true;
}






bool MainWindow::saveAs()
{
   if ( !_project )
   {
      return false;
   }
   QFileDialog fileDialog(nullptr,tr("Save Project"),"",tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);
   if ( !fileDialog.exec() )
   {
      return false;
   }
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();
   try
   {
      _project->saveAs(path);
   }
   catch (Exception::Base e)
   {
      e.show(tr("An error occured while attempting to save the project."),Exception::Icon::Warning);
      return false;
   }
   updateActions();
   return true;
}






bool MainWindow::save()
{
   if ( !_project )
   {
      return false;
   }
   try
   {
      _project->save();
   }
   catch (Exception::Base e)
   {
      e.show(tr("An error occured while attempting to save the project."),Exception::Icon::Warning);
      return false;
   }
   return true;
}
