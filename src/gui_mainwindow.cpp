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
#include "gui_aboutdialog.h"
#include "abstractprojectfactory.h"
#include "project.h"
#include "abstractblock.h"
#include "application.h"
#include "common.h"
#include "scanthread.h"



using namespace std;
using namespace Gui;
//



/*!
 * The key used to save/restore the geometry of the main window using Qt settings. 
 */
const char* MainWindow::_geometryKey {"gui.mainwindow.geometry"};
/*!
 * The key used to save/restore the state of the main window using Qt settings. 
 */
const char* MainWindow::_stateKey {"gui.mainwindow.state"};






/*!
 *
 * @param text  
 *
 * @param exception  
 */
void MainWindow::showException(const QString& text, const Exception::Base& exception)
{
   QMessageBox info;
   info.setWindowTitle(exception.title());
   info.setText(text);
   info.setInformativeText(exception.details());
   info.setIcon(QMessageBox::Warning);
   info.exec();
}






/*!
 * Constructs a new main window with the optional parent. 
 *
 * @param parent Optional parent for this new main window. 
 */
MainWindow::MainWindow(QWidget* parent):
   QMainWindow(parent)
{
   setupGui();
   restoreSettings();
   updateTitle();
   updateActions();
}






/*!
 * Sets the given project as this main window's new project, deleting any previous 
 * project this window may contain. This window takes ownership of any new project 
 * given to it. A null pointer can be given which means this window will have no 
 * project. 
 *
 * @param project  
 */
void MainWindow::setProject(std::unique_ptr<Project>&& project)
{
   // Delete any previous project this window may contain, setting the new pointer as 
   // this window's project. 
   delete _project;
   _project = project.release();

   // Update this window's title and file menu actions. If the new project given is a 
   // null pointer then return. 
   updateTitle();
   updateActions();
   if ( !_project ) return;

   // Set this window's new project's parent as this window, set this window's block 
   // view with the new project's model, and set this window's modified state based 
   // on the new project's modified state. 
   _project->setParent(this);
   _view->setModel(_project->model());
   setWindowModified(_project->isModified());

   // Connect all project signals to this window's slots. 
   connect(_project,&Project::nameChanged,this,&MainWindow::projectNameChanged);
   connect(_project,&Project::modified,this,&MainWindow::projectModified);
   connect(_project,&Project::saved,this,&MainWindow::projectSaved);
   connect(_project,&Project::saveFileChanged,this,&MainWindow::projectFileChanged);
}






/*!
 * Implements the Qt interface which is called when this window has a close event. 
 * This implementation determines if it is OK to close the window and accept the 
 * event or not and ignore it. 
 *
 * @param event Pointer to the close event that this is processing. 
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
   // If it is OK to close this window then accept the given event else reject it. 
   if ( isOkToClose() )
   {
      saveSettings();
      event->accept();
   }
   else event->ignore();
}






/*!
 * Called when an add action is triggered. This creates a new project of the given 
 * type and sets it to this window if it has none or else a new main window is 
 * created. 
 *
 * @param type The project type used to create a new project. 
 */
void MainWindow::newTriggered(int type)
{
   // Create new project _project_ with the given type and set its name to the 
   // default. 
   unique_ptr<Project> project {new Project(type)};
   project->setName(tr("Untitled Project"));

   // If this window has no project then set it to _project_, else create a new main 
   // window and set the new window's project to _project_. 
   if ( !_project ) setProject(std::move(project));
   else
   {
      MainWindow* window {new MainWindow};
      window->setProject(std::move(project));
      window->show();
   }
}






/*!
 * Called when the open action is triggered. This opens a file dialog for the user 
 * to select a Socrates Reference project and if successful open it. The open 
 * project is set to this window if it has none or a new window otherwise. 
 */
