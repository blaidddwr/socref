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
#include <exception.h>
#include "application.h"



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
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this object's spell library resources, its GUI, and set it's window 
 *    title. 
 */
TextEdit::Dialog::Dialog(TextEdit* parent):
   PersistentDialog("gui.textedit.dialog.geometry",parent),
   _edit(parent)
{
   // 1
   setupSpeller();
   setupGui();
   setWindowTitle("Spell Checker");
}






/*!
 * Delete this object's spell checking library resources. 
 */
TextEdit::Dialog::~Dialog()
{
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






/*!
 * Begins spell checking on the parent text editor of this dialog by finding the 
 * first misspelled word. If no misspelled word are found this returns. 
 *
 * @return Always returns Qt dialog accept. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Show this object's GUI and then find the first misspelled word. If a 
 *    misspelled word is found then call the Qt dialog exec method, else return Qt 
 *    accept. 
 */
int TextEdit::Dialog::exec()
{
   // 1
   show();
   if ( findNextWord() ) return QDialog::exec();
   return QDialog::Accepted;
}






/*!
 * Called when the change button is clicked. This changes the current misspelled 
 * word in the parent editor's text of this dialog and then finds the next 
 * misspelled word. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Replace the current misspelled word in the parent editor's text of this 
 *    dialog with the text from this object's word edit widget. 
 *
 * 2. Find the next misspelled word in the parent text editor of this dialog. 
 */
void TextEdit::Dialog::changeClicked()
{
   // 1
   QString text {_edit->toPlainText()};
   text.remove(_offset,_length);
   text.insert(_offset,_wordEdit->text());
   _edit->setPlainText(text);

   // 2
   findNextWord();
}






/*!
 * Called when the ignore once button is clicked. This skips the current misspelled 
 * word and finds the next one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Skip the current misspelled word of the parent editor's text and find the 
 *    next one. 
 */
void TextEdit::Dialog::ignoreOnceClicked()
{
   // 1
   _offset += _length;
   findNextWord();
}






/*!
 * Called when the ignore all button is clicked. This adds this object's current 
 * misspelled word to the list of words to ignore and finds the next misspelled 
 * word. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Add this object's current misspelled word to its list of ignored words and 
 *    then find the next misspelled word. 
 */
void TextEdit::Dialog::ignoreAllClicked()
{
   // 1
   _ignored << _currentWord;
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
   _changeButton->setDisabled(!isCorrectWord(text));
}






/*!
 * Finds the next misspelled word in the parent editor of this dialog, returning 
 * true if a misspelled word is found or false if there is nothing else to find. 
 *
 * @return True if a misspelled word is found or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Grab the text _text_ of this object's parent editor, finding the next word in 
 *    it after this object's text offset. 
 *
 * 2. If a next word is found then check it for spelling and update this object's 
 *    offset and length. If the word is misspelled then set it as this object's 
 *    current misspelled word and return true, else find the next word in this 
 *    object's parent editor. 
 *
 * 3. If no words are found query the user if they want to start again from the 
 *    beginning. 
 */
bool TextEdit::Dialog::findNextWord()
{
   // 1
   QString text {_edit->toPlainText()};
   QRegularExpression pattern("[\\w'-]+");
   QRegularExpressionMatch match {pattern.match(text,_offset)};

   // 2
   while ( match.hasMatch() )
   {
      _offset = match.capturedStart();
      _length = match.capturedLength();
      if ( !isCorrectWord(match.captured()) )
      {
         setWord(text,match);
         return true;
      }
      else _offset += _length;
      match = pattern.match(text,_offset);
   }

   // 3
   return queryEndOfText();
}






/*!
 * Tests if the given word is spelled correctly or not. 
 *
 * @param word  
 *
 * @return True if the given word is spelled correctly or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given word is contained in this object's ignore list then return true. 
 *
 * 2. Use this object's Aspell speller to check for correct spelling, returning 
 *    true if it is correct or false if it is misspelled. 
 */
bool TextEdit::Dialog::isCorrectWord(const QString& word)
{
   // 1
   if ( _ignored.contains(word) ) return true;

   // 2
   QByteArray utfWord {word.toLocal8Bit()};
   return aspell_speller_check(_spell,utfWord.data(),utfWord.size());
}






/*!
 * Queries the user with a new dialog if they want to run spell checking again on 
 * the parent editor of this dialog. 
 *
 * @return True if the user wants to run spell checking again and a misspelled word 
 *         was found or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a Qt message box querying the user if they want to spell check this 
 *    object's parent editor from the beginning again. 
 *
 * 2. Execute _box_ in modal mode. If the user responds with yes then reset this 
 *    object's offset and length and find the next misspelled word, returning the 
 *    result of the find next word method. 
 *
 * 3. Else the user responded with no so call on this dialog to close and return 
 *    false. 
 */
bool TextEdit::Dialog::queryEndOfText()
{
   // 1
   QMessageBox box;
   box.setWindowTitle(tr("Spell Check Complete"));
   box.setText(tr("End of text has been reached. Do you want to run spell check again from beginning of text?"));
   box.addButton(tr("No"),QMessageBox::RejectRole);
   box.addButton(tr("Yes"),QMessageBox::AcceptRole);

   // 2
   if ( box.exec() )
   {
      _offset = 0;
      _length = 0;
      return findNextWord();
   }

   // 3
   else
   {
      accept();
      return false;
   }
}






/*!
 * Sets the current misspelled word for this dialog. 
 *
 * @param text The complete text of the parent editor of this dialog. 
 *
 * @param match The Qt regular expression match within the given complete text that 
 *              is a misspelled word. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's current misspelled word and its word edit widget to the 
 *    captured text of the given match. 
 *
 * 2. Set this object's snippet with the given complete text and locations of the 
 *    misspelled word, then update this object's word suggestions. 
 */
void TextEdit::Dialog::setWord(const QString& text, QRegularExpressionMatch& match)
{
   // 1
   _currentWord = match.captured();
   _wordEdit->setText(_currentWord);

   // 2
   setSnippet(text,match.capturedStart(),match.capturedLength());
   updateSuggested();
}






/*!
 * Se the snippet text for this dialog with the given text and offset positions for 
 * the misspelled word. 
 *
 * @param text The complete text of the parent editor of this dialog. 
 *
 * @param begin The offset where the misspelled word begins in the given complete 
 *              text. 
 *
 * @param length The length of the misspelled word in the given complete text. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the starting and end offsets for the text to be displayed before the 
 *    misspelled word. 
 *
 * 2. Get the starting offset for the text to be displayed after the misspelled 
 *    word. 
 *
 * 3. Build the snippet text to _sentence_, adding the snippet dots text before and 
 *    after the built sentence and highlighting the misspelled word as bold. 
 *
 * 4. Set this object's snippet view label text to _sentence_. 
 */
void TextEdit::Dialog::setSnippet(const QString& text, int begin, int length)
{
   // 1
   int begin1 {begin - _snippetRadius};
   int length1 {begin};
   if ( begin1 < 0 ) begin1 = 0;

   // 2
   int begin2 {begin + length};

   // 3
   QString sentence {_snippetDots};
   sentence.append(text.mid(begin1,length1));
   sentence.append("<b>").append(text.mid(begin,length)).append("</b>");
   sentence.append(text.mid(begin2,_snippetRadius)).append(_snippetDots);

   // 4
   _snippetView->setText(sentence);
}






/*!
 * Update the word change suggestions displayed to the user for the current 
 * misspelled word of this dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the list of word change suggestions _elements_ from this object's Aspell 
 *    speller. 
 *
 * 2. Build the word suggestions text _text_, adding the title and then the list of 
 *    suggested words _elements_. 
 *
 * 3. Set this object's suggestions view label text to _text_ and delete the 
 *    temporary Aspell resources needed for generating the suggestion list. 
 */
void TextEdit::Dialog::updateSuggested()
{
   // 1
   QByteArray utfWord {_currentWord.toLocal8Bit()};
   const AspellWordList* suggestions {aspell_speller_suggest(_spell,utfWord.data(),utfWord.size())};
   AspellStringEnumeration* elements {aspell_word_list_elements(suggestions)};

   // 2
   bool first {true};
   const char* suggest;
   QString text{"<h3>Suggestions</h3>"};
   while ( (suggest = aspell_string_enumeration_next(elements)) != nullptr )
   {
      if ( first ) first = false;
      else text.append(" ");
      text.append(suggest);
   }

   // 3
   _suggestionsView->setText(text);
   delete_aspell_string_enumeration(elements);
}






/*!
 * Constructs and initializes all GUI elements for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new vertical layout _layout_, adding the top layout by calling the 
 *    setup top method and then adding the buttons. 
 *
 * 2. Set the layout of this dialog to _layout_. 
 */
void TextEdit::Dialog::setupGui()
{
   // 1
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupButtons());

   // 2
   setLayout(layout);
}






