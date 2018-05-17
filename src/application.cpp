#include "application.h"
#include <QDebug>
#include <exception.h>
//



/*!
 * The company key used to access this program's qt settings. 
 */
const char* Application::_companyKey {"blaidddwr"};
/*!
 * The program key used to access this program's qt settings. 
 */
const char* Application::_programKey {"socref"};






/*!
 * Constructs a new application instance. 
 *
 * @param argc The argument count from main. 
 *
 * @param argv The list of arguments from main. 
 */
Application::Application(int& argc, char** argv):
   QApplication(argc,argv)
{}






/*!
 * This implements the interface for handling events in the main event loop. The 
 * only thing this does is add a catch section so exceptions are handled. 
 *
 * @param receiver Pointer to the object that received the event. 
 *
 * @param event Pointer to the event that has happened. 
 *
 * @return True if the event was processed or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call the Qt application notify method and return its value. 
 *
 * 2. If any exception is caught then report it to the qt debug stream and return 
 *    false. 
 */
bool Application::notify(QObject* receiver, QEvent* event)
{
   // 1
   try
   {
      return QApplication::notify(receiver,event);
   }

   // 2
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






/*!
 * Returns this programs version as a string. 
 *
 * @return This programs version as a string. 
 */
QString Application::versionString()
{
   return QString::number(Application::_majorVersion).append(".")
                                                     .append(QString::number(Application::_minorVersion))
                                                     .append(".")
                                                     .append(QString::number(Application::_revision));
}
