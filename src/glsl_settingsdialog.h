#ifndef GLSL_SETTINGSDIALOG_H
#define GLSL_SETTINGSDIALOG_H
#include "gui_persistentdialog.h"
class QSpinBox;



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the settings dialog. This allows the user to edit all global settings
    * for the GLSL project type.
    */
   class SettingsDialog : public Gui::PersistentDialog
   {
      Q_OBJECT
   public:
      explicit SettingsDialog();
   private slots:
      void okClicked();
      void applyClicked();
   private:
      void setupGui();
      QLayout* setupForm();
      QLayout* setupButtons();
   private:
      /*!
       * The edit widget for the variable lines setting.
       */
      QSpinBox* _variableLinesBox;
      /*!
       * The edit widget for the structure lines setting.
       */
      QSpinBox* _structLinesBox;
      /*!
       * The edit widget for the function lines setting.
       */
      QSpinBox* _functionLinesBox;
      /*!
       * The edit widget for the indent spaces setting.
       */
      QSpinBox* _indentSpacesBox;
      /*!
       * The edit widget for the max columns setting.
       */
      QSpinBox* _maxColumnsBox;
   };
}

#endif
