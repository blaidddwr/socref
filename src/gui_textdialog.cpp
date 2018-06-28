#include "gui_textdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include "gui_textedit.h"
#include "application.h"



using namespace Gui;
//






/*!
 * Constructs a new text dialog with an optional parent. 
 *
 * @param parent Optional parent for this new text dialog. 
 */
TextDialog::TextDialog(QWidget* parent):
   PersistentDialog("gui.textdialog.geometry",parent)
{
   // Create and initialize the GUI of this dialog. 
   setupGui();
}






/*!
 * Returns the plain text from the text edit widget of this dialog. 
 *
 * @return Text from the text edit widget of this dialog. 
 */
QString TextDialog::text() const
{
   // Return the plain test this object's text edit widget contains. 
   return _edit->toPlainText();
}






/*!
 * Sets the plain text of the text edit widget of this dialog to the given text, 
 * overwriting any text the widget may contain. 
 *
 * @param text New text that the text edit widget of this dialog is set to. 
 */
void TextDialog::setText(const QString& text)
{
   // Set the plain text of this object's text edit widget. 
   _edit->setPlainText(text);
}






/*!
 * Constructs and initializes all GUI elements for this new text dialog. 
 */
void TextDialog::setupGui()
{
   // Create the text edit widget for this dialog and disable its popup dialog 
   // shortcut. 
   _edit = new TextEdit;
   _edit->setDialogPopupEnabled(false);

   // Create a new vertical layout, adding the text edit widget for this dialog and 
   // then the buttons. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_edit);
   layout->addLayout(setupButtons());

   // Set the layout for this dialog. 
   setLayout(layout);
}






/*!
 * Constructs and initializes the OK and cancel buttons for this new text dialog, 
 * returning the horizontal layout containing them. 
 *
 * @return Horizontal layout containing the OK and cancel buttons for this new 
 *         dialog. 
 */
QLayout* TextDialog::setupButtons()
{
   // Create the OK button for this dialog and connect its clicked signals. 
   QPushButton* ok {new QPushButton(tr("Ok"))};
   connect(ok,&QPushButton::clicked,[this]{ done(QDialog::Accepted); });

   // Create the cancel button for this dialog and connect its clicked signals. 
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout, adding the OK button and then the cancel 
   // button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(cancel);

   // Return the buttons layout. 
   return ret;
}
