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
 * 1. Create a new label _image_, setting its pixmap to the program's icon scaled 
 *    to the correct dimensions. 
 *
 * 2. Create a new horizontal box layout _ret_, adding the _image_ label and then 
 *    the layout returned from this object's setup text method. 
 *
 * 3. Return _ret_. 
 */
QLayout* AboutDialog::setupTop()
{
   // 1
   QLabel* image {new QLabel};
   image->setAlignment(Qt::AlignTop|Qt::AlignRight);
   image->setPixmap(QPixmap(":/icons/main.svg").scaled(_iconSize,_iconSize));

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(image);
   ret->addWidget(setupText());

   // 3
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
 * 1. Create a new push button _ok_, connecting its clicked signal to close this 
 *    dialog. 
 *
 * 2. Create a new horizontal box layout _ret_, adding a stretch and then the _ok_ 
 *    button. 
 *
 * 3. Return _ret_. 
 */
QLayout* AboutDialog::setupBottom()
{
   // 1
   QPushButton* ok {new QPushButton(tr("&Close"))};
   connect(ok,&QPushButton::clicked,this,&QDialog::accept);

   // 2
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(ok);

   // 3
   return ret;
}
