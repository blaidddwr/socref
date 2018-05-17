#ifndef GUI_TEXTEDIT_HIGHLIGHTER_H
#define GUI_TEXTEDIT_HIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include "gui_textedit.h"



class AspellConfig;
class AspellSpeller;
//



namespace Gui
{
   /*!
    * This is the spell checking syntax highlighter for its parent text editor class. 
    * This highlights any misspelled words in its parent editor by changing its text 
    * format so it is visible to the user. 
    */
   class TextEdit::Highlighter : public QSyntaxHighlighter
   {
      Q_OBJECT
   public:
      explicit Highlighter(QTextDocument* parent);
      ~Highlighter();
   protected:
      virtual void highlightBlock(const QString& text) override final;
   private:
      void setupSpeller();
      /*!
       * The text format used by this highlighter for any misspelled words in its parent 
       * editor. 
       */
      QTextCharFormat _format;
      /*!
       * Pointer to this highlighter's Aspell configuration resource. 
       */
      AspellConfig* _spellConfig;
      /*!
       * Pointer to this highlighter's Aspell speller resource. 
       */
      AspellSpeller* _spell;
   };
}



#endif
