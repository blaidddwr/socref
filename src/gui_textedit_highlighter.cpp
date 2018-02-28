#include <aspell.h>
#include <QRegularExpression>
#include "gui_textedit_highlighter.h"
#include "exception.h"



using namespace Gui;






TextEdit::Highlighter::Highlighter(QTextDocument* parent):
   QSyntaxHighlighter(parent)
{
   setupSpeller();
   _format.setFontUnderline(true);
   _format.setUnderlineColor(Qt::red);
   _format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
}






TextEdit::Highlighter::~Highlighter()
{
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






void TextEdit::Highlighter::highlightBlock(const QString& text)
{
   QRegularExpression pattern("[\\w'-]+");
   QRegularExpressionMatchIterator matches {pattern.globalMatch(text)};
   while ( matches.hasNext() )
   {
      QRegularExpressionMatch match {matches.next()};
      QByteArray word {match.captured().toLocal8Bit()};
      if ( !aspell_speller_check(_spell,word.data(),word.size()) )
      {
         setFormat(match.capturedStart(),match.capturedLength(),_format);
      }
   }
}






void TextEdit::Highlighter::setupSpeller()
{
   _spellConfig = new_aspell_config();
   aspell_config_replace(_spellConfig,"lang",_defaultLang);
   AspellCanHaveError* temp {new_aspell_speller(_spellConfig)};
   if ( aspell_error_number(temp) )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Failed initializing Aspell library for spell checking: %1")
               .arg(aspell_error_message(temp)));
      throw e;
   }
   _spell = to_aspell_speller(temp);
}
