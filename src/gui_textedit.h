#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H
#include <QPlainTextEdit>
//



namespace Gui
{
   /*!
    */
   class TextEdit : public QPlainTextEdit
   {
      Q_OBJECT
   public:
      explicit TextEdit(QWidget* parent = nullptr);
      bool isSpellCheckEnabled() const;
      void setSpellCheckEnabled(bool enabled);
      bool isDialogPopupEnabled() const;
      void setDialogPopupEnabled(bool enabled);
   protected:
      virtual void keyPressEvent(QKeyEvent* event) override final;
   private:
      class Highlighter;
      class Dialog;
      /*!
       */
      static const char* _defaultLang;
      /*!
       */
      bool _spellCheckEnabled {true};
      /*!
       */
      bool _dialogPopupEnabled {true};
      /*!
       */
      Highlighter* _spellHighlighter;
   };
}



#endif
