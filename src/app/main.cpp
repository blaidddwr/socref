#include <QtWidgets>
#include "WidgetWindowMain.h"

int main(int argc, char** argv)
{
    QApplication application(argc,argv);
    Q_INIT_RESOURCE(resources);
    QApplication::setOrganizationName("Galtwe");
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.0-DEV");
    auto window = new Widget::Window::Main;
    window->show();
    return application.exec();
}
