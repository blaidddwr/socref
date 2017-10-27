#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>



namespace Exception {

class Base;

} // namespace Exception

class Project;
class AbstractBlock;



//@@
namespace Gui {

class BlockView;



//@@
class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit MainWindow(QWidget* parent = nullptr);
   void setProject(Project* o_project);
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
   void viewSelectionChanged();
private:
   virtual void closeEvent(QCloseEvent* event) override final;
   void createActions();
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
   QAction* _removeAction;
   QAction* _editAction;
   QAction* _copyAction;
   QAction* _cutAction;
   QAction* _pasteAction;
   QAction* _moveUpAction;
   QAction* _moveDownAction;
   QMenu* _addMenu;
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
