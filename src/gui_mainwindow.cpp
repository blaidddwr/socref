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
 * Constructs a new main window with the optional parent. 
 *
 * @param parent Optional parent for this new main window. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this object's GUI, restore the state and geometry of the main window, 
 *    update this object's title, and update this object's actions. 
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
 * Saves the state and geometry of the main window. 
 */
MainWindow::~MainWindow()
{
   saveSettings();
}






/*!
 * Sets the given project as this main window's new project, deleting any previous 
 * project this window may contain. This window takes ownership of any new project 
 * given to it. A null pointer can be given which means this window will have no 
 * project. 
 *
 * @param project  
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete any previous project this window may contain, setting the new pointer 
 *    as this window's project. 
 *
 * 2. Update this window's title and file menu actions. If the new project given is 
 *    a null pointer then return. 
 *
 * 3. Set this window's new project's parent as this window, set this window's 
 *    block view with the new project's model, and set this window's modified state 
 *    based on the new project's modified state. 
 *
 * 4. Connect all project signals to this window's slots. 
 */
void MainWindow::setProject(std::unique_ptr<Project>&& project)
{
   // 1
   delete _project;
   _project = project.release();

   // 2
   updateTitle();
   updateActions();
   if ( !_project ) return;

   // 3
   _project->setParent(this);
   _view->setModel(_project->model());
   setWindowModified(_project->isModified());

   // 4
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If it is OK to close this window then accept the given event else reject it. 
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
   // 1
   if ( isOkToClose() ) event->accept();
   else event->ignore();
}






/*!
 * Called when an add action is triggered. This creates a new project of the given 
 * type and sets it to this window if it has none or else a new main window is 
 * created. 
 *
 * @param type The project type used to create a new project. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create new project _project_ with the given type and set its name to the 
 *    default. 
 *
 * 2. If this window has no project then set it to _project_, else create a new 
 *    main window and set the new window's project to _project_. 
 */
void MainWindow::newTriggered(int type)
{
   // 1
   unique_ptr<Project> project {new Project(type)};
   project->setName(tr("Untitled Project"));

   // 2
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a file dialog _file_ and query the user for a project file to open by 
 *    modal execution of it. If execution fails then return. 
 *
 * 2. Create a string _path_ setting it to the path of the first file selected from 
 *    the file dialog _file_. 
 *
 * 3. Open a new project with the path _path_. If this window has no project that 
 *    set it to this new project, else create a new main window with the new 
 *    project. 
 *
 * 4. If any exception is caught while opening the new project report it to the 
 *    user. 
 */
void MainWindow::openTriggered()
{
   // 1
   QFileDialog fileDialog(nullptr,tr("Open Project"),"",tr("Socrates' Reference File (*.scr)"));
   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
   if ( !fileDialog.exec() ) return;

   // 2
   QStringList files = fileDialog.selectedFiles();
   const QString path = files.constFirst();

   // 3
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

   // 4
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has a project then create a new project dialog with this 
 *    window and its project executing it in modal mode. 
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has a project then create a new scanner from its project, then 
 *    create a new scan dialog with the new scanner, and then modal execute the new 
 *    dialog. 
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new setting dialog _settings_ using the given project type. If the 
 *    returned settings dialog is null then throw an exception. 
 *
 * 2. Execute the _settings_ dialog in modal mode. 
 */
void MainWindow::settingTriggered(int type)
{
   // 1
   unique_ptr<QDialog> settings {AbstractProjectFactory::instance().makeSettings(type)};
   if ( !settings )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Project factory's make settings returned a null pointer."));
      throw e;
   }

   // 2
   settings->exec();
}






/*!
 * Called when the about action is triggered, executing the about dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an about dialog and execute it in modal mode. 
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new message box that informs the user the project file has changed 
 *    and ask the user if they want to reload the project or ignore this event. If 
 *    the user chooses to ignore it then return. 
 *
 * 2. Create a new project with the current project's file and then set the new 
 *    project to this window. 
 *
 * 3. If any exception is caught while attempting to reload the project then inform 
 *    the user. 
 */
void MainWindow::projectFileChanged()
{
   // 1
   QMessageBox notice;
   notice.setWindowTitle(tr("Project File Changed"));
   notice.setText(tr("The currently open project's file has been modified."));
   notice.setIcon(QMessageBox::Warning);
   notice.addButton(tr("Reload"),QMessageBox::AcceptRole);
   notice.addButton(tr("Ignore"),QMessageBox::RejectRole);
   int answer = notice.exec();
   if ( answer != QMessageBox::AcceptRole ) return;

   // 2
   try
   {
      unique_ptr<Project> project {new Project(_project->path())};
      setProject(std::move(project));
   }

   // 3
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to reload the project."),e);
   }
}






