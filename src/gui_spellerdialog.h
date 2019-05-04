#ifndef GUI_SPELLERDIALOG_H
#define GUI_SPELLERDIALOG_H
#include "gui_persistentdialog.h"
#include "global.h"
class QTextDocument;
class QLineEdit;
class QLabel;
class AspellConfig;
class AspellSpeller;



/*!
 * Contains all GUI elements for this application.
 */
namespace Gui
{
   /*!
    * This is a spell checking dialog for the given qt text document. This dialog
    * scans the text of the given qt text document and presents a dialog for each
    * misspelled word it finds. This dialog provides the user options to change,
    * ignore once, or ignore all for each misspelled word it finds. It also
    * provides suggestions for changing the misspelled words it finds. Lastly it
    * provides a snippet of the text around the misspelled word to provide context
    * to the user. This is a persistent dialog and remembers its geometry.
    *
    * Once the entire text of the parent editor of this dialog has been scanned, it
    * will ask the user if they wish to scan the entire text again. A cancel button
    * is also provided for the user to cancel the spell checking at anytime.
    */
   class SpellerDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit SpellerDialog(DictionaryModel* dictionary, const char* language, QTextDocument* document, QWidget* parent);
      virtual ~SpellerDialog() override final;
      virtual int exec() override final;
   private slots:
      void changeClicked();
      void ignoreOnceClicked();
      void ignoreAllClicked();
      void addClicked();
      void wordTextChanged(const QString& text);
   private:
      /*!
       * The amount of characters displayed before and after the misspelled word in
       * the snippet view of spell checkers.
       */
      constexpr static int _snippetRadius {256};
      /*!
       * The text added before and after a snippet view representing dots to
       * communicate there is more text on either side.
       */
      static const char* _snippetDots;
   private:
      bool findNextWord();
      bool isCorrectWord(const QString& word);
      bool queryEndOfText();
      void setWord(const QRegularExpressionMatch& match);
      void setSnippet(int begin, int length);
      void updateSuggested();
      void setupGui();
      QLayout* setupTop();
      QLayout* setupWordEdit();
      QLayout* setupButtons();
      void setupSpeller(const char* language);
      /*!
       * The qt text document this dialog is spell checking.
       */
      QTextDocument* _document;
      /*!
       * The custom dictionary model used by this spell checker for custom words.
       */
      DictionaryModel* _dictionary;
      /*!
       * The current misspelled word for this dialog.
       */
      QString _currentWord;
      /*!
       * List of words that are ignored and not flagged as misspelled even if the
       * spell checker says so.
       */
      QStringList _ignored;
      /*!
       * Current position in the parent text editor text where this dialog is checking
       * for misspelled words.
       */
      int _offset {0};
      /*!
       * Length of the current misspelled word for this dialog.
       */
      int _length {0};
      /*!
       * The word edit widget of this dialog where the user can edit and fix
       * misspelled words.
       */
      QLineEdit* _wordEdit;
      /*!
       * The snippet view label of this dialog which displays a portion of the text
       * around the current misspelled.
       */
      QLabel* _snippetView;
      /*!
       * The suggestions view label of this dialog that displays possible corrections
       * to the current misspelled word.
       */
      QLabel* _suggestionsView;
      /*!
       * The change button of this dialog.
       */
      QPushButton* _changeButton;
      /*!
       * The Aspell configuration resource of this dialog.
       */
      AspellConfig* _spellConfig;
      /*!
       * The Aspell speller resource of this dialog.
       */
      AspellSpeller* _spell;
   };
}

#endif
