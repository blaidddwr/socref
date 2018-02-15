#ifndef GUI_TEXTDIALOG_H
#define GUI_TEXTDIALOG_H
#include <QDialog>
#include "gui.h"



namespace Gui
{
   class TextDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit TextDialog(QWidget* parent = nullptr);
      virtual ~TextDialog() override final;
      QString text() const;
      void setText(const QString& text);
   private:
      void restoreSettings();
      void saveSettings();
      void setupGui();
      QLayout* setupButtons();
      static const char* _geometryKey;
      TextEdit* _edit;
   };
}



#endif
