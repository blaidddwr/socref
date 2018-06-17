#ifndef CPPQT_GUI_SETTINGSDIALOG_H
#define CPPQT_GUI_SETTINGSDIALOG_H
#include "gui_persistentdialog.h"



class QFormLayout;
class QSpinBox;
//



namespace CppQt
{
   namespace Gui
   {
      class SettingsDialog : public ::Gui::PersistentDialog
      {
      public:
         SettingsDialog();
      private slots:
         void okClicked();
         void applyClicked();
         void cancelClicked();
      private:
         void setupGui();
         QLayout* setupForm();
         void addIndentSpaces(QFormLayout* layout);
         void addHeaderLines(QFormLayout* layout);
         void addFunctionLines(QFormLayout* layout);
         void addMaxColumns(QFormLayout* layout);
         QLayout* setupButtons();
         QWidget* setupOkButton();
         QWidget* setupApplyButton();
         QWidget* setupCancelButton();
         QSpinBox* _indentSpacesBox;
         QSpinBox* _headerLinesBox;
         QSpinBox* _functionLinesBox;
         QSpinBox* _maxColumnsBox;
      };
   }
}



#endif
