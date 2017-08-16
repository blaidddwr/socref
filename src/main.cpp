#include "mainwindow.h"
#include <QApplication>
#include "singleton.h"
#include "singletonfactory.h"
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();

   return a.exec();
}