/*!
 * Constructs and initializes the top part of the GUI for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to the layout of the top part of the GUI for this dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize this new object's suggestion view label. 
 *
 * 2. Create a new horizontal layout _ret_, adding this object's word edit layout 
 *    and then its suggestions view label. 
 *
 * 3. Return _ret_. 
 */
QLayout* TextEdit::Dialog::setupTop()
{
   // 1
   _suggestionsView = new QLabel;
   _suggestionsView->setAlignment(Qt::AlignTop);
   _suggestionsView->setStyleSheet("margin-left:16px;");
   _suggestionsView->setWordWrap(true);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addLayout(setupWordEdit());
   ret->addWidget(_suggestionsView);

   // 3
   return ret;
}






/*!
 * Constructs and initializes the word edit portion of the GUI for this new dialog, 
 * returning its layout. 
 *
 * @return Pointer to the layout of the word edit portion of the GUI for this 
 *         dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the word line edit widget for new dialog, connecting its text changed 
 *    signal. 
 *
 * 2. Create and initialize the snippet view label for this new dialog. 
 *
 * 3. Create a vertical layout _ret_, adding this object's word edit widget then 
 *    its snippet view label. 
 *
 * 4. Return _ret_. 
 */
QLayout* TextEdit::Dialog::setupWordEdit()
{
   // 1
   _wordEdit = new QLineEdit;
   connect(_wordEdit,&QLineEdit::textChanged,this,&Dialog::wordTextChanged);

   // 2
   _snippetView = new QLabel;
   _snippetView->setWordWrap(true);
   _snippetView->setAlignment(Qt::AlignTop);
   _snippetView->setTextFormat(Qt::RichText);

   // 3
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_wordEdit);
   ret->addWidget(_snippetView);

   // 4
   return ret;
}






