#include "gui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include "gui_projectdialog.h"
#include "gui_scandialog.h"
#include "gui_blockview.h"
#include "abstractprojectfactory.h"
#include "project.h"
#include "abstractblock.h"
#include "application.h"
#include "common.h"
#include "scanthread.h"



using namespace std;
using namespace Gui;
const char* MainWindow::_geometryKey {"gui.mainwindow.geometry"};
const char* MainWindow::_stateKey {"gui.mainwindow.state"};






MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   setupGui();
   restoreSettings();
   updateTitle();
   updateActions();
}






void MainWindow::setProject(unique_ptr<Project>&& project)
{
   delete _project;
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
      connect(_project,&Project::saveFileChanged,this,&MainWindow::projectFileChanged);
      _project->setParent(this);
   }
}






void MainWindow::newTriggered()
{
   QAction* from {qobject_cast<QAction*>(sender())};
   unique_ptr<Project> project {new Project(from->data().toInt())};
   project->setName(tr("Untitled Project"));
   if ( !_project ) setProject(std::move(project));
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
         if ( !_project ) setProject(std::move(project));
         else
         {
            MainWindow* window = new MainWindow;
            window->setProject(std::move(project));
            window->show();
         }
      }
      catch (Exception::Base e)
      {
         showException(tr("An error occured while attempting to open the project."),e);
      }
   }
}






void MainWindow::saveTriggered()
{
   save();
}






void MainWindow::saveAsTriggered()
{
   saveAs();
}






void MainWindow::propertiesTriggered()
{
   ProjectDialog settings(_project,this);
   settings.exec();
}






void MainWindow::scanTriggered()
{
   unique_ptr<ScanThread> scanner {_project->makeScanner()};
   ScanDialog dialog(scanner.get());
   dialog.exec();
}






void MainWindow::closeTriggered()
{
   if ( isOkToContinue() ) setProject(nullptr);
}






void MainWindow::settingTriggered()
{
   QAction* from {qobject_cast<QAction*>(sender())};
   unique_ptr<QDialog> settings {AbstractProjectFactory::instance().makeSettings(from->data().toInt())};
   if ( !settings )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Project factory's make settings returned a null pointer."));
      throw e;
   }
   settings->exec();
}






void MainWindow::projectNameChanged()
{
   updateTitle();
}






void MainWindow::projectModified()
{
   setWindowModified(true);
}






void MainWindow::projectSaved()
{
   setWindowModified(false);
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
         showException(tr("An error occured while attempting to reload the project."),e);
      }
   }
}






void MainWindow::closeEvent(QCloseEvent* event)
{
   if ( isOkToContinue() )
   {
      event->accept();
      saveSettings();
      deleteLater();
   }
   else event->ignore();
}






void MainWindow::updateTitle()
{
   if ( _project ) setWindowTitle(tr("%1[*] (%2) - Socrates' Reference").arg(_project->name()).arg(AbstractProjectFactory::instance().name(_project->type())));
   else setWindowTitle(tr("Socrates' Reference"));
}






void MainWindow::updateActions()
{
   if ( _project ) _saveAction->setDisabled(_project->isNew());
   else _saveAction->setDisabled(true);
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _propertiesAction->setDisabled(!_project);
   _scanAction->setDisabled(!_project);
}






bool MainWindow::isOkToContinue()
{
   if ( !_project || !_project->isModified() ) return true;
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
      if ( _project->isNew() ) return saveAs();
      else return save();
      break;
   }
   return true;
}






bool MainWindow::saveAs()
{
   if ( !_project ) return false;
   QFileDialog fileDialog(nullptr,tr("Save Project"),"",tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptSave);
   if ( !fileDialog.exec() ) return false;
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();
   try
   {
      _project->saveAs(path);
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save the project."),e);
      return false;
   }
   updateActions();
   return true;
}






bool MainWindow::save()
{
   if ( !_project ) return false;
   try
   {
      _project->save();
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save the project."),e);
      return false;
   }
   return true;
}






void MainWindow::restoreSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
   restoreState(settings.value(_stateKey).toByteArray());
}






void MainWindow::saveSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
   settings.setValue(_stateKey,saveState());
}






void MainWindow::setupGui()
{
   static QIcon icon;
   if ( icon.isNull() ) icon = QIcon(":/icons/main.svg");
   setWindowIcon(icon);
   setupView();
   setupActions();
   setupMenus();
}






void MainWindow::setupActions()
{
   setupNewActions();
   setupSettingActions();
   setupOpenAction();
   setupSaveAction();
   setupSaveAsAction();
   setupPropertiesAction();
   setupScanAction();
   setupCloseAction();
   setupExitAction();
}






void MainWindow::setupNewActions()
{
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _newActions.append(new QAction(factory.name(i),this));
      _newActions.back()->setData(i);
      connect(_newActions.back(),&QAction::triggered,this,&MainWindow::newTriggered);
   }
}






void MainWindow::setupOpenAction()
{
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);
}






void MainWindow::setupSaveAction()
{
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);
}






void MainWindow::setupSaveAsAction()
{
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setShortcut(QKeySequence::SaveAs);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);
}






void MainWindow::setupPropertiesAction()
{
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);
}






void MainWindow::setupScanAction()
{
   _scanAction = new QAction(tr("Scan"),this);
   _scanAction->setStatusTip(tr("Scan and parse source files to add documentation."));
   _scanAction->setShortcut(Qt::CTRL + Qt::Key_B);
   connect(_scanAction,&QAction::triggered,this,&MainWindow::scanTriggered);
}






void MainWindow::setupCloseAction()
{
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);
}






void MainWindow::setupExitAction()
{
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&QWidget::close);
}






void MainWindow::setupSettingActions()
{
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _settingActions.append(new QAction(factory.name(i),this));
      _newActions.back()->setData(i);
      connect(_settingActions.back(),&QAction::triggered,this,&MainWindow::settingTriggered);
   }
}






void MainWindow::setupMenus()
{
   setupFileMenu();
   menuBar()->addMenu(_view->contextMenu());
   setupSettingsMenu();
}






void MainWindow::setupFileMenu()
{
   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
   QMenu* newMenu = fileMenu->addMenu(tr("&New"));
   for (auto action : qAsConst(_newActions)) newMenu->addAction(action);
   fileMenu->addAction(_openAction);
   fileMenu->addAction(_saveAction);
   fileMenu->addAction(_saveAsAction);
   fileMenu->addAction(_closeAction);
   fileMenu->addSeparator();
   fileMenu->addAction(_propertiesAction);
   fileMenu->addAction(_scanAction);
   fileMenu->addSeparator();
   fileMenu->addAction(_exitAction);
}






void MainWindow::setupSettingsMenu()
{
   QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
   for (auto action : qAsConst(_settingActions)) settingsMenu->addAction(action);
}






void MainWindow::setupView()
{
   _view = new BlockView(this);
   setCentralWidget(_view);
}
