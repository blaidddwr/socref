#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"
#include "exception.h"






int main(int argc, char** argv)
{
   Application application(argc,argv);
   try
   {
      ProjectFactory factory;
      AbstractProjectFactory::setInstance(&factory);
      Gui::MainWindow* window {new Gui::MainWindow};
      window->show();
      return application.exec();
   }
   catch (Exception::Base e)
   {
      e.show(QObject::tr("An unexpected error occured! Application will terminate immediately."),Exception::Icon::Critical,true);
      return -1;
   }
}
