#ifndef GUI_PROJECTDIALOG_H
#define GUI_PROJECTDIALOG_H
#include <QDialog>
#include "global.h"



class QFormLayout;
class QHBoxLayout;
class QLineEdit;



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
      void setupGui();
      QFormLayout* setupForm();
      QHBoxLayout* setupButtons();
      QPushButton* setupButton(const QString& text, void (ProjectDialog::*pointer)());
      Project* _project;
      QLineEdit* _nameEdit;
      QLineEdit* _scanDirectoryEdit;
      QLineEdit* _filtersEdit;
   };
}



#endif
