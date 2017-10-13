#include <QApplication>

#include "gui_mainwindow.h"
#include "projectfactory.h"






int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   ProjectFactory factory;
   AbstractProjectFactory::setInstance(&factory);
   Gui::MainWindow w;
   w.show();

   return a.exec();
}
