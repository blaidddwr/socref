#include <QDebug>
#include <exception.h>
#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"
//






/*!
 * Implements the main function. 
 *
 * @param argc See GCC docs. 
 *
 * @param argv See GCC docs. 
 */
int main(int argc, char** argv)
{
   // Create a new application. 
   Application application(argc,argv);
   try
   {
      // Create a new global project factory and set it to this programs global project 
      // factory instance. 
      ProjectFactory factory;
      AbstractProjectFactory::setInstance(&factory);

      // Create a new blank main window and show it. 
      Gui::MainWindow* window {new Gui::MainWindow};
      window->show();

      // Execute the application. 
      return application.exec();
   }

   // Catch any thrown exception, reporting it and exiting this program with a 
   // failure code. 
   catch (Exception::Base e)
   {
      qDebug().nospace().noquote() << QStringLiteral("EXCEPTION");
      qDebug().nospace().noquote() << QString("Location: %1:%2").arg(e.file()).arg(e.line());
      qDebug().nospace().noquote() << QStringLiteral("Function: ") << e.function();
      qDebug().nospace().noquote() << QStringLiteral("Title: ") << e.title();
      qDebug().nospace().noquote() << QStringLiteral("Details: ") << e.details();
      return -1;
   }
   catch (std::exception e)
   {
      qDebug().nospace() << QString("EXCEPTION(%1)").arg(e.what());
      return -1;
   }
   catch (...)
   {
      qDebug() << QStringLiteral("EXCEPTION(unknown!)");
      return -1;
   }
}
