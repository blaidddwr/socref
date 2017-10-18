#include <QMessageBox>

#include "exception.h"



using namespace Exception;






//@@
void Base::show(const QString& text, Icon icon, bool debug) const noexcept
{
   // create message box and fill in basic info
   QMessageBox info;
   info.setWindowTitle(getTitle());
   info.setText(text);
   info.setInformativeText(getDetails());

   // determine which icon to set for the message box
   switch (icon)
   {
   case Icon::Warning:
      info.setIcon(QMessageBox::Warning);
      break;
   case Icon::Critical:
      info.setIcon(QMessageBox::Critical);
      break;
   }

   // if debug details are requested set details
   if ( debug )
   {
      info.setDetailedText(QObject::tr("%1:%2\n\n%3\n").arg(getFile()).arg(getLine())
                           .arg(getFunction()));
   }

   // modally execute message box
   info.exec();
}
