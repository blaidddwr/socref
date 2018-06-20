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
 */
AboutDialog::AboutDialog(QWidget* parent):
   PersistentDialog("gui.aboutdialog.geometry",parent)
{
   // Setup this object's GUI and set the dialog window's title. 
   setupGui();
   setWindowTitle(tr("About Socrates' Reference"));
}






/*!
 * Constructs all GUI elements for this new about dialog. 
 */
void AboutDialog::setupGui()
{
   // Create a new vertical box layout _layout_, adding the top and bottom layout 
   // sections of this object's GUI in that order. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupBottom());

   // Set this object's layout to _layout_. 
   setLayout(layout);
}






/*!
 * Constructs the top part of this object's GUI consisting of an image to the left 
 * and description to the right, returning the layout. 
 *
 * @return Pointer to layout of the top section of this object's GUI. 
 */
QLayout* AboutDialog::setupTop()
{
   // Create a new label _image_, setting its pixmap to the program's icon scaled to 
   // the correct dimensions. 
   QLabel* image {new QLabel};
   image->setAlignment(Qt::AlignTop|Qt::AlignRight);
   image->setPixmap(QPixmap(":/icons/main.svg").scaled(_iconSize,_iconSize));

   // Create a new horizontal box layout _ret_, adding the _image_ label and then the 
   // layout returned from this object's setup text method. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(image);
   ret->addWidget(setupText());

   // Return _ret_. 
   return ret;
}






/*!
 * Constructs the rich text displayed in the top right of this object's GUI, 
 * returning a pointer to the widget. 
 *
 * @return Pointer to the widget of rich text displayed in the top right of this 
 *         object's GUI. 
 */
QWidget* AboutDialog::setupText()
{
   // Create a new label _ret_, setting its alignment, word wrapping, text format, 
   // and margin. 
   QLabel* ret {new QLabel};
   ret->setAlignment(Qt::AlignTop);
   ret->setWordWrap(true);
   ret->setTextFormat(Qt::RichText);
   ret->setMargin(8);

   // Create a new qt file device _file_ and open the about HTML file with it. If 
   // opening the file fails then throw an exception. 
   QFile file(":/html/about.html");
   if ( !file.open(QIODevice::ReadOnly) )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed opening about.html: %1").arg(file.errorString()));
      throw e;
   }

   // Set the text of the created label _ret_ with the contents of _file_, replacing 
   // the keys for this program version and the Qt version. 
   QString text {file.readAll()};
   text.replace("%SOCREF_VER%",Application::versionString());
   text.replace("%QT_VER%",QT_VERSION_STR);
   ret->setText(text);

   // Return _ret_. 
   return ret;
}






/*!
 * Constructs the bottom part of this object's GUI which simply consists of an OK 
 * button on the bottom right, returning the layout. 
 *
 * @return Pointer to layout of the bottom section of this object's GUI. 
 */
QLayout* AboutDialog::setupBottom()
{
   // Create a new push button _ok_, connecting its clicked signal to close this 
   // dialog. 
   QPushButton* ok {new QPushButton(tr("&Close"))};
   connect(ok,&QPushButton::clicked,[this]{ done(QDialog::Accepted); });

   // Create a new horizontal box layout _ret_, adding a stretch and then the _ok_ 
   // button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(ok);

   // Return _ret_. 
   return ret;
}
