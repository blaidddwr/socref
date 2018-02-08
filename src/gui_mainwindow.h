#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <memory>
#include <QMainWindow>
#include "global.h"
#include "gui.h"
#include "exception_base.h"



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
      void closeTriggered();
      void projectNameChanged();
      void projectModified();
      void projectSaved();
      void projectFileChanged();
   private:
      virtual void closeEvent(QCloseEvent* event) override final;
      void setupGui();
      void setupActions();
      void setupNewActions();
      void setupOpenAction();
      void setupSaveAction();
      void setupSaveAsAction();
      void setupPropertiesAction();
      void setupCloseAction();
      void setupExitAction();
      void setupMenus();
      void setupView();
      void updateTitle();
      void updateActions();
      bool isOkToContinue();
      bool saveAs();
      bool save();
      void showException(const QString& text, const Exception::Base& exception) const;
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
