#ifndef CPPQT_GUI_SETTINGSDIALOG_H
#define CPPQT_GUI_SETTINGSDIALOG_H
#include <QSettings>
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
         static int indentSpaces();
         static int headerLines();
         static int functionLines();
      private slots:
         void okClicked();
         void applyClicked();
      private:
         void setupGui();
         QLayout* setupForm();
         void addIndentSpaces(QFormLayout* layout);
         void addHeaderLines(QFormLayout* layout);
         void addFunctionLines(QFormLayout* layout);
         QLayout* setupButtons();
         QWidget* setupOkButton();
         QWidget* setupApplyButton();
         QWidget* setupCancelButton();
         static const char* _indentSpacesKey;
         static const char* _headerLinesKey;
         static const char* _functionLinesKey;
         QSettings _settings;
         QSpinBox* _indentSpacesBox;
         QSpinBox* _headerLinesBox;
         QSpinBox* _functionLinesBox;
      };
   }
}



#endif
