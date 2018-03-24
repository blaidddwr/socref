#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "global.h"
#include "gui.h"



namespace Gui
{
   class MainWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      explicit MainWindow(QWidget* parent = nullptr);
      void setProject(std::unique_ptr<Project>&& project);
   private slots:
      void newTriggered();
      void openTriggered();
      void saveTriggered();
      void saveAsTriggered();
      void propertiesTriggered();
      void scanTriggered();
      void closeTriggered();
      void settingTriggered();
      void projectNameChanged();
      void projectModified();
      void projectSaved();
      void projectFileChanged();
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private:
      void updateTitle();
      void updateActions();
      bool isOkToContinue();
      bool saveAs();
      bool save();
      void restoreSettings();
      void saveSettings();
      void setupGui();
      void setupActions();
      void setupNewActions();
      void setupOpenAction();
      void setupSaveAction();
      void setupSaveAsAction();
      void setupPropertiesAction();
      void setupScanAction();
      void setupCloseAction();
      void setupExitAction();
      void setupSettingActions();
      void setupMenus();
      void setupFileMenu();
      void setupSettingsMenu();
      void setupView();
      static const char* _geometryKey;
      static const char* _stateKey;
      Project* _project {nullptr};
      QList<QAction*> _newActions;
      QList<QAction*> _settingActions;
      BlockView* _view;
      QAction* _openAction;
      QAction* _saveAction;
      QAction* _saveAsAction;
      QAction* _propertiesAction;
      QAction* _scanAction;
      QAction* _closeAction;
      QAction* _exitAction;
   };
}



#endif
