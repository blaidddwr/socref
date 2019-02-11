#include "gui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include "gui_projectdialog.h"
#include "gui_dictionarydialog.h"
#include "gui_blockview.h"
#include "abstract_block.h"
#include "abstract_projectfactory.h"
#include "exception.h"
#include "project.h"
#include "scanthread.h"
#include "application.h"



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
 * The key used to save/restore the state of the block view contained in the main 
 * window using Qt settings. 
 */
const char* MainWindow::_viewStateKey {"gui.mainwindow.view.state"};






/*!
 * Constructs a new main window with the optional parent. 
 *
 * @param parent Optional parent for this new main window. 
 */
MainWindow::MainWindow(QWidget* parent):
   QMainWindow(parent)
{
   // Create this window's GUI and restore the state and geometry of the main window 
   // and its block view. 
   setupGui();
   restoreSettings();

   // Update this window's title and actions. 
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
void MainWindow::setProject(Soc::Ut::QPtr<Project>&& project)
{
   // Delete any previous project this window may contain and set the new project. 
   delete _project;
   _project = project.release(this);

   // Update this window's title and actions. 
   updateTitle();
   updateActions();

   // Check to see if the new project for this window is a null pointer. 
   if ( !_project ) return;

   // Set the project's parent to this window, updating this this window's block view 
   // and updating its modification state. 
   _view->setModel(_project->model());
   setWindowModified(_project->isModified());

   // Connect all of the new project signals. 
   connect(_project,&Project::nameChanged,this,&MainWindow::projectNameChanged);
   connect(_project,&Project::modified,this,&MainWindow::projectModified);
   connect(_project,&Project::saved,this,&MainWindow::projectSaved);
   connect(_project,&Project::saveFileChanged,this,&MainWindow::projectFileChanged);
}






/*!
 * Implements _QWidget_ interface. This implementation determines if it is OK to 
 * close the window and accept the event or not and ignore it. 
 *
 * @param event See Qt docs. 
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
   // Check to see if it is OK to close this window's project. 
   if ( isOkToClose() )
   {
      // Save the geometry and state of this window and its block view and accept the 
      // close event. 
      saveSettings();
      event->accept();
   }

   // Else it is not OK to close this window to ignore the close event. 
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
   // Create a new project with the given type. 
   Soc::Ut::QPtr<Project> project {new Project(type)};
   project->setName(tr("Untitled Project"));

   // If this window has no project then set it to the new project. 
   if ( !_project ) setProject(std::move(project));

   // Else this window already has a project. 
   else
   {
      // Create a new window, setting its project to the new project, and then show it. 
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
   // Create initialize a file dialog for opening a project file. 
   QFileDialog fileDialog(nullptr
                          ,tr("Open Project")
                          ,QDir::currentPath()
                          ,tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

   // Execute the file dialog and make sure it returned accepted. 
   if ( !fileDialog.exec() ) return;

   // Get the first file path selected by the user from the file dialog. 
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();

   // Set the current working directory to the parent directory of the selected 
   // project. 
   QDir::setCurrent(QFileInfo(path).dir().path());

   try
   {
      // Open a new project with the file path. 
      Soc::Ut::QPtr<Project> project {new Project(path)};

      // If this window has no project that set it to the opened project. 
      if ( !_project ) setProject(std::move(project));

      // Else this window already has a project. 
      else
      {
         // Create a new window, setting its project to the opened project, and then show 
         // it. 
         MainWindow* window = new MainWindow;
         window->setProject(std::move(project));
         window->show();
      }
   }

   // If an exception was thrown then report it to the user. 
   catch (Exception e)
   {
      QMessageBox::warning(this,tr("Failed Opening Project"),e.message());
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
 * Called when the dictionary action is triggered, executing a project dictionary 
 * dialog if this window contains a project. 
 */
void MainWindow::dictionaryTriggered()
{
   // Make sure this window has a project. 
   if ( _project )
   {
      // Create a new dictionary dialog with this window's project and execute it. 
      DictionaryDialog dictionary(_project);
      dictionary.exec();
   }
}






/*!
 * Called when the properties action is triggered, executing a project properties 
 * dialog if this window contains a project. 
 */
void MainWindow::propertiesTriggered()
{
   // Make sure this window has a project. 
   if ( _project )
   {
      // Create a new project dialog with this window's project and execute it. 
      ProjectDialog settings(_project);
      settings.exec();
   }
}






/*!
 * Called when the scan action is triggered. If this window has a project then a 
 * project scan is executed using the scan dialog class. 
 */
void MainWindow::scanTriggered()
{
   // Make sure this window has a project and does not have a currently running scan 
   // thread. 
   if ( _project && !_scanThread )
   {
      // Create a new scan thread and progress bar for this window. 
      _scanThread = new ScanThread(_project->createScannerMap(),_project->scanDirectory(),_project->scanFilters().split(' ',QString::SkipEmptyParts),this);
      QProgressBar* progressBar {new QProgressBar};

      // Connect all required signals for this window's new scan thread, using a queued 
      // connection since all signals will be emitted on its own thread. 
      connect(_scanThread,&ScanThread::progressChanged,progressBar,&QProgressBar::setValue,Qt::QueuedConnection);
      connect(_scanThread,&ScanThread::finished,this,&MainWindow::scanFinished,Qt::QueuedConnection);
      connect(_scanThread,&ScanThread::finished,progressBar,&QProgressBar::deleteLater,Qt::QueuedConnection);
      connect(_scanThread,&ScanThread::exceptionThrown,this,&MainWindow::scanExceptionThrown,Qt::QueuedConnection);

      // Initialize the progress bar and add it to this window's status bar. 
      progressBar->setRange(0,100);
      progressBar->setValue(0);
      statusBar()->addWidget(progressBar);

      // Start the new scan thread. 
      _scanThread->start();
   }
}






/*!
 * Called when an active scan of this window's project's source files have 
 * finished. 
 */
void MainWindow::scanFinished()
{
   // Make sure this window's scan thread pointer is valid. 
   Q_CHECK_PTR(_scanThread);

   // Delete this window's scan thread and reset its pointer to null. 
   delete _scanThread;
   _scanThread = nullptr;
}






/*!
 * Called when an active scan of this window's project's source files fails and 
 * throws an exception within its thread, opening a warning message box informing 
 * the user of what went wrong. 
 *
 * @param e The exception that was thrown within the scan thread that caused it to 
 *          fail. 
 */
void MainWindow::scanExceptionThrown(const Exception& e)
{
   QMessageBox::warning(this,tr("Scanning Failed"),e.message());
}






/*!
 * Called when the close action is triggered, closing any open project of this 
 * window if it is OK to close. 
 */
void MainWindow::closeTriggered()
{
   // If this window's project is OK to close then do so. 
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
   // Make a new setting dialog using the given project type and make sure it is not 
   // null. 
   Soc::Ut::QPtr<QDialog> settings {Abstract::ProjectFactory::instance().makeSettings(type)};
   Q_CHECK_PTR(settings.get());

   // Execute the settings dialog. 
   settings->exec();
}






/*!
 * Called when the about action is triggered, executing the about message box. 
 */
void MainWindow::aboutTriggered()
{
   // Open the internal file containing the HTML of the about dialog and make sure it 
   // worked. 
   QFile file(":/html/about.html");
   bool ok {file.open(QIODevice::ReadOnly)};
   Q_ASSERT(ok);

   // Read in the entire contents of the HTML and replace the application version 
   // string. 
   QString text {file.readAll()};
   text.replace("%SOCREF_VER%",Application::versionString());

   // Execute a message box about dialog. 
   QMessageBox::about(this,tr("About Socrates' Reference"),text);
}






/*!
 * Called when the about qt action is triggered, executing the about qt message 
 * box. 
 */
void MainWindow::aboutQtTriggered()
{
   QMessageBox::aboutQt(this);
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
 * Called when this window's project has been modified, updating this window as 
 * modified. 
 */
void MainWindow::projectModified()
{
   setWindowModified(true);
}






/*!
 * Called when this window's project has been saved, updating this window as 
 * unmodified. 
 */
void MainWindow::projectSaved()
{
   setWindowModified(false);
}






/*!
 * Called when this window's project file has been modified by an outside source. 
 * This executes a dialog informing the user of this fact and reloads the project 
 * if the user chooses to do so. 
 */
void MainWindow::projectFileChanged()
{
   // Create a new message box that informs the user the project file has changed and 
   // asks the user if they want to reload the project or ignore it. 
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::AcceptRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);

   // Execute the message box and check to see if they chose to reload the project. 
   int answer = notice.exec();
   if ( answer != QMessageBox::AcceptRole ) return;

   // Reload the project by creating a new project with the same file path and 
   // setting this window's project to the reloaded one. 
   setProject(Soc::Ut::QPtr<Project>(new Project(_project->path())));
}






/*!
 * Updates this window's title, including the project name if it has one. 
 */
void MainWindow::updateTitle()
{
   // If this window has a project then set the window title with the project name 
   // else just set it to the application's name. 
   if ( _project )
   {
      setWindowTitle(tr("%1[*] (%2) - Socrates' Reference")
                     .arg(_project->name())
                     .arg(Abstract::ProjectFactory::instance().name(_project->type())));
   }
   else setWindowTitle(tr("Socrates' Reference"));
}






/*!
 * Updates some of this window's actions by enabling or disabling them based off 
 * aspects of this window's project. 
 */
void MainWindow::updateActions()
{
   // Enable or disable the save action based off this window's project. 
   if ( _project ) _saveAction->setDisabled(_project->isNew());
   else _saveAction->setDisabled(true);

   // Enable or disable all other relevant actions based off this window having a 
   // project or not. 
   _saveAsAction->setDisabled(!_project);
   _closeAction->setDisabled(!_project);
   _dictionaryAction->setDisabled(!_project);
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
   // If this window has no project or the project is not modified then return that 
   // it is OK. 
   if ( !_project || !_project->isModified() ) return true;

   // Create a message box that queries the user about what to do with this window's 
   // project's unsaved changes. 
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Unsaved Project Changes"));
   confirm.setText(tr("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!"));
   confirm.setIcon(QMessageBox::Question);
   confirm.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard);

   // Execute the message box and get the user's response. 
   switch (confirm.exec())
   {
   case QMessageBox::Save:

      // If this window's project is new then attempt to save as else attempt to save. 
      if ( _project->isNew() ) return saveAs();
      else return save();

   // If the user choose cancel then return that it is not OK else the user choose 
   // discard so return that it is OK. 
   case QMessageBox::Cancel: return false;
   default: return true;
   }
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
   // Make sure this window has a project to save. 
   if ( !_project ) return false;

   // Create a new file dialog to query the user for a file path to save this 
   // window's project. 
   QFileDialog dialog(nullptr,tr("Save Project"),"",tr("Socrates' Reference File (*.scr)"));
   dialog.setAcceptMode(QFileDialog::AcceptSave);

   // Execute the file dialog and make sure it returned accepted. 
   if ( !dialog.exec() ) return false;

   // Get the first file path from the file dialog chosen by the user. 
   QStringList files = dialog.selectedFiles();
   const QString path = files.constFirst();

   try
   {
      // Attempt to save this window's project to the file path. 
      _project->saveAs(path);

      // Update this window's actions and return success. 
      updateActions();
      return true;
   }

   // If an exception was thrown then report it to the user and return false. 
   catch (Exception e)
   {
      QMessageBox::warning(this,tr("Failed Saving Project"),e.message());
      return false;
   }
}






