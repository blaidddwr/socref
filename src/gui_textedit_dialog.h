#ifndef GUI_TEXTEDIT_DIALOG_H
#define GUI_TEXTEDIT_DIALOG_H
#include "gui_persistentdialog.h"
#include "gui_textedit.h"



class QLineEdit;
class QLabel;
class AspellConfig;
class AspellSpeller;
//



namespace Gui
{
   /*!
    * This is the spell checking dialog for its parent text editor. This dialog scans 
    * the text of its parent editor and presents a dialog for each misspelled word it 
    * finds. This dialog provides the user options to change, ignore once, or ignore 
    * all for each misspelled word it finds. It also provides suggestions for changing 
    * the misspelled words it finds. Lastly it provides a snippet of the text around 
    * the misspelled word to provide context to the user. This is a persistent dialog 
    * and remembers its geometry. 
    * 
    * Once the entire text of the parent editor of this dialog has been scanned, it 
    * will ask the user if they wish to scan the entire text again. A cancel button is 
    * also provided for the user to cancel the spell checking at anytime. 
    */
   class TextEdit::Dialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit Dialog(TextEdit* parent);
      virtual ~Dialog() override final;
      virtual int exec() override final;
   private slots:
      void changeClicked();
      void ignoreOnceClicked();
      void ignoreAllClicked();
      void wordTextChanged(const QString& text);
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
      void setupSpeller();
      /*!
       * The amount of characters displayed before and after the misspelled word in the 
       * snippet view of spell checkers. 
       */
      constexpr static int _snippetRadius {256};
      /*!
       * The text added before and after a snippet view representing dots to communicate 
       * there is more text on either side. 
       */
      static const char* _snippetDots;
      /*!
       * The current misspelled word for this dialog. 
       */
      QString _currentWord;
      /*!
       * List of words that are ignored and not flagged as misspelled even if the spell 
       * checker says so. 
       */
      QStringList _ignored;
      /*!
       * Pointer to the parent text editor this dialog is spell checking. 
       */
      TextEdit* _edit;
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
       * Pointer to the word edit widget of this dialog where the user can edit and fix 
       * misspelled words. 
       */
      QLineEdit* _wordEdit;
      /*!
       * Pointer to the snippet view label of this dialog which displays a portion of the 
       * text around the current misspelled. 
       */
      QLabel* _snippetView;
      /*!
       * Pointer to the suggestions view label of this dialog that displayed possible 
       * corrections to the current misspelled word. 
       */
      QLabel* _suggestionsView;
      /*!
       * Pointer to the change button of this dialog. 
       */
      QPushButton* _changeButton;
      /*!
       * Pointer the Aspell configuration resource of this dialog. 
       */
      AspellConfig* _spellConfig;
      /*!
       * Pointer the Aspell speller resource of this dialog. 
       */
      AspellSpeller* _spell;
   };
}



#endif
