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
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize the properties of this highlighter's text format used for 
 *    misspelled words. 
 *
 * 2. Setup this highlighter's spell checking library. 
 */
TextEdit::Highlighter::Highlighter(QTextDocument* parent):
   QSyntaxHighlighter(parent)
{
   // 1
   _format.setFontUnderline(true);
   _format.setUnderlineColor(Qt::red);
   _format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

   // 2
   setupSpeller();
}






/*!
 * Delete this highlighter's spell checking library resources. 
 */
TextEdit::Highlighter::~Highlighter()
{
   // 1
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






/*!
 * Implements the Qt interface that is called for highlighting blocks of text in 
 * its parent text document. This implementation searches for misspelled words, 
 * highlighting any that are found. 
 *
 * @param text The block of text in this highlighter's parent text document that is 
 *             highlighted. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Use a Qt regular expression to search for all words in the given text, saving 
 *    all matches to _matches_. 
 *
 * 2. Iterate through all matches in _matches_, checking that each word is spelled 
 *    correctly. If any misspelled words are found use this highlighter's special 
 *    text format on those words to highlight them to the user. 
 */
void TextEdit::Highlighter::highlightBlock(const QString& text)
{
   // 1
   QRegularExpression pattern("[\\w'-]+");
   QRegularExpressionMatchIterator matches {pattern.globalMatch(text)};

   // 2
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






/*!
 * Constructs and initializes all Aspell library resources for this new 
 * highlighter. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize this highlighter's Aspell configuration, setting its 
 *    default language, and then create a temporary can have errors Aspell speller 
 *    _temp_. If _temp_ has errors then throw an exception. 
 *
 * 2. Set this highlighter's speller by extracting it from _temp_. 
 */
void TextEdit::Highlighter::setupSpeller()
{
   // 1
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

   // 2
   _spell = to_aspell_speller(temp);
}