/*!
 * Saves this window's project to its file, returning true on success or false on 
 * failure. If this window has no project then false is returned. 
 *
 * @return True if the project was successfully saved or false otherwise. 
 */
bool MainWindow::save()
{
   // Make sure this window has a project to save. 
   if ( !_project ) return false;

   try
   {
      // Attempt to save this window's project. 
      _project->save();

      // Return success. 
      return true;
   }

   // If an exception was thrown then report it to the user and return false. 
   catch (Exception e)
   {
      QMessageBox::warning(this,tr("Failed Saving Project"),e.message());
      return false;
   }
}






/*!
 * Restores the state and geometry of the main window from Qt settings. 
 */
void MainWindow::restoreSettings()
{
   // Create a settings object using this application's keys. 
   QSettings settings;

   // Restore this window's state and geometry. 
   restoreGeometry(settings.value(_geometryKey).toByteArray());
   restoreState(settings.value(_stateKey).toByteArray());

   // Restore this window's block view state. 
   _view->restoreState(settings.value(_viewStateKey).toByteArray());
}






/*!
 * Saves the state and geometry of the main window to Qt settings. 
 */
void MainWindow::saveSettings()
{
   // Create a settings object using this application's keys. 
   QSettings settings;

   // Save this window's state and geometry. 
   settings.setValue(_geometryKey,saveGeometry());
   settings.setValue(_stateKey,saveState());

   // Save this window's block view state. 
   settings.setValue(_viewStateKey,_view->saveState());
}






