#ifndef GUI_PROJECTDIALOG_H
#define GUI_PROJECTDIALOG_H
#include "gui_persistentdialog.h"
#include "global.h"
class QFormLayout;
class QHBoxLayout;
class QLineEdit;
class QListView;



/*!
 * Contains all GUI elements for this application.
 */
namespace Gui
{
   /*!
    * This is the project properties dialog. This dialog allows the user to view
    * and edit the basic properties of a project. This is a persistent dialog and
    * remembers its geometry.
    */
   class ProjectDialog : public PersistentDialog
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
      QLayout* setupForm();
      QLayout* setupDirectory();
      QLayout* setupButtons();
      /*!
       * Pointer to the project this properties dialog is presenting to the user.
       */
      Project* _project;
      /*!
       * Pointer to the name edit widget for this dialog that represents the project's
       * name.
       */
      QLineEdit* _nameEdit;
      /*!
       * Pointer to the scan directory edit widget for this dialog that represents the
       * project's scan directory.
       */
      QLineEdit* _scanDirectoryEdit;
      /*!
       * Pointer to the filters edit widget for this dialog that represents the
       * project's scan filters.
       */
      QLineEdit* _filtersEdit;
   };
}

#endif