void MainWindow::openTriggered()
{
   // Create a file dialog _file_ and query the user for a project file to open by 
   // modal execution of it. If execution fails then return. 
   QFileDialog fileDialog(nullptr,tr("Open Project"),"",tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
   if ( !fileDialog.exec() ) return;

   // Create a string _path_ setting it to the path of the first file selected from 
   // the file dialog _file_. 
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();

   // Open a new project with the path _path_. If this window has no project that set 
   // it to this new project, else create a new main window with the new project. 
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

   // If any exception is caught while opening the new project report it to the user. 
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to open the project."),e);
   }
}






/*!
 * Called when the save action is triggered, calling this window's save method. 
 */
void MainWindow::saveTriggered()
{
   save();
}






/*!
 * Called when the save as action is triggered, calling this window's save as 
 * method. 
 */
void MainWindow::saveAsTriggered()
{
   saveAs();
}






/*!
 * Called when the properties action is triggered, executing a project properties 
 * dialog if this window contains a project. 
 */
void MainWindow::propertiesTriggered()
{
   if ( _project )
   {
      ProjectDialog settings(_project,this);
      settings.exec();
   }
}






/*!
 * Called when the scan action is triggered. If this window has a project then a 
 * project scan is executed using the scan dialog class. 
 */
void MainWindow::scanTriggered()
{
   if ( _project )
   {
      unique_ptr<ScanThread> scanner {_project->makeScanner()};
      ScanDialog dialog(scanner.get());
      dialog.exec();
   }
}






/*!
 * Called when the close action is triggered, closing any open project of this 
 * window if it is OK to close. 
 */
void MainWindow::closeTriggered()
{
   if ( isOkToClose() ) setProject(nullptr);
}






/*!
 * Called when a setting action is triggered. This provides the settings dialog for 
 * the given project type. 
 *
 * @param type The project type whose settings dialog is displayed. 
 */
void MainWindow::settingTriggered(int type)
{
   // Create a new setting dialog _settings_ using the given project type. If the 
   // returned settings dialog is null then throw an exception. 
   unique_ptr<QDialog> settings {AbstractProjectFactory::instance().makeSettings(type)};
   if ( !settings )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Project factory's make settings returned a null pointer."));
      throw e;
   }

   // Execute the _settings_ dialog in modal mode. 
   settings->exec();
}






/*!
 * Called when the about action is triggered, executing the about dialog. 
 */
void MainWindow::aboutTriggered()
{
   AboutDialog dialog;
   dialog.exec();
}






/*!
 * Called when this window's project's name has changed, updating this window's 
 * title. 
 */
void MainWindow::projectNameChanged()
{
   updateTitle();
}






/*!
 * Called when this window's project has been modified, setting this window state 
 * as modified. 
 */
void MainWindow::projectModified()
{
   setWindowModified(true);
}






/*!
 * Called when this window's project has been saved, setting this window state as 
 * unmodified. 
 */
void MainWindow::projectSaved()
{
   setWindowModified(false);
}






/*!
 * Called when this window's project file has been modified by something besides 
 * the project itself. This executes a dialog informing the user of this fact and 
 * reloads the project if the user chooses to do so. 
 */
void MainWindow::projectFileChanged()
{
   // Create a new message box that informs the user the project file has changed and 
   // ask the user if they want to reload the project or ignore this event. If the 
   // user chooses to ignore it then return. 
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::AcceptRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);
   int answer = notice.exec();
   if ( answer != QMessageBox::AcceptRole ) return;

   // Create a new project with the current project's file and then set the new 
   // project to this window. 
   try
   {
      unique_ptr<Project> project {new Project(_project->path())};
      setProject(std::move(project));
   }

   // If any exception is caught while attempting to reload the project then inform 
   // the user. 
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to reload the project."),e);
   }
}






/*!
 * Updates this window's title, including the project name if it has one. 
 */
void MainWindow::updateTitle()
{
   // If this window has a project then set the window title with the project name 
   // and type along with the application's name, else just set it to the 
   // application's name. 
   if ( _project )
   {
      setWindowTitle(tr("%1[*] (%2) - Socrates' Reference")
                     .arg(_project->name())
                     .arg(AbstractProjectFactory::instance().name(_project->type())));
   }
   else setWindowTitle(tr("Socrates' Reference"));
}






