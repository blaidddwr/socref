#ifndef CPPQT_SETTINGS_DIALOG_H
#define CPPQT_SETTINGS_DIALOG_H
#include "cppqt_settings.h"
#include "gui_persistentdialog.h"



class QSpinBox;
//



namespace CppQt
{
   /*!
    * This is the settings dialog. This allows the user to edit all global settings 
    * for the C++/Qt project type. 
    */
   class Settings::Dialog : public ::Gui::PersistentDialog
   {
      Q_OBJECT
   public:
      explicit Dialog();
   private slots:
      void okClicked();
      void applyClicked();
   private:
      void setupGui();
      QLayout* setupForm();
      QLayout* setupButtons();
      /*!
       * The edit widget for the indent spaces setting. 
       */
      QSpinBox* _indentSpacesBox;
      /*!
       * The edit widget for the header lines setting. 
       */
      QSpinBox* _headerLinesBox;
      /*!
       * The edit widget for the function lines setting. 
       */
      QSpinBox* _functionLinesBox;
      /*!
       * The edit widget for the max columns setting. 
       */
      QSpinBox* _maxColumnsBox;
   };
}



#endif
