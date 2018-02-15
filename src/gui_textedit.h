#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H
#include <QPlainTextEdit>



namespace Gui
{
   class TextEdit : public QPlainTextEdit
   {
      Q_OBJECT
   public:
      TextEdit(QWidget* parent = nullptr);
      bool isSpellCheckEnabled() const;
      void setSpellCheckEnabled(bool enabled);
      bool isDialogPopupEnabled() const;
      void setDialogPopupEnabled(bool enabled);
   protected:
      class Highlighter;
      class Dialog;
      virtual void keyPressEvent(QKeyEvent* event) override final;
      bool _spellCheckEnabled {true};
      bool _dialogPopupEnabled {true};
      Highlighter* _spellHighliter;
   private:
      static const char* _defaultLang;
   };
}



#endif
