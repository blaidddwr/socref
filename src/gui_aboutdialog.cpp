#include "gui_aboutdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <exception.h>
#include "application.h"



using namespace Gui;
//






/*!
 * Constructs a new about dialog with the given optional parent. 
 *
 * @param parent Pointer to this new about dialog object's parent or null for no 
 *               parent. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup this object's GUI and set the dialog window's title. 
 */
AboutDialog::AboutDialog(QWidget* parent):
   PersistentDialog("gui.aboutdialog.geometry",parent)
{
   // 1
   setupGui();
   setWindowTitle(tr("About Socrates' Reference"));
}






/*!
 * Constructs all GUI elements for this new about dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new vertical box layout _layout_, adding the top and bottom layout 
 *    sections of this object's GUI in that order. 
 *
 * 2. Set this object's layout to _layout_. 
 */
void AboutDialog::setupGui()
{
   // 1
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupBottom());

   // 2
   setLayout(layout);
}






/*!
 * Constructs the top part of this object's GUI consisting of an image to the left 
 * and description to the right, returning the layout. 
 *
 * @return Pointer to layout of the top section of this object's GUI. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new horizontal box layout _ret_, adding the image and text widgets 
 *    of this object's GUI in that order. 
 *
 * 2. Return _ret_. 
 */
QLayout* AboutDialog::setupTop()
{
   // 1
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(setupImage());
   ret->addWidget(setupText());

   // 2
   return ret;
}






/*!
 * Constructs the image displayed in the top left of this object's GUI, returning a 
 * pointer to the widget. 
 *
 * @return Pointer to widget for the image displayed in the top left of this 
 *         object's GUI. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new label, set its pixmap to the program's icon scaled to the 
 *    correct dimensions, and return it. 
 */
QWidget* AboutDialog::setupImage()
{
   // 1
   QLabel* ret {new QLabel};
   ret->setAlignment(Qt::AlignTop|Qt::AlignRight);
   ret->setPixmap(QPixmap(":/icons/main.svg").scaled(_iconSize,_iconSize));
   return ret;
}






/*!
 * Constructs the rich text displayed in the top right of this object's GUI, 
 * returning a pointer to the widget. 
 *
 * @return Pointer to the widget of rich text displayed in the top right of this 
 *         object's GUI. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new label _ret_, setting its alignment, word wrapping, text format, 
 *    and margin. 
 *
 * 2. Create a new qt file device _file_ and open the about HTML file with it. If 
 *    opening the file fails then throw an exception. 
 *
 * 3. Set the text of the created label _ret_ with the contents of _file_, 
 *    replacing the keys for this program version and the Qt version. 
 *
 * 4. Return _ret_. 
 */
QWidget* AboutDialog::setupText()
{
   // 1
   QLabel* ret {new QLabel};
   ret->setAlignment(Qt::AlignTop);
   ret->setWordWrap(true);
   ret->setTextFormat(Qt::RichText);
   ret->setMargin(8);

   // 2
   QFile file(":/html/about.html");
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed opening about.html: %1").arg(file.errorString()));
      throw e;
   }

   // 3
   QString text {file.readAll()};
   text.replace("%SOCREF_VER%",Application::versionString());
   text.replace("%QT_VER%",QT_VERSION_STR);
   ret->setText(text);

   // 4
   return ret;
}






/*!
 * Constructs the bottom part of this object's GUI which simply consists of an OK 
 * button on the bottom right, returning the layout. 
 *
 * @return Pointer to layout of the bottom section of this object's GUI. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new horizontal box layout _ret_, adding a stretch and the OK button 
 *    of this object's GUI in that order so the OK button is on the right side. 
 *
 * 2. Return _ret_. 
 */
QLayout* AboutDialog::setupBottom()
{
   // 1
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(setupOk());

   // 2
   return ret;
}






/*!
 * Constructs the OK button for this object's GUI, returning a pointer to the 
 * widget. 
 *
 * @return Pointer to the OK button widget of this object's GUI. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new OK push button of this object's GUI, connecting its clicked 
 *    signal to close this dialog, and return it. 
 */
QWidget* AboutDialog::setupOk()
{
   // 1
   QPushButton* ret {new QPushButton(tr("&Close"))};
   connect(ret,&QPushButton::clicked,this,&QDialog::accept);
   return ret;
}
