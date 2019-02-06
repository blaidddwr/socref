#include "gui_textedit_dialog.h"
#include <aspell.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QMessageBox>
#include <QSettings>
#include "application.h"
#include "dictionarymodel.h"



using namespace Gui;
//



/*!
 * The text added before and after a snippet view representing dots to communicate 
 * there is more text on either side. 
 */
const char* TextEdit::Dialog::_snippetDots {"..."};






/*!
 * Constructs a new spell checker dialog with the given text editor parent. 
 *
 * @param parent The parent text edit for this new spell checking dialog. 
 */
TextEdit::Dialog::Dialog(TextEdit* parent):
   PersistentDialog("gui.textedit.dialog.geometry",parent),
   _edit(parent)
{
   // Setup the speller, GUI, and window title for this new dialog. 
   setupSpeller();
   setupGui();
   setWindowTitle("Spell Checker");
}






/*!
 * Delete this object's spell checking library resources. 
 */
TextEdit::Dialog::~Dialog()
{
   // Free the speller resources of this dialog. 
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






/*!
 * Begins spell checking on the parent text editor of this dialog by finding the 
 * first misspelled word. If no misspelled word are found this returns. 
 *
 * @return Always returns Qt dialog accept. 
 */
int TextEdit::Dialog::exec()
{
   // Show this dialog so it is not invisible. 
   show();

   // Find the first misspelled word, executing this dialog if one is found or 
   // returning right away if no misspelled word is found. 
   if ( findNextWord() ) return QDialog::exec();
   else return QDialog::Accepted;
}






/*!
 * Called when the change button is clicked. This changes the current misspelled 
 * word in the parent editor's text of this dialog and then finds the next 
 * misspelled word. 
 */
void TextEdit::Dialog::changeClicked()
{
   // Replace the currently misspelled word with the text from the word edit widget 
   // of this dialog. 
   QString text {_edit->toPlainText()};
   text.remove(_offset,_length);
   text.insert(_offset,_wordEdit->text());
   _edit->setPlainText(text);

   // Find the next misspelled word. 
   findNextWord();
}






/*!
 * Called when the ignore once button is clicked. This skips the current misspelled 
 * word and finds the next one. 
 */
void TextEdit::Dialog::ignoreOnceClicked()
{
   // Skip the current misspelled word and find the next one. 
   _offset += _length;
   findNextWord();
}






/*!
 * Called when the ignore all button is clicked. This adds this object's current 
 * misspelled word to the list of words to ignore and finds the next misspelled 
 * word. 
 */
void TextEdit::Dialog::ignoreAllClicked()
{
   // Add the current misspelled word to the list of ignored words for this dialog 
   // and then find the next misspelled word. 
   _ignored << _currentWord;
   findNextWord();
}






/*!
 * Called when the add button is clicked. This adds this object's current 
 * misspelled word to this object's text edit's custom dictionary. 
 */
void TextEdit::Dialog::addClicked()
{
   // Add the current misspelled word to this object's text edit's custom dictionary 
   // and then find the next misspelled word. 
   _edit->_dictionary->addWord(_currentWord);
   findNextWord();
}






/*!
 * Called when the text in the word edit widget of this dialog changes. This 
 * updates the enabled state of the change button of this dialog depending on if 
 * the new text is a misspelled word or not, disabling it if it is misspelled. 
 *
 * @param text New text in the word edit widget of this dialog. 
 */
void TextEdit::Dialog::wordTextChanged(const QString& text)
{
   // Change the enabled state of the word edit widget of this dialog based off it 
   // being misspelled or not. 
   _changeButton->setDisabled(!isCorrectWord(text));
}






/*!
 * Finds the next misspelled word in the parent editor of this dialog, returning 
 * true if a misspelled word is found or false if there is nothing else to find. 
 *
 * @return True if a misspelled word is found or false otherwise. 
 */
bool TextEdit::Dialog::findNextWord()
{
   // Find the first word of the parent editor's text at or after the offset of this 
   // dialog. 
   QString text {_edit->toPlainText()};
   QRegularExpression pattern("[^\\s\\t:,._]+");
   QRegularExpressionMatch match {pattern.match(text,_offset)};

   // While a word is found at or after the offset of this dialog then continue. 
   while ( match.hasMatch() )
   {
      // Update the offset and length parameters of this dialog with the current word 
      // match. 
      _offset = match.capturedStart();
      _length = match.capturedLength();

      // Check to see if the current word is misspelled. 
      if ( !isCorrectWord(match.captured()) )
      {
         // Update the current misspelled word of this dialog to the new one found and 
         // return true. 
         setWord(match);
         return true;
      }

      // Update the offset for this dialog to find the next word in the parent editor's 
      // text. 
      _offset += _length;
      match = pattern.match(text,_offset);
   }

   // End of text is reached so query the user if they want to start over from the 
   // beginning. 
   return queryEndOfText();
}






/*!
 * Tests if the given word is spelled correctly or not. 
 *
 * @param word The word that is checked for spelling. 
 *
 * @return True if the given word is spelled correctly or false otherwise. 
 */
bool TextEdit::Dialog::isCorrectWord(const QString& word)
{
   // Check to see if the given word is in the ignore list of this dialog. 
   if ( _ignored.contains(word) ) return true;

   // Check to see if the given word is in the custom dictionary. 
   if ( _edit->_dictionary->hasWord(word) ) return true;

   // Use the spell checker of this dialog to see and return if the given word is 
   // spelled correctly. 
   QByteArray utfWord {word.toLocal8Bit()};
   return aspell_speller_check(_spell,utfWord.data(),utfWord.size());
}






/*!
 * Queries the user with a dialog if they want to run spell checking again on the 
 * parent editor of this dialog. 
 *
 * @return True if the user wants to run spell checking again and a misspelled word 
 *         was found or false otherwise. 
 */
bool TextEdit::Dialog::queryEndOfText()
{
   // Create a message box to query the user if they want to spell check again from 
   // the beginning. 
   QMessageBox box;
   box.setWindowTitle(tr("Spell Check Complete"));
   box.setText(tr("End of text has been reached. Do you want to run spell check again from beginning of text?"));
   box.addButton(tr("No"),QMessageBox::RejectRole);
   box.addButton(tr("Yes"),QMessageBox::AcceptRole);

   // Execute the message box and check to see if the user choose yes. 
   if ( box.exec() )
   {
      // Reset the offset and length of this dialog and find the next misspelled word. 
      _offset = 0;
      _length = 0;
      return findNextWord();
   }

   // Else the user responded with no. 
   else
   {
      // Close the dialog with accept and return false. 
      accept();
      return false;
   }
}






/*!
 * Sets the current misspelled word for this dialog. 
 *
 * @param match The Qt regular expression match within the given complete text that 
 *              is a misspelled word. 
 */
void TextEdit::Dialog::setWord(const QRegularExpressionMatch& match)
{
   // Set the misspelled word for this dialog. 
   _currentWord = match.captured();
   _wordEdit->setText(_currentWord);

   // Set the snippet and suggested word replacements for this dialog. 
   setSnippet(match.capturedStart(),match.capturedLength());
   updateSuggested();
}






/*!
 * Set the snippet text for this dialog with the given text and offset positions 
 * for the misspelled word. 
 *
 * @param begin The offset where the misspelled word begins in the given complete 
 *              text. 
 *
 * @param length The length of the misspelled word in the given complete text. 
 */
void TextEdit::Dialog::setSnippet(int begin, int length)
{
   // Get the text of the parent editor for this dialog. 
   QString text {_edit->toPlainText()};

   // Figure out the beginning offset and length for the left half of the new 
   // snippet. 
   int begin1 {begin - _snippetRadius};
   if ( begin1 < 0 ) begin1 = 0;
   int length1 {begin - begin1};

   // Figure out the offset for the right half of the new snippet. 
   int begin2 {begin + length};

   // Add snippet dots to the new snippet if it is not at the beginning of the parent 
   // editor's text. 
   QString snippet;
   if ( begin1 > 0 ) snippet.append(_snippetDots);

   // Append the left side, misspelled word, and then right side to the new snippet. 
   snippet.append(text.mid(begin1,length1));
   snippet.append("<b>").append(text.mid(begin,length)).append("</b>");
   snippet.append(text.mid(begin2,_snippetRadius));

   // Add snippets dots to the new snippet if it is not at the end of the parent 
   // editor's text. 
   if ( (begin2 + _snippetRadius) < text.length() ) snippet.append(_snippetDots);

   // Set the snippet view of this dialog to the new snippet. 
   _snippetView->setText(snippet);
}






/*!
 * Update the word change suggestions displayed to the user for the current 
 * misspelled word of this dialog. 
 */
void TextEdit::Dialog::updateSuggested()
{
   // Get a list of word change suggestions from the current misspelled word of this 
   // dialog. 
   QByteArray utfWord {_currentWord.toLocal8Bit()};
   const AspellWordList* suggestions {aspell_speller_suggest(_spell,utfWord.data(),utfWord.size())};
   AspellStringEnumeration* elements {aspell_word_list_elements(suggestions)};

   // Initialize for iterating through suggestions and add the title. 
   bool first {true};
   const char* suggest;
   QString text{"<h3>Suggestions</h3>"};

   // Iterate through all word change suggestions. 
   while ( (suggest = aspell_string_enumeration_next(elements)) != nullptr )
   {
      // If this is not the first word change suggestion then append a space. 
      if ( first ) first = false;
      else text.append(" ");

      // Append the word change suggestion. 
      text.append(suggest);
   }

   // Set the suggestion view of this dialog with the new suggestion list and delete 
   // temporary Aspell resources. 
   _suggestionsView->setText(text);
   delete_aspell_string_enumeration(elements);
}






/*!
 * Constructs and initializes all GUI elements for this new dialog. 
 */
void TextEdit::Dialog::setupGui()
{
   // Create a new vertical layout, adding the top layout and then adding the 
   // buttons. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupButtons());

   // Set the layout of this dialog. 
   setLayout(layout);
}






/*!
 * Constructs and initializes the top part of the GUI for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to the layout of the top part of the GUI for this dialog. 
 */
QLayout* TextEdit::Dialog::setupTop()
{
   // Create and initialize the suggestions view label of this dialog. 
   _suggestionsView = new QLabel;
   _suggestionsView->setAlignment(Qt::AlignTop);
   _suggestionsView->setStyleSheet("margin-left:16px;");
   _suggestionsView->setWordWrap(true);

   // Create a new horizontal layout, adding the word edit layout and then the 
   // suggestions view label. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addLayout(setupWordEdit());
   ret->addWidget(_suggestionsView);

   // Return the top layout. 
   return ret;
}






/*!
 * Constructs and initializes the word edit portion of the GUI for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to the layout of the word edit portion of the GUI for this 
 *         dialog. 
 */
QLayout* TextEdit::Dialog::setupWordEdit()
{
   // Create the word line edit widget for this new dialog, connecting its text 
   // changed signal. 
   _wordEdit = new QLineEdit;
   connect(_wordEdit,&QLineEdit::textChanged,this,&Dialog::wordTextChanged);

   // Create and initialize the snippet view label for this new dialog. 
   _snippetView = new QLabel;
   _snippetView->setWordWrap(true);
   _snippetView->setAlignment(Qt::AlignTop);
   _snippetView->setTextFormat(Qt::RichText);

   // Create a vertical layout, adding the word edit widget then the snippet view 
   // label. 
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_wordEdit);
   ret->addWidget(_snippetView);

   // Return the word edit layout. 
   return ret;
}






