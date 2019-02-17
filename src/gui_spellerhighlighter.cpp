#include "gui_spellerhighlighter.h"
#include <aspell.h>
#include <QRegularExpression>
#include "dictionarymodel.h"



namespace Gui
{






/*!
 * Constructs a new spell checking highlighter with the given text document
 * parent.
 *
 * @param dictionary The custom dictionary model this new highlighter uses to
 *                   check for custom spell checking words.
 *
 * @param language The language used with this highlighter's spell checking
 *                 engine.
 *
 * @param document The text document of the text editor that is the parent for
 *                 this new highlighter.
 */
SpellerHighlighter::SpellerHighlighter(DictionaryModel* dictionary, const char* language, QTextDocument* document)
   :
   QSyntaxHighlighter(document),
   _dictionary(dictionary)
{
   // Make sure the given dictionary, language, and document pointers are valid.
   Q_CHECK_PTR(dictionary);
   Q_CHECK_PTR(language);
   Q_CHECK_PTR(document);

   // Initialize the highlight format for misspelled words.
   _format.setFontUnderline(true);
   _format.setUnderlineColor(Qt::red);
   _format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

   // Setup this highlighter's spell checking library.
   setupSpeller(language);
}






/*!
 * Delete this highlighter's spell checking library resources.
 */
SpellerHighlighter::~SpellerHighlighter()
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
void SpellerHighlighter::highlightBlock(const QString& text)
{
   // Use a Qt regular expression to match all words in the given text block.
   QRegularExpression pattern("[^\\s\\t:,._]+");
   QRegularExpressionMatchIterator matches {pattern.globalMatch(text)};

   // Iterate through all matched words.
   while ( matches.hasNext() )
   {
      // Extract the matched word.
      QRegularExpressionMatch match {matches.next()};
      QByteArray word {match.captured().toLocal8Bit()};

      // Check to see if the word is misspelled by checking the custom dictionary and
      // then the Aspell spell checker.
      if ( !_dictionary->hasWord(match.captured())
           && !aspell_speller_check(_spell,word.data(),word.size()) )
      {
         // Highlight the misspelled word.
         setFormat(match.capturedStart(),match.capturedLength(),_format);
      }
   }
}






/*!
 * Constructs and initializes all Aspell library resources for this new
 * highlighter using the given language.
 *
 * @param language The language used with this highlighter's spell checking
 *                 engine.
 */
void SpellerHighlighter::setupSpeller(const char* language)
{
   // Create and set this highlighter's Aspell configuration.
   _spellConfig = new_aspell_config();
   aspell_config_replace(_spellConfig,"lang",language);

   // Create this highlighter's Aspell speller using the configuration and make sure
   // it worked.
   AspellCanHaveError* temp {new_aspell_speller(_spellConfig)};
   Q_ASSERT(!aspell_error_number(temp));

   // Set this highlighter's speller by extracting it from the temporary holder.
   _spell = to_aspell_speller(temp);
}

}
