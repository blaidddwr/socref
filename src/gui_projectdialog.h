#ifndef GUI_PROJECTDIALOG_H
#define GUI_PROJECTDIALOG_H
#include <QDialog>



class Project;
class QLineEdit;
class QHBoxLayout;
class QFormLayout;



namespace Gui
{
   class ProjectDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit ProjectDialog(Project* project, QWidget* parent = nullptr);
   private slots:
      void okClicked();
      void applyClicked();
      void browseClicked();
   private:
      void createDialog();
      QFormLayout* createForm();
      QHBoxLayout* createButtons();
      QPushButton* createButton(const QString& text, void (ProjectDialog::*pointer)());
      Project* _project;
      QLineEdit* _nameEdit;
      QLineEdit* _scanDirectoryEdit;
      QLineEdit* _filtersEdit;
   };
}



#endif
