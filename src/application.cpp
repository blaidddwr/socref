#include <QDebug>
#include "application.h"
#include "exception_base.h"






Application::Application(int& argc, char** argv):
   QApplication(argc,argv)
{}






bool Application::notify(QObject* receiver, QEvent* event)
{
   try
   {
      return QApplication::notify(receiver,event);
   }
   catch (Exception::Base e)
   {
      qDebug().nospace() << "Exception Caught!";
      qDebug().nospace() << "Location: " << e.file() << ":" << e.line();
      qDebug().nospace() << "Function: " << e.function();
      qDebug().nospace() << "Title: " << e.title();
      qDebug().nospace() << "Details: " << e.details() << "\n";
   }
   catch (std::exception e)
   {
      qDebug().nospace() << "Exception Caught!";
      qDebug().nospace() << e.what() << "\n";
   }
   catch (...)
   {
      qDebug() << "Exception Caught!\n";
   }
   return false;
}
