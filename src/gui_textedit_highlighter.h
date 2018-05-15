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
       */
      QTextCharFormat _format;
      /*!
       */
      AspellConfig* _spellConfig;
      /*!
       */
      AspellSpeller* _spell;
   };
}



#endif
