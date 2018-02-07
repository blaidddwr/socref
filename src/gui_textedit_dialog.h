#ifndef GUI_TEXTEDIT_DIALOG_H
#define GUI_TEXTEDIT_DIALOG_H
#include <QDialog>
#include "gui_textedit.h"



class QLineEdit;
class QLabel;
class AspellConfig;
class AspellSpeller;



namespace Gui
{
   class TextEdit::Dialog : public QDialog
   {
   public:
      Dialog(TextEdit* parent);
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
      void setWord(const QString& text, const QRegularExpressionMatch& match);
      void clearWord();
      void setSnippet(const QString& text, int begin, int length);
      void updateSuggested();
      QLayout* setupTop();
      QLayout* setupButtons();
      QLayout* setupWordEdit();
      constexpr static int _snippetRadius {256};
      const char* _defaultLang {"en_US"};
      const char* _snippetDots {"..."};
      QString _currentWord;
      QStringList _ignored;
      TextEdit* _edit;
      int _offset {0};
      int _length {0};
      QLineEdit* _wordEdit;
      QLabel* _snippetView;
      QLabel* _suggestionsView;
      QPushButton* _changeButton;
      AspellConfig* _spellConfig;
      AspellSpeller* _spell;
   };
}



#endif
