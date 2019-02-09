#ifndef GUI_SPELLERHIGHLIGHTER_H
#define GUI_SPELLERHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include "global.h"



class AspellConfig;
class AspellSpeller;
//



namespace Gui
{
   /*!
    * This is a spell checking syntax highlighter for its given qt text document. This 
    * highlights any misspelled words in its given qt text document by changing its 
    * text format so it is visible to the user. 
    */
   class SpellerHighlighter : public QSyntaxHighlighter
   {
      Q_OBJECT
   public:
      explicit SpellerHighlighter(DictionaryModel* dictionary, const char* language, QTextDocument* document);
      virtual ~SpellerHighlighter() override final;
   protected:
      virtual void highlightBlock(const QString& text) override final;
   private:
      void setupSpeller(const char* language);
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
      /*!
       * Pointer to the custom dictionary model used for custom spell checking words. 
       */
      DictionaryModel* _dictionary;
   };
}



#endif
