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
 *
 * @param parent  
 */
TextDialog::TextDialog(QWidget* parent):
   PersistentDialog("gui.textdialog.geometry",parent)
{
   setupGui();
}






/*!
 */
QString TextDialog::text() const
{
   return _edit->toPlainText();
}






/*!
 *
 * @param text  
 */
void TextDialog::setText(const QString& text)
{
   _edit->setPlainText(text);
}






/*!
 */
void TextDialog::setupGui()
{
   _edit = new TextEdit;
   _edit->setDialogPopupEnabled(false);
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_edit);
   layout->addLayout(setupButtons());
   setLayout(layout);
}






/*!
 */
QLayout* TextDialog::setupButtons()
{
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   connect(ok,&QPushButton::clicked,this,&QDialog::accept);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(cancel);
   return ret;
}
