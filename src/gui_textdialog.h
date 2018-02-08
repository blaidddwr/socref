#ifndef GUI_TEXTDIALOG_H
#define GUI_TEXTDIALOG_H
#include <QDialog>
#include "gui.h"



namespace Gui
{
   class TextDialog : public QDialog
   {
   public:
      TextDialog(QWidget* parent = nullptr);
      QString text() const;
      void setText(const QString& text);
   private:
      void setupGui();
      QLayout* setupButtons();
      TextEdit* _edit;
   };
}



#endif
