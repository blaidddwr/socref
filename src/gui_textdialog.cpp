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
   setupGui();
}






/*!
 * Returns the plain text from the text edit widget of this dialog. 
 *
 * @return Text from the text edit widget of this dialog. 
 */
QString TextDialog::text() const
{
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
   _edit->setPlainText(text);
}






/*!
 * Constructs and initializes all GUI elements for this new text dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the text edit widget for this dialog, disabling its popup dialog 
 *    feature. 
 *
 * 2. Create a new vertical layout _layout_, adding the text edit widget for this 
 *    dialog and then the buttons. 
 *
 * 3. Set the layout for this dialog to _layout_. 
 */
void TextDialog::setupGui()
{
   // 1
   _edit = new TextEdit;
   _edit->setDialogPopupEnabled(false);

   // 2
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_edit);
   layout->addLayout(setupButtons());

   // 3
   setLayout(layout);
}






/*!
 * Constructs and initializes the OK and cancel buttons for this new text dialog, 
 * returning the horizontal layout containing them. 
 *
 * @return Horizontal layout containing the OK and cancel buttons for this new 
 *         dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the OK and cancel buttons for this dialog, connecting their clicked 
 *    signals. 
 *
 * 2. Create a new horizontal layout _ret_, adding the OK button and then the 
 *    cancel button. 
 *
 * 3. Return _ret_. 
 */
QLayout* TextDialog::setupButtons()
{
   // 1
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&QDialog::accept);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(cancel);

   // 3
   return ret;
}
