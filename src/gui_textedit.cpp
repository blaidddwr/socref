#include "gui_textedit.h"
#include "gui_textedit_highlighter.h"
#include "gui_textedit_dialog.h"
#include "gui_textdialog.h"



using namespace Gui;






TextEdit::TextEdit(QWidget* parent):
   QPlainTextEdit(parent)
{
   _spellHighliter = new Highlighter(document());
}






bool TextEdit::isSpellCheckEnabled() const
{
   return _spellCheckEnabled;
}






void TextEdit::setSpellCheckEnabled(bool enabled)
{
   if ( !enabled )
   {
      delete _spellHighliter;
      _spellHighliter = nullptr;
   }
   else if ( !_spellHighliter ) _spellHighliter = new Highlighter(document());
   _spellCheckEnabled = enabled;
}






bool TextEdit::isDialogPopupEnabled() const
{
   return _dialogPopupEnabled;
}






void TextEdit::setDialogPopupEnabled(bool enabled)
{
   _dialogPopupEnabled = enabled;
}






void TextEdit::keyPressEvent(QKeyEvent* event)
{
   if ( event->modifiers()&Qt::ControlModifier && event->modifiers()&Qt::ShiftModifier )
   {
      if ( event->key() == Qt::Key_E && _dialogPopupEnabled )
      {
         TextDialog dialog;
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
