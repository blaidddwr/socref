#include "application.h"



/*!
 * The organization key used to access this program's qt settings.
 */
const char* Application::_organizationKey {"blaidddwr"};
/*!
 * The application key used to access this program's qt settings.
 */
const char* Application::_applicationKey {"socref"};






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
Application::Application(int& argc, char** argv)
   :
   QApplication(argc,argv)
{
   // Set the organization and application name for this program.
   setOrganizationName(_organizationKey);
   setApplicationName(_applicationKey);
}

