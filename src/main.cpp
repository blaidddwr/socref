#include <QDebug>
#include <exception.h>
#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"






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
      qDebug().nospace() << "Location: " << e.file() << ":" << e.line();
      qDebug().nospace() << "Function: " << e.function();
      qDebug().nospace() << "Title: " << e.title();
      qDebug().nospace() << "Details: " << e.details();
      return -1;
   }
}
