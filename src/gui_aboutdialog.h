#ifndef GUI_ABOUTDIALOG_H
#define GUI_ABOUTDIALOG_H
#include "gui_persistentdialog.h"
//



namespace Gui
{
   /*!
    * This is a simple dialog that presents information about this program. The only 
    * functionality is to simply close it by clicking OK. 
    */
   class AboutDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit AboutDialog(QWidget* parent = nullptr);
   private:
      void setupGui();
      QLayout* setupTop();
      QWidget* setupText();
      QLayout* setupBottom();
      /*!
       * The width and height of the program's icon displayed in the top left of this 
       * object's GUI. 
       */
      constexpr static int _iconSize {128};
   };
}



#endif
