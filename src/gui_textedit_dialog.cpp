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
 */
const char* TextEdit::Dialog::_snippetDots {"..."};






/*!
 *
 * @param parent  
 */
TextEdit::Dialog::Dialog(TextEdit* parent):
   PersistentDialog("gui.textedit.dialog.geometry",parent),
   _edit(parent)
{
   setupSpeller();
   setupGui();
   setWindowTitle("Spell Checker");
}






/*!
 */
TextEdit::Dialog::~Dialog()
{
   delete_aspell_speller(_spell);
   delete_aspell_config(_spellConfig);
}






/*!
 */
int TextEdit::Dialog::exec()
{
   show();
   if ( findNextWord() ) return QDialog::exec();
   return -1;
}






/*!
 */
void TextEdit::Dialog::changeClicked()
{
   QString text {_edit->toPlainText()};
   text.remove(_offset,_length);
   text.insert(_offset,_wordEdit->text());
   _edit->setPlainText(text);
   findNextWord();
}






/*!
 */
void TextEdit::Dialog::ignoreOnceClicked()
{
   _offset += _length;
   findNextWord();
}






/*!
 */
void TextEdit::Dialog::ignoreAllClicked()
{
   _ignored << _currentWord;
   findNextWord();
}






/*!
 *
 * @param text  
 */
void TextEdit::Dialog::wordTextChanged(const QString& text)
{
   _changeButton->setDisabled(!isCorrectWord(text));
}






/*!
 */
bool TextEdit::Dialog::findNextWord()
{
   QString text {_edit->toPlainText()};
   QRegularExpression pattern("[\\w'-]+");
   QRegularExpressionMatch match {pattern.match(text,_offset)};
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
   return queryEndOfText();
}






/*!
 *
 * @param word  
 */
bool TextEdit::Dialog::isCorrectWord(const QString& word)
{
   if ( _ignored.contains(word) ) return true;
   QByteArray utfWord {word.toLocal8Bit()};
   return aspell_speller_check(_spell,utfWord.data(),utfWord.size());
}






/*!
 */
bool TextEdit::Dialog::queryEndOfText()
{
   QMessageBox box;
   box.setWindowTitle(tr("Spell Check Complete"));
   box.setText(tr("End of text has been reached. Do you want to run spell check again from beginning of text?"));
   box.addButton(tr("No"),QMessageBox::RejectRole);
   box.addButton(tr("Yes"),QMessageBox::AcceptRole);
   if ( box.exec() )
   {
      _offset = 0;
      _length = 0;
      return findNextWord();
   }
   else
   {
      accept();
      return false;
   }
}






/*!
 *
 * @param text  
 *
 * @param match  
 */
void TextEdit::Dialog::setWord(const QString& text, QRegularExpressionMatch& match)
{
   _currentWord = match.captured();
   _wordEdit->setText(_currentWord);
   setSnippet(text,match.capturedStart(),match.capturedLength());
   updateSuggested();
}






/*!
 */
void TextEdit::Dialog::clearWord()
{
   _wordEdit->clear();
   _snippetView->clear();
}






/*!
 *
 * @param text  
 *
 * @param begin  
 *
 * @param length  
 */
void TextEdit::Dialog::setSnippet(const QString& text, int begin, int length)
{
   int begin1 {begin - _snippetRadius};
   int length1 {begin};
   if ( begin1 < 0 ) begin1 = 0;
   int begin2 {begin + length};
   QString sentence {_snippetDots};
   sentence.append(text.mid(begin1,length1));
   sentence.append("<b>").append(text.mid(begin,length)).append("</b>");
   sentence.append(text.mid(begin2,_snippetRadius)).append(_snippetDots);
   _snippetView->setText(sentence);
}






/*!
 */
void TextEdit::Dialog::updateSuggested()
{
   QByteArray utfWord {_currentWord.toLocal8Bit()};
   const AspellWordList* suggestions {aspell_speller_suggest(_spell,utfWord.data(),utfWord.size())};
   AspellStringEnumeration* elements {aspell_word_list_elements(suggestions)};
   bool first {true};
   const char* suggest;
   QString text{"<h3>Suggestions</h3>"};
   while ( (suggest = aspell_string_enumeration_next(elements)) != nullptr )
   {
      if ( first ) first = false;
      else text.append(" ");
      text.append(suggest);
   }
   _suggestionsView->setText(text);
   delete_aspell_string_enumeration(elements);
}






/*!
 */
void TextEdit::Dialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupButtons());
   setLayout(layout);
}






/*!
 */
QLayout* TextEdit::Dialog::setupTop()
{
   setupSuggestions();
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addLayout(setupWordEdit());
   ret->addWidget(_suggestionsView);
   return ret;
}






/*!
 */
QLayout* TextEdit::Dialog::setupButtons()
{
   _changeButton = new QPushButton(tr("&Change"));
   QPushButton* ignoreOnce {new QPushButton(tr("&Ignore Once"))};
   QPushButton* ignoreAll {new QPushButton(tr("Ignore &All"))};
   QPushButton* cancel {new QPushButton(tr("Canc&el"))};
   connect(_changeButton,&QPushButton::clicked,this,&Dialog::changeClicked);
   connect(ignoreOnce,&QPushButton::clicked,this,&Dialog::ignoreOnceClicked);
   connect(ignoreAll,&QPushButton::clicked,this,&Dialog::ignoreAllClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_changeButton);
   ret->addWidget(ignoreOnce);
   ret->addWidget(ignoreAll);
   ret->addStretch();
   ret->addWidget(cancel);
   return ret;
}






/*!
 */
QLayout* TextEdit::Dialog::setupWordEdit()
{
   _wordEdit = new QLineEdit;
   setupSnippet();
   connect(_wordEdit,&QLineEdit::textChanged,this,&Dialog::wordTextChanged);
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_wordEdit);
   ret->addWidget(_snippetView);
   return ret;
}






/*!
 */
void TextEdit::Dialog::setupSnippet()
{
   _snippetView = new QLabel;
   _snippetView->setWordWrap(true);
   _snippetView->setAlignment(Qt::AlignTop);
   _snippetView->setTextFormat(Qt::RichText);
}






/*!
 */
void TextEdit::Dialog::setupSuggestions()
{
   _suggestionsView = new QLabel;
   _suggestionsView->setAlignment(Qt::AlignTop);
   _suggestionsView->setStyleSheet("margin-left:16px;");
   _suggestionsView->setWordWrap(true);
}






/*!
 */
void TextEdit::Dialog::setupSpeller()
{
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
   _spell = to_aspell_speller(temp);
}
