#include "gui_textedit_highlighter.h"
#include <aspell.h>
#include <QRegularExpression>
#include <exception.h>



using namespace Gui;
//






/*!
 * Constructs a new spell checking highlighter with the given text document parent. 
 *
 * @param parent The text document of the text editor that is the parent for this 
 *               new highlighter. 
 */
TextEdit::Highlighter::Highlighter(QTextDocument* parent):
   QSyntaxHighlighter(parent)
{
   // Initialize the highlight format for misspelled words. 
   _format.setFontUnderline(true);
   _format.setUnderlineColor(Qt::red);
   _format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

   // Setup this highlighter's spell checking library. 
   setupSpeller();
}






/*!
 * Delete this highlighter's spell checking library resources. 
 */
TextEdit::Highlighter::~Highlighter()
{
   // Free this highlighter's Aspell resources. 
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






/*!
 * Implements _QSyntaxHighlighter_ interface. This implementation searches for 
 * misspelled words, highlighting any that are found. 
 *
 * @param text See Qt docs. 
 */
void TextEdit::Highlighter::highlightBlock(const QString& text)
{
   // Use a Qt regular expression to match all words in the given text block. 
   QRegularExpression pattern("[\\w'-]+");
   QRegularExpressionMatchIterator matches {pattern.globalMatch(text)};

   // Iterate through all matched words. 
   while ( matches.hasNext() )
   {
      // Extract the matched word and check to see if it it misspelled. 
      QRegularExpressionMatch match {matches.next()};
      QByteArray word {match.captured().toLocal8Bit()};
      if ( !aspell_speller_check(_spell,word.data(),word.size()) )
      {
         // Highlight the misspelled word. 
         setFormat(match.capturedStart(),match.capturedLength(),_format);
      }
   }
}






/*!
 * Constructs and initializes all Aspell library resources for this new 
 * highlighter. 
 */
void TextEdit::Highlighter::setupSpeller()
{
   // Create and set this highlighter's Aspell configuration. 
   _spellConfig = new_aspell_config();
   aspell_config_replace(_spellConfig,"lang",_defaultLang);

   // Create this highlighter's Aspell speller using the configuration and make sure 
   // it worked. 
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

   // Set this highlighter's speller by extracting it from the temporary holder. 
   _spell = to_aspell_speller(temp);
}
