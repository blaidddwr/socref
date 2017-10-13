#include <QApplication>

#include "gui_mainwindow.h"






int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   Gui::MainWindow w;
   w.show();

   return a.exec();
}
