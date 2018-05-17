#include "gui_textedit.h"
#include <QAction>
#include <QDebug>
#include "gui_textedit_highlighter.h"
#include "gui_textedit_dialog.h"
#include "gui_textdialog.h"



using namespace Gui;
//



/*!
 * The default language used by this editor's spell checking library Aspell. 
 */
const char* TextEdit::_defaultLang {"en_US"};






/*!
 * Constructs a new text editor with an optional parent. Spell checking and the 
 * dialog popup shortcut are enabled by default. 
 *
 * @param parent Optional parent for this new text editor. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create this text editor's highlighter and setup its shortcut actions. 
 */
TextEdit::TextEdit(QWidget* parent):
   QPlainTextEdit(parent)
{
   // 1
   _spellHighlighter = new Highlighter(document());
   setupActions();
}






/*!
 * Returns the enabled state of this editor's spell checking. 
 *
 * @return True if this editor's spell checking is enabled or false otherwise. 
 */
bool TextEdit::isSpellCheckEnabled() const
{
   return _spellCheckEnabled;
}






/*!
 * Returns the enabled state of this editor's dialog popup shortcut. 
 *
 * @return True if this editor's dialog popup shortcut is enabled or false 
 *         otherwise. 
 */
bool TextEdit::isDialogPopupEnabled() const
{
   return _dialogPopupEnabled;
}






/*!
 * Sets the enabled state of this editor's spell checking with the given state. 
 *
 * @param enabled True to enable this editor's spell checking or false to disable 
 *                it. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given state is to disable spell checking then delete any highlighter 
 *    this editor may have and set its pointer to null, else if this editor has no 
 *    highlighter then create a new one. 
 *
 * 2. Set this editor's spell checking enable state to the new one given. 
 */
void TextEdit::setSpellCheckEnabled(bool enabled)
{
   // 1
   if ( !enabled )
   {
      delete _spellHighlighter;
      _spellHighlighter = nullptr;
   }
   else if ( !_spellHighlighter ) _spellHighlighter = new Highlighter(document());

   // 2
   _spellCheckEnabled = enabled;
}






/*!
 * Set the enabled state of this editor's dialog popup shortcut with the given 
 * state. 
 *
 * @param enabled True to enable this editor's dialog popup shortcut or false to 
 *                disable it. 
 */
void TextEdit::setDialogPopupEnabled(bool enabled)
{
   _dialogPopupEnabled = enabled;
}






/*!
 * Called when this editor's spell checker action is triggered. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this editor's spell checking is disabled then return, else create a new 
 *    spell checker dialog and execute it in modal mode. 
 */
void TextEdit::spellCheckTriggered()
{
   // 1
   if ( !_spellCheckEnabled ) return;
   Dialog spellCheck(this);
   spellCheck.exec();
}






/*!
 * Called when this editor's dialog popup action is triggered. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this editor's dialog popup shortcut is disabled then return. 
 *
 * 2. Create a new text dialog _dialog_, setting its text to this editor's text and 
 *    its title. 
 *
 * 3. Execute _dialog_ in modal mode. If execution returns success then set this 
 *    editor's text to the text of _dialog_. 
 */
void TextEdit::dialogPopupTriggered()
{
   // 1
   if ( !_dialogPopupEnabled ) return;

   // 2
   TextDialog dialog;
   dialog.setWindowTitle("Text Editor");
   dialog.setText(toPlainText());

   // 3
   if ( dialog.exec() )
   {
      setPlainText(dialog.text());
   }
}






/*!
 * Constructs and initializes this new editor's actions for spell checking and 
 * dialog popup shortcut. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new action _spell_ for this editor's spell checking action. 
 *
 * 2. Create a new action _dialog_ for this editor's dialog popup action. 
 *
 * 3. Add _spell_ and _dialog_ to this editor's actions. 
 */
void TextEdit::setupActions()
{
   // 1
   QAction* spell {new QAction(this)};
   spell->setShortcutContext(Qt::WidgetShortcut);
   spell->setShortcut(Qt::CTRL + Qt::Key_S);
   connect(spell,&QAction::triggered,this,&TextEdit::spellCheckTriggered);

   // 2
   QAction* dialog {new QAction(this)};
   dialog->setShortcutContext(Qt::WidgetShortcut);
   dialog->setShortcut(Qt::CTRL + Qt::Key_E);
   connect(dialog,&QAction::triggered,this,&TextEdit::dialogPopupTriggered);

   // 3
   addAction(spell);
   addAction(dialog);
}
