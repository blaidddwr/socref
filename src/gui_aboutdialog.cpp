#include "gui_aboutdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>



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
   setupGui();
   setWindowTitle(tr("About Socrates' Reference"));
}






/*!
 * Constructs all GUI elements for this new about dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new vertical box layout, adding the top and bottom layout sections 
 *    of this object's GUI in that order. 
 *
 * 2. Set this object's layout to the created vertical box layout. 
 */
void AboutDialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupBottom());
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
 * 1. Create a new horizontal box layout, adding the image and text widgets of this 
 *    object's GUI in that order. 
 *
 * 2. Return the created horizontal box layout. 
 */
QLayout* AboutDialog::setupTop()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(setupImage());
   ret->addWidget(setupText());
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
 * 1. Create a new label, setting its alignment, word wrapping, text format, and 
 *    margin. 
 *
 * 2. Set the text of the created label; the name of this program and its version, 
 *    the version of qt and the compiler used, and a funny couple paragraphs about 
 *    copyright. 
 *
 * 3. Return the new label. 
 */
QWidget* AboutDialog::setupText()
{
   QLabel* ret {new QLabel};
   ret->setAlignment(Qt::AlignTop);
   ret->setWordWrap(true);
   ret->setTextFormat(Qt::RichText);
   ret->setMargin(8);
   QString text {"<h3>Socrates' Reference 0.4.0</h3>"};
   text.append("<p>Based on Qt ").append(QT_VERSION_STR).append("</p>");
   text.append("<p><b>FUCK COPYRIGHT LAWS</b></p>");
   text.append("<p>Copyright has been perverted as a tool of oppression and censorship used by the kleptocrats of the fascist west to rape and pillage individual creators. This is a full 180 to what copyright should be; to protoect the individual creator. So FUCK COPYRIGHT.</p>");
   text.append("<p>This program is provided as is with no warranty of any kind, including the warranty of design, merchantability and fitness for a particular purpose.</p>");
   ret->setText(text);
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
 * 1. Create a new horizontal box layout, adding a stretch and the OK button of 
 *    this object's GUI in that order so the OK button is on the right side. 
 *
 * 2. Return the created horizontal box layout. 
 */
QLayout* AboutDialog::setupBottom()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(setupOk());
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
   QPushButton* ret {new QPushButton(tr("&Close"))};
   connect(ret,&QPushButton::clicked,this,&QDialog::accept);
   return ret;
}
