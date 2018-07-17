#include "application.h"
#include <QDebug>
#include <exception.h>
//



/*!
 * The organization key used to access this program's qt settings. 
 */
const char* Application::_organizationKey {"blaidddwr"};
/*!
 * The application key used to access this program's qt settings. 
 */
const char* Application::_applicationKey {"socref"};






/*!
 * Implements _QCoreApplication_ interface. The only thing this does is add a catch 
 * section so exceptions are handled. 
 *
 * @param receiver See Qt docs. 
 *
 * @param event See Qt docs. 
 *
 * @return See Qt docs. 
 */
bool Application::notify(QObject* receiver, QEvent* event)
{
   // Call the qt implementation of this method. 
   try
   {
      return QApplication::notify(receiver,event);
   }

   // Catch any thrown exception and report it to debug output. 
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

   // Return the code that says this event has not been handled because an exception 
   // was thrown. 
   return false;
}






/*!
 * Returns this programs version as a string. 
 *
 * @return This programs version as a string. 
 */
QString Application::versionString()
{
   return QString("%1.%2.%3").arg(_majorVersion).arg(_minorVersion).arg(_revision);
}






/*!
 * Constructs a new application instance. 
 *
 * @param argc The argument count from main. 
 *
 * @param argv The list of arguments from main. 
 */
Application::Application(int& argc, char** argv):
   QApplication(argc,argv)
{
   // Set the organization and application name for this program. 
   setOrganizationName(_organizationKey);
   setApplicationName(_applicationKey);
}
