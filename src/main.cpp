#include <QDebug>
#include <exception.h>
#include "application.h"
#include "gui_mainwindow.h"
#include "projectfactory.h"



//






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
      qDebug().nospace().noquote() << QStringLiteral("EXCEPTION");
      qDebug().nospace().noquote() << QString("Location: %1:%2").arg(e.file()).arg(e.line());
      qDebug().nospace().noquote() << QStringLiteral("Function: ") << e.function();
      qDebug().nospace().noquote() << QStringLiteral("Title: ") << e.title();
      qDebug().nospace().noquote() << QStringLiteral("Details: ") << e.details();
   }
   catch (std::exception e)
   {
      qDebug().nospace() << QString("EXCEPTION(%1)").arg(e.what());
   }
   catch (...)
   {
      qDebug() << QStringLiteral("EXCEPTION(unknown!)");
   }
}
