#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "global.h"
#include "gui.h"
//



namespace Gui
{
   /*!
    * This is the main window for the program. This class can contain an open class 
    * along with providing a menu bar for the user. Each window can contain only one 
    * project, but multiple instance windows of this class can exist in the program. 
    * When the last main window is deleted this program exits. 
    * 
    * The menu bar provided by this class window contains the file, edit, settings, 
    * and help menus. The file menu provides basic actions for the window. These basic 
    * actions include creating a new project, opening a project, saving a project, 
    * opening the properties of an open project, scanning the source files of an open 
    * project, closing an open project, and exiting a main window. 
    * 
    * The edit menu is the context menu of the block view class. The settings menu is 
    * the global settings for each project type. Lastly the help menu currently just 
    * provides an about dialog. 
    */
   class MainWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      explicit MainWindow(QWidget* parent = nullptr);
      void setProject(std::unique_ptr<Project>&& project);
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private slots:
      void newTriggered(int type);
      void openTriggered();
      void saveTriggered();
      void saveAsTriggered();
      void propertiesTriggered();
      void scanTriggered();
      void closeTriggered();
      void settingTriggered(int type);
      void aboutTriggered();
      void projectNameChanged();
      void projectModified();
      void projectSaved();
      void projectFileChanged();
   private:
      void updateTitle();
      void updateActions();
      bool isOkToClose();
      bool saveAs();
      bool save();
      void restoreSettings();
      void saveSettings();
      void setupGui();
      void setupActions();
      void setupNewActions();
      void setupSettingActions();
      void setupMenus();
      void setupFileMenu();
      /*!
       * The key used to save/restore the geometry of the main window using Qt settings. 
       */
      static const char* _geometryKey;
      /*!
       * The key used to save/restore the state of the main window using Qt settings. 
       */
      static const char* _stateKey;
      /*!
       * Pointer to this window's block view. 
       */
      BlockView* _view;
      /*!
       * Pointer to this window's project. 
       */
      Project* _project {nullptr};
      /*!
       * List of new actions for this window used to create new projects of a given type 
       * for each action. 
       */
      QList<QAction*> _newActions;
      /*!
       * Pointer this this window's open action. 
       */
      QAction* _openAction;
      /*!
       * Pointer this this window's save action. 
       */
      QAction* _saveAction;
      /*!
       * Pointer this this window's save as action. 
       */
      QAction* _saveAsAction;
      /*!
       * Pointer this this window's property action used to open the project properties 
       * dialog. 
       */
      QAction* _propertiesAction;
      /*!
       * Pointer this this window's scan action. 
       */
      QAction* _scanAction;
      /*!
       * Pointer this this window's close action. 
       */
      QAction* _closeAction;
      /*!
       * Pointer this this window's exit action. 
       */
      QAction* _exitAction;
      /*!
       * List of setting actions for this window used to open the global settings of a 
       * given project type for each action. 
       */
      QList<QAction*> _settingActions;
      /*!
       * Pointer this this window's about action. 
       */
      QAction* _aboutAction;
   };
}



#endif
