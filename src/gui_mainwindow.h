#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>

#include <QMainWindow>

#include "exception.h"
#include "project.h"
#include "abstractblock.h"
#include "gui_blockview.h"



namespace Gui
{
   class MainWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      explicit MainWindow(QWidget* parent = nullptr);
      MainWindow& setProject(std::unique_ptr<Project>&& project);
   private slots:
      void newTriggered();
      void openTriggered();
      void saveTriggered() { save(); }
      void saveAsTriggered() { saveAs(); }
      void propertiesTriggered();
      void closeTriggered();
      void projectNameChanged() { updateTitle(); }
      void projectModified() { setWindowModified(true); }
      void projectSaved() { setWindowModified(false); }
      void projectFileChanged();
   private:
      virtual void closeEvent(QCloseEvent* event) override final;
      void createActions();
      void createOpenAction();
      void createSaveAction();
      void createSaveAsAction();
      void createPropertiesAction();
      void createCloseAction();
      void createExitAction();
      void createMenus();
      void createView();
      void updateTitle();
      void updateActions();
      bool isOkToContinue();
      bool saveAs();
      bool save();
      Project* _project {nullptr};
      QList<QAction*> _newActions;
      BlockView* _view;
      QAction* _openAction;
      QAction* _saveAction;
      QAction* _saveAsAction;
      QAction* _propertiesAction;
      QAction* _closeAction;
      QAction* _exitAction;
   };
}



#endif