/*!
 * Constructs and initializes all buttons for this new dialog, returning the layout 
 * containing all the buttons. 
 *
 * @return Pointer to layout of all buttons for this new dialog. 
 */
QLayout* TextEdit::Dialog::setupButtons()
{
   // Create the change button for this dialog, connecting its clicked signal. 
   _changeButton = new QPushButton(tr("&Change"));
   connect(_changeButton,&QPushButton::clicked,this,&Dialog::changeClicked);

   // Create the ignore once button for this dialog, connecting its clicked signal. 
   QPushButton* ignoreOnce {new QPushButton(tr("&Ignore Once"))};
   connect(ignoreOnce,&QPushButton::clicked,this,&Dialog::ignoreOnceClicked);

   // Create the ignore all button for this dialog, connecting its clicked signal. 
   QPushButton* ignoreAll {new QPushButton(tr("Ignore &All"))};
   connect(ignoreAll,&QPushButton::clicked,this,&Dialog::ignoreAllClicked);

   // Create the add button for this dialog, connecting its clicked signal. 
   QPushButton* add {new QPushButton(tr("Add"))};
   connect(add,&QPushButton::clicked,this,&Dialog::addClicked);

   // Create the cancel button for this dialog, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("Canc&el"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Accepted); });

   // Create a new horizontal layout, adding the change button then the ignore once 
   // button then the ignore button then the add button then a stretch and then the 
   // cancel button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_changeButton);
   ret->addWidget(ignoreOnce);
   ret->addWidget(ignoreAll);
   ret->addWidget(add);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return the buttons layout. 
   return ret;
}






/*!
 * Constructs and initializes all Aspell library resources for this new dialog. 
 */
void TextEdit::Dialog::setupSpeller()
{
   // Create and set the Aspell configuration of this dialog. 
   _spellConfig = new_aspell_config();
   aspell_config_replace(_spellConfig,"lang",_defaultLang);

   // Create the Aspell speller for this dialog using the configuration and make sure 
   // it worked. 
   AspellCanHaveError* temp {new_aspell_speller(_spellConfig)};
   Q_ASSERT(!aspell_error_number(temp));

   // Set the speller of this dialog by extracting it from the temporary holder. 
   _spell = to_aspell_speller(temp);
}
