#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>



class Project;



//@@
namespace Gui {

//@@
class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow(QWidget* parent = nullptr);
   void setProject(Project* o_project);
private slots:
   void newTriggered();
   void openTriggered();
   void saveTriggered();
   void saveAsTriggered();
   void closeTriggered();
   void projectSettingsTriggered();
   void projectNameChanged();
   void projectModified();
   void projectSaved();
   void projectFileChanged();
private:
   void createActions();
   void createMenus();
   void updateTitle();
   void updateActions();
   bool isOkToContinue();
   bool saveAs();
   bool save();
   Project* _project {nullptr};
   QVector<QAction*> _newActions;
   QAction* _openAction;
   QAction* _saveAction;
   QAction* _saveAsAction;
   QAction* _closeAction;
   QAction* _projectSettingsAction;
};



//@@
inline void MainWindow::saveTriggered() { save(); }

//@@
inline void MainWindow::saveAsTriggered() { saveAs(); }

//@@
inline void MainWindow::projectNameChanged() { updateTitle(); }

//@@
inline void MainWindow::projectModified() { setWindowModified(true); }

//@@
inline void MainWindow::projectSaved() { setWindowModified(false); }

} // namespace Gui



#endif
