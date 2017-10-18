#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"
#include "exception.h"






int main(int argc, char *argv[])
{
   // initialize Qt
   Application application(argc, argv);
   try
   {
      // initialize project factory
      ProjectFactory factory;
      AbstractProjectFactory::setInstance(&factory);

      // create first window
      Gui::MainWindow* window {new Gui::MainWindow};
      window->show();

      // execute Qt
      return application.exec();
   }

   // if any unknown exception occurs let user know and terminate
   catch (Exception::Base e)
   {
      e.show(QObject::tr("An unexpected error occured! Application will terminate immediately.")
             ,Exception::Icon::Critical,true);
      return -1;
   }
}
