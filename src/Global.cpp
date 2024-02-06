#include "Global.h"
#include <QtWidgets>
#include "Test.h"
#include "WidgetWindowMain.h"


int main(
    int argc
    ,char** argv
)
{
    QApplication application(argc,argv);
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.0-DEV");
    if (argc >= 2)
    {
        if (!strcmp(argv[1],"--test"))
        {
            Test::initialize();
            return Test::execute(argc,argv);
        }
        else if (!strcmp(argv[1],"--test-all"))
        {
            Test::initialize();
            return Test::executeAll(argc,argv);
        }
    }
    auto window = new Widget::Window::Main;
    window->show();
    return application.exec();
}