/*!
 * Constructs and initializes all GUI elements for this new main window. 
 */
void MainWindow::setupGui()
{
   // Load this application's logo icon if it hasn't already been loaded. 
   static QIcon icon;
   if ( icon.isNull() ) icon = QIcon(":/icons/main.svg");

   // Create the block view for this window and set it as the central widget. 
   _view = new BlockView(this);
   setCentralWidget(_view);

   // Call the status bar method so it is created for this new window right away. 
   statusBar();

   // Set this window's icon and create it's actions and menus. 
   setWindowIcon(icon);
   setupActions();
   setupMenus();
}






/*!
 * Constructs and initializes all actions for this new window. 
 */
void MainWindow::setupActions()
{
   // Create and initialize the open action for this window. 
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);

   // Create and initialize the save action for this window. 
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);

   // Create and initialize the save as action for this window. 
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setShortcut(QKeySequence::SaveAs);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);

   // Create and initialize the dictionary action for this window. 
   _dictionaryAction = new QAction(tr("&Dictionary"),this);
   _dictionaryAction->setStatusTip(tr("Edit the custom dictionary of a project."));
   connect(_dictionaryAction,&QAction::triggered,this,&MainWindow::dictionaryTriggered);

   // Create and initialize the properties action for this window. 
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);

   // Create and initialize the scan action for this window. 
   _scanAction = new QAction(tr("Scan"),this);
   _scanAction->setStatusTip(tr("Scan and parse source files to add documentation."));
   _scanAction->setShortcut(Qt::CTRL + Qt::Key_B);
   connect(_scanAction,&QAction::triggered,this,&MainWindow::scanTriggered);

   // Create and initialize the close action for this window. 
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);

   // Create and initialize the exit action for this window. 
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&QWidget::close);

   // Create and initialize the about action for this window. 
   _aboutAction = new QAction(tr("&About"),this);
   _aboutAction->setStatusTip(tr("Get information about this program."));
   connect(_aboutAction,&QAction::triggered,this,&MainWindow::aboutTriggered);

   // Create and initialize the about qt action for this window. 
   _aboutQtAction = new QAction(tr("&About Qt"),this);
   _aboutQtAction->setStatusTip(tr("Get information about the Qt library."));
   connect(_aboutQtAction,&QAction::triggered,this,&MainWindow::aboutQtTriggered);

   // Create the new actions and setting actions. 
   setupNewActions();
   setupSettingActions();
}






