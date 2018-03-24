#ifndef GUI_TEXTDIALOG_H
#define GUI_TEXTDIALOG_H
#include "gui_persistentdialog.h"
#include "gui.h"



namespace Gui
{
   class TextDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit TextDialog(QWidget* parent = nullptr);
      QString text() const;
      void setText(const QString& text);
   private:
      void setupGui();
      QLayout* setupButtons();
      TextEdit* _edit;
   };
}



#endif
