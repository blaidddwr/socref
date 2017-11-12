#include <QMessageBox>

#include "application.h"
#include "exception.h"






bool Application::notify(QObject* receiver, QEvent* event)
{
   try
   {
      return QApplication::notify(receiver,event);
   }
   catch (Exception::Base e)
   {
      e.show(QObject::tr("An unexpected error occured!"),Exception::Icon::Critical,true);
   }
   catch (std::exception e)
   {
      QMessageBox info;
      info.setIcon(QMessageBox::Critical);
      info.setWindowTitle(e.what());
      info.setText(tr("An unexpected standard library exception occured!"));
      info.exec();
   }
   catch (...)
   {
      QMessageBox info;
      info.setIcon(QMessageBox::Critical);
      info.setWindowTitle(tr("Unknown Exception"));
      info.setText(tr("An unexpected and unknown exception occured!"));
      info.exec();
   }
   return false;
}