/*!
 * Creates this window's new actions for each project type that exists. 
 */
void MainWindow::setupNewActions()
{
   // Iterate through every project type. 
   Abstract::ProjectFactory& factory = Abstract::ProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      // Create a new action for the project type and connect its triggered signal. 
      _newActions.append(new QAction(factory.name(i),this));
      connect(_newActions.back(),&QAction::triggered,[this,i]{ newTriggered(i); });
   }
}






/*!
 * Creates this window's setting actions for each project type that exists. 
 */
void MainWindow::setupSettingActions()
{
   // Iterate through every project type. 
   Abstract::ProjectFactory& factory = Abstract::ProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      // Create a settings action for the project type and connect its triggered signal. 
      _settingActions.append(new QAction(factory.name(i),this));
      connect(_settingActions.back(),&QAction::triggered,[this,i]{ settingTriggered(i); });
   }
}






/*!
 * Constructs and initializes this new window's menus, adding them to its menu bar. 
 */
void MainWindow::setupMenus()
{
   // Create and add the file menu to this window's menu bar. 
   setupFileMenu();

   // Add this window's block view's context menu to its menu bar. 
   menuBar()->addMenu(_view->contextMenu());

   // Add the settings menu to this window's menu bar, adding all setting actions. 
   QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
   for (auto action : qAsConst(_settingActions)) settingsMenu->addAction(action);

   menuBar()->addSeparator();
   // Add the help menu to this window's menu bar, adding the about and about qt 
   // actions. 
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
   helpMenu->addAction(_aboutAction);
   helpMenu->addAction(_aboutQtAction);
}






/*!
 * Constructs and initializes this new window's file menu, adding it to this 
 * window's menu bar. 
 */
void MainWindow::setupFileMenu()
{
   // Create and add the new file menu to this window's menu bar. 
   QMenu* file = menuBar()->addMenu(tr("&File"));

   // Add the new menu to the file menu, populating it with all new actions of this 
   // window. 
   QMenu* newMenu = file->addMenu(tr("&New"));
   for (auto action : qAsConst(_newActions)) newMenu->addAction(action);

   // Add all other actions to the file menu, adding separators where appropriate. 
   file->addAction(_openAction);
   file->addAction(_saveAction);
   file->addAction(_saveAsAction);
   file->addAction(_closeAction);
   file->addSeparator();
   file->addAction(_dictionaryAction);
   file->addAction(_propertiesAction);
   file->addAction(_scanAction);
   file->addSeparator();
   file->addAction(_exitAction);
}