/*!
 * Updates this window's title, including the project name if it has one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has a project then set the window title with the project name 
 *    and type along with the application's name, else just set it to the 
 *    application's name. 
 */
void MainWindow::updateTitle()
{
   // 1
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has a project and it is now new then enable the save action 
 *    else disable it. 
 *
 * 2. If this window has a project then enable the save as, close, properties, and 
 *    scan actions. Else if this window has no project disable the same actions. 
 */
void MainWindow::updateActions()
{
   // 1
   if ( _project ) _saveAction->setDisabled(_project->isNew());
   else _saveAction->setDisabled(true);

   // 2
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has no project or the project is not modified then return 
 *    true. 
 *
 * 2. Create a Qt message box _confirm_ that queries the user about what to do with 
 *    this window's project's unsaved changes. The options are to save the project, 
 *    cancel closing it, or discard the changes. 
 *
 * 3. Executes the _confirm_ dialog in modal mode and get the user's response. If 
 *    cancel is chosen then return false, else if save is chosen then attempt to 
 *    save the project returning its result, else if discard is chosen then return 
 *    true. 
 */
bool MainWindow::isOkToClose()
{
   // 1
   if ( !_project || !_project->isModified() ) return true;

   // 2
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Unsaved Project Changes"));
   confirm.setText(tr("The currently open project has unsaved changes. Closing the project will cause all unsaved changes to be lost!"));
   confirm.setIcon(QMessageBox::Question);
   confirm.setStandardButtons(QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard);

   // 3
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has no project then return false. 
 *
 * 2. Create a new file dialog _dialog_, setting its accept mode to saving a file 
 *    and executing it in modal mode. If execution fails then return false. 
 *
 * 3. Get the file path from _dialog_ chosen by the user and attempt to save the 
 *    project with that file path. If a Socrates exception is caught then report it 
 *    to the user and return false. 
 *
 * 4. Update this window's actions and return true on success. 
 */
bool MainWindow::saveAs()
{
   // 1
   if ( !_project ) return false;

   // 2
   QFileDialog dialog(nullptr,tr("Save Project"),"",tr("Socrates' Reference File (*.scr)"));
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   if ( !dialog.exec() ) return false;

   // 3
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

   // 4
   updateActions();
   return true;
}






/*!
 * Saves this window's project to its file, returning true on success or false on 
 * failure. If this window has no project then false is returned. 
 *
 * @return True if the project was successfully saved or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this window has no project then return false. 
 *
 * 2. Attempt to save this project. If any Socrates exception is caught then report 
 *    it to the user and return false. 
 *
 * 3. Return true on success. 
 */
bool MainWindow::save()
{
   // 1
   if ( !_project ) return false;

   // 2
   try
   {
      _project->save();
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while attempting to save the project."),e);
      return false;
   }

   // 3
   return true;
}






/*!
 * Restores the state and geometry of the main window from Qt settings. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Restore this window's state and geometry for the main window using Qt 
 *    settings and this class's keys. 
 */
void MainWindow::restoreSettings()
{
   // 1
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
   restoreState(settings.value(_stateKey).toByteArray());
}






/*!
 * Saves the state and geometry of the main window to Qt settings. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Save this window's state and geometry for the main window using Qt settings 
 *    and this class's keys. 
 */
void MainWindow::saveSettings()
{
   // 1
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
   settings.setValue(_stateKey,saveState());
}






/*!
 * Constructs and initializes all GUI elements for this new main window. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Load this application's logo icon if it hasn't already been loaded and then 
 *    set this window's icon to it. 
 *
 * 2. Create a new block view object for this window, setting it as this window's 
 *    central widget. 
 *
 * 3. Setup this new window's actions and menus. 
 */
void MainWindow::setupGui()
{
   // 1
   static QIcon icon;
   if ( icon.isNull() ) icon = QIcon(":/icons/main.svg");
   setWindowIcon(icon);

   // 2
   _view = new BlockView(this);
   setCentralWidget(_view);

   // 3
   setupActions();
   setupMenus();
}






/*!
 * Constructs and initializes all actions for this new window. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new action for this window's open action. 
 *
 * 2. Create a new action for this window's save action. 
 *
 * 3. Create a new action for this window's save as action. 
 *
 * 4. Create a new action for this window's properties action. 
 *
 * 5. Create a new action for this window's scan action. 
 *
 * 6. Create a new action for this window's close action. 
 *
 * 7. Create a new action for this window's exit action, connecting its triggered 
 *    signal to close this window. 
 *
 * 8. Create a new action for this window's about action. 
 *
 * 9. Setup the new project and project settings actions. 
 */
void MainWindow::setupActions()
{
   // 1
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing project."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::openTriggered);

   // 2
   _saveAction = new QAction(tr("&Save"),this);
   _saveAction->setShortcut(QKeySequence::Save);
   _saveAction->setStatusTip(tr("Save current project."));
   connect(_saveAction,&QAction::triggered,this,&MainWindow::saveTriggered);

   // 3
   _saveAsAction = new QAction(tr("Save &As"),this);
   _saveAsAction->setShortcut(QKeySequence::SaveAs);
   _saveAsAction->setStatusTip(tr("Save current project under a new path."));
   connect(_saveAsAction,&QAction::triggered,this,&MainWindow::saveAsTriggered);

   // 4
   _propertiesAction = new QAction(tr("&Properties"),this);
   _propertiesAction->setStatusTip(tr("Edit basic properties of a project."));
   connect(_propertiesAction,&QAction::triggered,this,&MainWindow::propertiesTriggered);

   // 5
   _scanAction = new QAction(tr("Scan"),this);
   _scanAction->setStatusTip(tr("Scan and parse source files to add documentation."));
   _scanAction->setShortcut(Qt::CTRL + Qt::Key_B);
   connect(_scanAction,&QAction::triggered,this,&MainWindow::scanTriggered);

   // 6
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the current project."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::closeTriggered);

   // 7
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit this window."));
   connect(_exitAction,&QAction::triggered,this,&QWidget::close);

   // 8
   _aboutAction = new QAction(tr("&About"),this);
   _aboutAction->setStatusTip(tr("Get information about this program."));
   connect(_aboutAction,&QAction::triggered,this,&MainWindow::aboutTriggered);

   // 9
   setupNewActions();
   setupSettingActions();
}






/*!
 * Creates this window's new actions for each project type that exists. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through every project type, creating a new action for each one. Add 
 *    each new action to this window's list of new actions and connect its signal 
 *    to this window's appropriate slot using a lambda function to save the type. 
 */
void MainWindow::setupNewActions()
{
   // 1
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _newActions.append(new QAction(factory.name(i),this));
      connect(_newActions.back(),&QAction::triggered,[this,i]{ newTriggered(i); });
   }
}






/*!
 * Creates this window's setting actions for each project type that exists. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through every project type, creating a new action for each one. Add 
 *    each new action to this window's list of setting actions and connect its 
 *    signal to this window's appropriate slot using a lambda function to save the 
 *    type. 
 */
void MainWindow::setupSettingActions()
{
   // 1
   AbstractProjectFactory& factory = AbstractProjectFactory::instance();
   for (int i = 0; i < factory.size() ;++i)
   {
      _settingActions.append(new QAction(factory.name(i),this));
      connect(_settingActions.back(),&QAction::triggered,[this,i]{ settingTriggered(i); });
   }
}






/*!
 * Constructs and initializes this new window's menus, adding them to its menu bar. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup and add the file menu and then add the context menu from this new 
 *    window's block view object. 
 *
 * 2. Add the settings menu, adding all setting actions to the menu. 
 *
 * 3. Add the help menu, adding the single about action to it. 
 */
void MainWindow::setupMenus()
{
   // 1
   setupFileMenu();
   menuBar()->addMenu(_view->contextMenu());

   // 2
   QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));
   for (auto action : qAsConst(_settingActions)) settingsMenu->addAction(action);

   // 3
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
   helpMenu->addAction(_aboutAction);
}






/*!
 * Construct and initialize this new window's file menu, adding it to this window's 
 * menu bar. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Add a new file menu to this window's menu bar, saving its pointer to _file_. 
 *
 * 2. Add a new sub menu to _file_, populating it with all new actions of this 
 *    window. 
 *
 * 3. Add all other actions to _file_, adding separators where appropriate. 
 */
void MainWindow::setupFileMenu()
{
   // 1
   QMenu* file = menuBar()->addMenu(tr("&File"));

   // 2
   QMenu* newMenu = file->addMenu(tr("&New"));
   for (auto action : qAsConst(_newActions)) newMenu->addAction(action);

   // 3
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
