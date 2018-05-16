#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H
#include <QPlainTextEdit>
//



namespace Gui
{
   /*!
    * This is a plain text editor widget that has additional features to the Qt 
    * version. This editor has spellchecking support and the ability to open a new 
    * text editor dialog with the same text. The spellchecking support comes in two 
    * ways. The first way is highlighting any misspelled words in the editor. The 
    * second way is a spell checker dialog that scans all words of the text allowing 
    * for corrections to any misspelled words. A special shortcut is used to activate 
    * the spell checker dialog or open the editor into its own dialog. 
    */
   class TextEdit : public QPlainTextEdit
   {
      Q_OBJECT
   public:
      explicit TextEdit(QWidget* parent = nullptr);
      bool isSpellCheckEnabled() const;
      bool isDialogPopupEnabled() const;
      void setSpellCheckEnabled(bool enabled);
      void setDialogPopupEnabled(bool enabled);
   private slots:
      void spellCheckTriggered();
      void dialogPopupTriggered();
   private:
      class Highlighter;
      class Dialog;
      void setupActions();
      /*!
       * The default language used by this editor's spell checking library Aspell. 
       */
      static const char* _defaultLang;
      /*!
       * True if spell checking is enabled for this editor or false otherwise. 
       */
      bool _spellCheckEnabled {true};
      /*!
       * True if the dialog popup shortcut is enabled for this editor or false otherwise. 
       */
      bool _dialogPopupEnabled {true};
      /*!
       * Pointer to the spell checking highlighter for this editor. This is null if spell 
       * checking is disabled. 
       */
      Highlighter* _spellHighlighter;
   };
}



#endif