/*!
 * Constructs and initializes all buttons for this new dialog, returning the layout 
 * containing all the buttons. 
 *
 * @return Pointer to layout of all buttons for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the change, ignore once, ignore all, and cancel buttons for this new 
 *    dialog, connecting all their clicked signals. 
 *
 * 2. Create a new horizontal layout _ret_, adding this object's change, ignore 
 *    once, ignore all buttons, a stretch, and then this object's cancel button. 
 *
 * 3. Return _ret_. 
 */
QLayout* TextEdit::Dialog::setupButtons()
{
   // 1
   _changeButton = new QPushButton(tr("&Change"));
   QPushButton* ignoreOnce {new QPushButton(tr("&Ignore Once"))};
   QPushButton* ignoreAll {new QPushButton(tr("Ignore &All"))};
   QPushButton* cancel {new QPushButton(tr("Canc&el"))};
   connect(_changeButton,&QPushButton::clicked,this,&Dialog::changeClicked);
   connect(ignoreOnce,&QPushButton::clicked,this,&Dialog::ignoreOnceClicked);
   connect(ignoreAll,&QPushButton::clicked,this,&Dialog::ignoreAllClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::accept);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_changeButton);
   ret->addWidget(ignoreOnce);
   ret->addWidget(ignoreAll);
   ret->addStretch();
   ret->addWidget(cancel);

   // 3
   return ret;
}






/*!
 * Constructs and initializes all Aspell library resources for this new dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize this object's Aspell configuration, setting its default 
 *    language, and then create a temporary can have errors Aspell speller _temp_. 
 *    If _temp_ has errors then throw an exception. 
 *
 * 2. Set this object's speller by extracting it from _temp_. 
 */
void TextEdit::Dialog::setupSpeller()
{
   // 1
   _spellConfig = new_aspell_config();
   aspell_config_replace(_spellConfig,"lang",_defaultLang);
   AspellCanHaveError* temp {new_aspell_speller(_spellConfig)};
   if ( aspell_error_number(temp) )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed initializing Aspell library for spell checking: %1")
                   .arg(aspell_error_message(temp)));
      throw e;
   }

   // 2
   _spell = to_aspell_speller(temp);
}
