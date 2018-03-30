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
 * 1. Simply call the Qt notify method but wrap it in a catch statement so 
 *    exceptions can be handled in this program. 
 */
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
