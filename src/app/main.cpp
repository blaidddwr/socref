#include <QtWidgets>
#include "CppLanguage.h"
#include "CppQtLanguage.h"
#include "FactoryLanguage.h"
#include "WidgetWindowMain.h"

int main(int argc, char** argv)
{
    QApplication application(argc,argv);
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    Q_INIT_RESOURCE(cppqt);
    QApplication::setOrganizationName("Galtwe");
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.0-DEV");
    Factory::Language::instance()->appendLanguage(new Cpp::Language);
    Factory::Language::instance()->appendLanguage(new CppQt::Language);
    auto window = new Widget::Window::Main;
    window->show();
    return application.exec();
}
