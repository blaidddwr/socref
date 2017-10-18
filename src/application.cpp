#include <QMessageBox>

#include "application.h"
#include "exception.h"






//@@
bool Application::notify(QObject* receiver, QEvent* event)
{
   // process the event like normal
   try
   {
      return QApplication::notify(receiver,event);
   }

   // if an exception occurs report it
   catch (Exception::Base e)
   {
      e.show(QObject::tr("An unexpected error occured!"),Exception::Icon::Critical,true);
   }

   // if an std exception occurs report it
   catch (std::exception e)
   {
      QMessageBox info;
      info.setIcon(QMessageBox::Critical);
      info.setWindowTitle(e.what());
      info.setText(tr("An unexpected standard library exception occured!"));
      info.exec();
   }

   // if any other unknown exception occurs report it
   catch (...)
   {
      QMessageBox info;
      info.setIcon(QMessageBox::Critical);
      info.setWindowTitle(tr("Unknown Exception"));
      info.setText(tr("An unexpected and unknown exception occured!"));
      info.exec();
   }

   // if any exception occured signal this event should be propagated up
   return false;
}
