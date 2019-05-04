#include "gui_textedit.h"
#include <QAction>
#include <QDebug>
#include "gui_spellerhighlighter.h"
#include "gui_spellerdialog.h"
#include "gui_textdialog.h"
#include "abstract_block.h"
#include "project.h"



namespace Gui
{



/*!
 * The default language used by this editor's spell checking library Aspell.
 */
const char* TextEdit::_defaultLang {"en_US"};






/*!
 * Constructs a new text editor with the given block and an optional parent.
 * Spell checking and the dialog popup shortcut are enabled by default.
 *
 * @param block Pointer to the block that is contextually being used. This
 *              usually means the block that is being edited. This is used to
 *              get the custom dictionary model of the block's project.
 *
 * @param parent Optional parent for this new text editor.
 */
TextEdit::TextEdit(Abstract::Block* block, QWidget* parent)
   :
   QPlainTextEdit(parent)
{
   // Make sure the given block pointer is not null.
   Q_ASSERT(block);

   // Get the project pointer from the given block making sure it is not null.
   Project* project {qobject_cast<Project*>(block->root()->QObject::parent())};
   Q_ASSERT(project);

   // Set the custom dictionary to the project's dictionary.
   _dictionary = project->dictionary();

   // Create this text editor's highlighter and setup its shortcut actions.
   _spellHighlighter = new SpellerHighlighter(_dictionary,_defaultLang,document());
   setupActions();
}






/*!
 * Constructs a new text editor with the given custom dictionary and an optional
 * parent. Spell checking and the dialog popup shortcut are enabled by default.
 *
 * @param dictionary The custom dictionary model this new text dialog uses to
 *                   check for custom spell checking words.
 *
 * @param parent Optional parent for this new text editor.
 */
TextEdit::TextEdit(DictionaryModel* dictionary, QWidget* parent)
   :
   QPlainTextEdit(parent),
   _dictionary(dictionary)
{
   // Make sure the given custom dictionary pointer is not null.
   Q_ASSERT(dictionary);

   // Create this text editor's highlighter and setup its shortcut actions.
   _spellHighlighter = new SpellerHighlighter(_dictionary,_defaultLang,document());
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
 * @param enabled True to enable this editor's spell checking or false to
 *                disable it.
 */
void TextEdit::setSpellCheckEnabled(bool enabled)
{
   // Check to see if spell checking is being disabled.
   if ( !enabled )
   {
      // Delete any highlighter this editor may have and set its pointer to null.
      delete _spellHighlighter;
      _spellHighlighter = nullptr;
   }

   // Else if this editor has no highlighter then create a new one.
   else if ( !_spellHighlighter ) _spellHighlighter = new SpellerHighlighter(_dictionary,_defaultLang,document());

   // Update this editor's spell checking enable state.
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
 */
void TextEdit::spellCheckTriggered()
{
   // Make sure spell checking is enabled.
   if ( !_spellCheckEnabled ) return;

   // Create a new spell checker dialog and execute it in modal mode.
   SpellerDialog dialog(_dictionary,_defaultLang,document(),this);
   dialog.exec();

   // Reset the text of this widget so any added custom spell check words are no
   // longer marked as misspelled in the highlighter.
   setPlainText(toPlainText());
}






/*!
 * Called when this editor's dialog popup action is triggered.
 */
void TextEdit::dialogPopupTriggered()
{
   // Make sure this editor's dialog popup action is enabled.
   if ( !_dialogPopupEnabled ) return;

   // Create a new text dialog, setting its content to this editor's text and its
   // title.
   TextDialog dialog(_dictionary);
   dialog.setWindowTitle("Text Editor");
   dialog.setText(toPlainText());

   // Execute the text dialog in modal mode and check to see if it returns accept.
   if ( dialog.exec() )
   {
      // Update this text editor's contents with the text of the text dialog.
      setPlainText(dialog.text());
   }
}






/*!
 * Constructs and initializes this new editor's actions for spell checking and
 * dialog popup shortcut.
 */
void TextEdit::setupActions()
{
   // Create and initialize this editor's spell checking action and connect its
   // triggered signal.
   QAction* spell {new QAction(this)};
   spell->setShortcutContext(Qt::WidgetShortcut);
   spell->setShortcut(Qt::CTRL + Qt::Key_S);
   connect(spell,&QAction::triggered,this,&TextEdit::spellCheckTriggered);

   // Create and initialize this editor's dialog popup action and connect its
   // triggered signal.
   QAction* dialog {new QAction(this)};
   dialog->setShortcutContext(Qt::WidgetShortcut);
   dialog->setShortcut(Qt::CTRL + Qt::Key_E);
   connect(dialog,&QAction::triggered,this,&TextEdit::dialogPopupTriggered);

   // Add this editor's shortcut actions.
   addAction(spell);
   addAction(dialog);
}

}
