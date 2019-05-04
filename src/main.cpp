#include <QDebug>
#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"






/*!
 * Implements the main function.
 *
 * @param argc See GCC docs.
 *
 * @param argv See GCC docs.
 *
 * @return See GCC docs.
 */
int main(int argc, char** argv)
{
   // Create a new application.
   Application application(argc,argv);

   // Create a new global project factory and set it to this programs global project
   // factory instance.
   ProjectFactory factory;
   Abstract::ProjectFactory::setInstance(&factory);

   // Create a new blank main window and show it.
   Gui::MainWindow* window {new Gui::MainWindow};
   window->show();

   // Execute the application.
   return application.exec();
}
