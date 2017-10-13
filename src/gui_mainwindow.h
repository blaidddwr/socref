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
   void setProject(Project* project);
private slots:
   void newTriggered();
   void openTriggered();
   void saveTriggered();
   void saveAsTriggered();
   void closeTriggered();
   void projectSettingsTriggered();
private:
   void createActions();
   void createMenus();
   Project* _project {nullptr};
   QVector<QAction*> _newActions;
   QAction* _openAction;
   QAction* _saveAction;
   QAction* _saveAsAction;
   QAction* _closeAction;
   QAction* _projectSettingsAction;
};

} // namespace Gui



#endif
