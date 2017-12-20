#include <QMessageBox>

#include "exception.h"



using namespace Exception;






void Base::show(const QString& text, Icon icon, bool debug) const noexcept
{
   QMessageBox info;
   info.setWindowTitle(title());
   info.setText(text);
   info.setInformativeText(details());
   switch (icon)
   {
   case Icon::Warning:
      info.setIcon(QMessageBox::Warning);
      break;
   case Icon::Critical:
      info.setIcon(QMessageBox::Critical);
      break;
   }
   if ( debug )
   {
      info.setDetailedText(QObject::tr("%1:%2\n\n%3\n").arg(file()).arg(line()).arg(function()));
   }
   info.exec();
}
