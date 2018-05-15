#include "gui_textedit.h"
#include "gui_textedit_highlighter.h"
#include "gui_textedit_dialog.h"
#include "gui_textdialog.h"



using namespace Gui;
//



/*!
 */
const char* TextEdit::_defaultLang {"en_US"};






/*!
 *
 * @param parent  
 */
TextEdit::TextEdit(QWidget* parent):
   QPlainTextEdit(parent)
{
   _spellHighlighter = new Highlighter(document());
}






/*!
 */
bool TextEdit::isSpellCheckEnabled() const
{
   return _spellCheckEnabled;
}






/*!
 *
 * @param enabled  
 */
void TextEdit::setSpellCheckEnabled(bool enabled)
{
   if ( !enabled )
   {
      delete _spellHighlighter;
      _spellHighlighter = nullptr;
   }
   else if ( !_spellHighlighter ) _spellHighlighter = new Highlighter(document());
   _spellCheckEnabled = enabled;
}






/*!
 */
bool TextEdit::isDialogPopupEnabled() const
{
   return _dialogPopupEnabled;
}






/*!
 *
 * @param enabled  
 */
void TextEdit::setDialogPopupEnabled(bool enabled)
{
   _dialogPopupEnabled = enabled;
}






/*!
 *
 * @param event  
 */
void TextEdit::keyPressEvent(QKeyEvent* event)
{
   if ( event->modifiers()&Qt::ControlModifier && event->modifiers()&Qt::ShiftModifier )
   {
      if ( event->key() == Qt::Key_E && _dialogPopupEnabled )
      {
         TextDialog dialog;
         dialog.setWindowTitle("Text Editor");
         dialog.setText(toPlainText());
         if ( dialog.exec() )
         {
            setPlainText(dialog.text());
         }
      }
      else if ( event->key() == Qt::Key_C && _spellCheckEnabled )
      {
         Dialog spellCheck(this);
         spellCheck.exec();
      }
      else QPlainTextEdit::keyPressEvent(event);
   }
   else QPlainTextEdit::keyPressEvent(event);
}