/*!
 * Updates some of this window's actions by enabling or disabling them based off 
 * aspects of this window's project. 
 */
void MainWindow::updateActions()
{
   // If this window has a project and it is now new then enable the save action else 
   // disable it. 
   if ( _project ) _saveAction->setDisabled(_project->isNew());
   else _saveAction->setDisabled(true);

   // If this window has a project then enable the save as, close, properties, and 
   // scan actions. Else if this window has no project disable the same actions. 
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _propertiesAction->setDisabled(!_project);
   _scanAction->setDisabled(!_project);
}






/*!
 * Makes sure this window's project has no unsaved changes and is OK to be closed, 
 * returning true if it is OK. If this window has no project then true is also 
 * returned. If this window's project has unsaved changes then the user is prompted 
 * to save it, cancel, or discard changes. 
 *
 * @return True if it is OK to close this window's current project or this window 
 *         has no project, else false is returned. 
 */
bool MainWindow::isOkToClose()
{
   // If this window has no project or the project is not modified then return true. 
   if ( !_project || !_project->isModified() ) return true;

   // Create a Qt message box _confirm_ that queries the user about what to do with 
   // this window's project's unsaved changes. The options are to save the project, 
   // cancel closing it, or discard the changes. 
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Unsaved Project Changes"));
   confirm.setText(tr("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!"));
   confirm.setIcon(QMessageBox::Question);
   confirm.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard);

   // Executes the _confirm_ dialog in modal mode and get the user's response. If 
   // cancel is chosen then return false, else if save is chosen then attempt to save 
   // the project returning its result, else if discard is chosen then return true. 
   switch (confirm.exec())
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






/*!
 * Saves this window's project to a new file location queried by presenting a file 
 * dialog to the user, returning true on success. If this window has no project, 
 * the file dialog fails execution, or saving the project throws an exception then 
 * false is returned. 
 *
 * @return True if this window's project was successfully saved to a new location 
 *         or false otherwise. 
 */
bool MainWindow::saveAs()
{
   // If this window has no project then return false. 
   if ( !_project ) return false;

   // Create a new file dialog _dialog_, setting its accept mode to saving a file and 
   // executing it in modal mode. If execution fails then return false. 
   QFileDialog dialog(nullptr,tr("Save Project"),"",tr("Socrates' Reference File (*.scr)"));
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   if ( !dialog.exec() ) return false;

   // Get the file path from _dialog_ chosen by the user and attempt to save the 
   // project with that file path. If a Socrates exception is caught then report it 
   // to the user and return false. 
   QStringList files = dialog.selectedFiles();
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

   // Update this window's actions and return true on success. 
   updateActions();
   return true;
}






/*!
 * Saves this window's project to its file, returning true on success or false on 
 * failure. If this window has no project then false is returned. 
 *
 * @return True if the project was successfully saved or false otherwise. 
 */
bool MainWindow::save()
{
   // If this window has no project then return false. 
   if ( !_project ) return false;

   // Attempt to save this project. If any Socrates exception is caught then report 
   // it to the user and return false. 
   try
   {
      _project->save();
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save the project."),e);
      return false;
   }

   // Return true on success. 
   return true;
}






/*!
 * Restores the state and geometry of the main window from Qt settings. 
 */
void MainWindow::restoreSettings()
{
   // Restore this window's state and geometry for the main window using Qt settings 
   // and this class's keys. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
   restoreState(settings.value(_stateKey).toByteArray());
}






/*!
 * Saves the state and geometry of the main window to Qt settings. 
 */
void MainWindow::saveSettings()
{
   // Save this window's state and geometry for the main window using Qt settings and 
   // this class's keys. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
   settings.setValue(_stateKey,saveState());
}






/*!
 * Constructs and initializes all GUI elements for this new main window. 
 */
