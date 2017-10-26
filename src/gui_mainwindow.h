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
   void addTriggered();
   void removeTriggered();
   void editTriggered();
   void copyTriggered();
   void cutTriggered();
   void pasteTriggered();
   void moveUpTriggered();
   void moveDownTriggered();
   void projectNameChanged();
   void projectModified();
   void projectSaved();
   void projectFileChanged();
   void blockSelected(AbstractBlock* block);
   void blockCopied(AbstractBlock* o_block);
private:
   virtual void closeEvent(QCloseEvent* event) override final;
   void createActions();
   void createMenus();
   void updateTitle();
   void updateActions();
   void updateAddMenu();
   bool isOkToContinue();
   bool saveAs();
   bool save();
   Project* _project {nullptr};
   AbstractBlock* _selected {nullptr};
   AbstractBlock* _copy {nullptr};
   QList<QAction*> _newActions;
   QAction* _openAction;
   QAction* _saveAction;
   QAction* _saveAsAction;
   QAction* _propertiesAction;
   QAction* _closeAction;
   QAction* _exitAction;
   QList<QAction*> _addActions;
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
