#include "Global.h"
#include <QtWidgets>
#include "Test.h"
#include "WidgetWindowMain.h"


int main(
    int argc
    ,char** argv
)
{
    Test::extractArguments(argc,argv);
    QApplication application(argc,argv);
    QApplication::setOrganizationName("Galtwe");
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.0-DEV");
    QCommandLineParser parser;
    parser.setApplicationDescription(
        QApplication::tr(
            "Socrates' Reference is a graphical software design application with the additional"
            " capability of generating source code."
        )
    );
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption testListOption(
        "test-list"
        ,QApplication::tr("List all available unit tests.")
        );
    QCommandLineOption testAllOption("test-all",QApplication::tr("Run all unit test."));
    QCommandLineOption testOption("test",QApplication::tr("Run <test> unit test."),"test");
    parser.addOptions({testListOption,testAllOption,testOption});
    parser.process(application);
    if (parser.isSet(testListOption))
    {
        Test::listTests();
        return 0;
    }
    else if (parser.isSet(testAllOption))
    {
        return Test::executeAll();
    }
    else if (parser.isSet(testOption))
    {
        return Test::execute(parser.value(testOption));
    }
    else
    {
        auto window = new Widget::Window::Main;
        window->show();
        return application.exec();
    }
}