void MainWindow::setupGui()
{
   // Load this application's logo icon if it hasn't already been loaded and then set 
   // this window's icon to it. 
   static QIcon icon;
   if ( icon.isNull() ) icon = QIcon(":/icons/main.svg");
   setWindowIcon(icon);

   // Create a new block view object for this window, setting it as this window's 
   // central widget. 
   _view = new BlockView(this);
   setCentralWidget(_view);

   // Setup this new window's actions and menus. 
   setupActions();
   setupMenus();
}






/*!
 * Constructs and initializes all actions for this new window. 
 */
void MainWindow::setupActions()
{
   // Create a new action for this window's open action. 
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);

   // Create a new action for this window's save action. 
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);

   // Create a new action for this window's save as action. 
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setShortcut(QKeySequence::SaveAs);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);

   // Create a new action for this window's properties action. 
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);

   // Create a new action for this window's scan action. 
   _scanAction = new QAction(tr("Scan"),this);
   _scanAction->setStatusTip(tr("Scan and parse source files to add documentation."));
   _scanAction->setShortcut(Qt::CTRL + Qt::Key_B);
   connect(_scanAction,&QAction::triggered,this,&MainWindow::scanTriggered);

   // Create a new action for this window's close action. 
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);

   // Create a new action for this window's exit action, connecting its triggered 
   // signal to close this window. 
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&QWidget::close);

   // Create a new action for this window's about action. 
   _aboutAction = new QAction(tr("&About"),this);
   _aboutAction->setStatusTip(tr("Get information about this program."));
   connect(_aboutAction,&QAction::triggered,this,&MainWindow::aboutTriggered);

   // Setup the new project and project settings actions. 
   setupNewActions();
   setupSettingActions();
}






/*!
 * Creates this window's new actions for each project type that exists. 
 */
void MainWindow::setupNewActions()
{
   // Iterate through every project type, creating a new action for each one. Add 
   // each new action to this window's list of new actions and connect its signal to 
   // this window's appropriate slot using a lambda function to save the type. 
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _newActions.append(new QAction(factory.name(i),this));
      connect(_newActions.back(),&QAction::triggered,[this,i]{ newTriggered(i); });
   }
}






/*!
 * Creates this window's setting actions for each project type that exists. 
 */
void MainWindow::setupSettingActions()
{
   // Iterate through every project type, creating a new action for each one. Add 
   // each new action to this window's list of setting actions and connect its signal 
   // to this window's appropriate slot using a lambda function to save the type. 
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _settingActions.append(new QAction(factory.name(i),this));
      connect(_settingActions.back(),&QAction::triggered,[this,i]{ settingTriggered(i); });
   }
}






/*!
 * Constructs and initializes this new window's menus, adding them to its menu bar. 
 */
void MainWindow::setupMenus()
{
   // Setup and add the file menu and then add the context menu from this new 
   // window's block view object. 
   setupFileMenu();
   menuBar()->addMenu(_view->contextMenu());

   // Add the settings menu, adding all setting actions to the menu. 
   QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
   for (auto action : qAsConst(_settingActions)) settingsMenu->addAction(action);

   // Add the help menu, adding the single about action to it. 
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
   helpMenu->addAction(_aboutAction);
}






/*!
 * Construct and initialize this new window's file menu, adding it to this window's 
 * menu bar. 
 */
void MainWindow::setupFileMenu()
{
   // Add a new file menu to this window's menu bar, saving its pointer to _file_. 
   QMenu* file = menuBar()->addMenu(tr("&File"));

   // Add a new sub menu to _file_, populating it with all new actions of this 
   // window. 
   QMenu* newMenu = file->addMenu(tr("&New"));
   for (auto action : qAsConst(_newActions)) newMenu->addAction(action);

   // Add all other actions to _file_, adding separators where appropriate. 
   file->addAction(_openAction);
   file->addAction(_saveAction);
   file->addAction(_saveAsAction);
   file->addAction(_closeAction);
   file->addSeparator();
   file->addAction(_propertiesAction);
   file->addAction(_scanAction);
   file->addSeparator();
   file->addAction(_exitAction);
}
